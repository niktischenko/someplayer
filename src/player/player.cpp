/*
 * SomePlayer - An alternate music player for Maemo 5
 * Copyright (C) 2010 Nikolay (somebody) Tischenko <niktischenko@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "player.h"
#include <phonon/MediaSource>
#include <phonon/Effect>
#include <phonon/BackendCapabilities>
#include <phonon/EffectParameter>
#include "../config.h"
#include <QTime>
#include <QTimer>

using namespace SomePlayer::Playback;
using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

inline QList<Track> __sub(QList<Track> one, QList<Track> two, Track three) {
	QList<Track> result;
	foreach (Track t, one) {
		if (!two.contains(t) && !(t == three)) {
			result.append(t);
		}
	}
	return result;
}

Player::Player(QObject *parent) :
    QObject(parent)
{
	_player = new Phonon::MediaObject(this);
	_output = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	_player->setTickInterval(1000);
	_equalizer = NULL;
	_equalizer_enabled = false;
	connect(_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(_stateChanged(Phonon::State,Phonon::State)));
	connect(_player, SIGNAL(tick(qint64)), this, SLOT(_tick(qint64)));
	connect(_player, SIGNAL(finished()), this, SLOT(next()));
	_path = Phonon::createPath(_player, _output);
	QList<Phonon::EffectDescription> effects = Phonon::BackendCapabilities::availableAudioEffects();
	foreach (Phonon::EffectDescription desc, effects) {
		if (desc.name() == "equalizer-10bands") {
			_equalizer = new Phonon::Effect(desc, this);
			Config config;
			if (config.equalizerEnabled()) {
				for (int i = 0; i < 10; i++) {
					QVariant var = config.getEqualizerValue(QString("band%1").arg(i));
					setEqualizerValue(i, var.toDouble());
				}
				enableEqualizer();
			}
		}
	}
	int seed = QTime::currentTime().msec();
	qsrand(seed);
	_random = _config.getValue("playback/random").toBool();
	_repeat = (RepeatRule) _config.getValue("playback/repeat").toInt();
}

void Player::setTrackId(int id) {
	_to_history(_track);
	_track = _playlist.tracks().at(id);
	_set_source();
	play();
}

void Player::toggle() {
	if (_state == PLAYER_PLAYING) { // pause
		_player->pause();
		_state = PLAYER_PAUSED;
		emit stateChanged(_state);
	} else { //play
		play();
	}
}

void Player::stop() {
	_player->stop();
	_state = PLAYER_STOPPED;
	emit stateChanged(_state);
}

void Player::next() {
	int count = _playlist.tracks().count();
	if (count == 0) {
		stop(); // empty playlist
		return;
	}
	if (_repeat == REPEAT_ONE) {
		_set_source();
		play();
		return;
	}
	Track _new;
	while (!_queue.isEmpty()) {
		_new = _queue.takeFirst();
		if (_playlist.tracks().contains(_new)) {
			_to_history(_track);
			_track = _new;
			_set_source();
			play();
			return;
		}
	}
	if (!_random) {
		int pos = _playlist.tracks().indexOf(_track) + 1;
		if (pos >= _playlist.tracks().count()) {
			if (_repeat == REPEAT_NO) {
				stop();
			}
			pos %= _playlist.tracks().count();
		}
		_to_history(_track);
		_track = _playlist.tracks().at(pos);
		_set_source();
		play();
		return;
	}
	// random
	QList<Track> sub = __sub(_playlist.tracks(), _history, _track);
	int size = sub.count();
	if (size == 0) {
		_to_history(_track);
		_set_source();
		play();
		return;
	}
	int pos = qrand() % size;
	_to_history(_track);
	_track = sub.at(pos);
	_set_source();
	play();
}

void Player::_set_source() {
	_player->setCurrentSource(Phonon::MediaSource(_track.source()));
	emit trackChanged(_track);
}

void Player::prev() {
	if (_history.isEmpty()) {
		_set_source();
		play();
		return;
	}
	_queue.push_front(_track);
	_track = _history.takeFirst();
	_set_source();
	play();
}

void Player::_stateChanged(Phonon::State newState, Phonon::State /*oldState*/) {
	switch (newState) {
	case Phonon::PlayingState:
		_state = PLAYER_PLAYING;
		emit stateChanged(_state);
		break;
	case Phonon::ErrorState:
		play(); // force
		break;
	default:
		break;
	}
}

void Player::_tick(qint64 ticks) {
	int done = ticks/1000;
	int all = _track.metadata().length();
	emit tick(done, all);
	if (done+2 == all) {
		_track.setCount(_track.count()+1);
		emit trackDone(_track);
	}
}

void Player::setPlaylist(Playlist playlist) {
	_playlist = playlist;
	_truncate_history();
}

void Player::seek(int s) {
	_player->seek(s*1000);
	if (s >= _track.metadata().length()) {
		next();
	}
}

void Player::play() {
	if (_playlist.tracks().isEmpty())
		return;
	if (_track.source().isEmpty()) {
		next();
		return;
	}
	_state = PLAYER_PLAYING;
	emit stateChanged(_state);
	_player->play();
}

void Player::enqueue(int id) {
	_queue.push_back(_playlist.tracks().at(id));
}

void Player::toggleRandom() {
	_random = !_random;
	_config.setValue("playback/random", _random);
}

void Player::toggleRepeat() {
	if (_repeat == REPEAT_NO) {
		_repeat = REPEAT_ALL;
	} else if (_repeat == REPEAT_ALL) {
		_repeat = REPEAT_ONE;
	} else if (_repeat == REPEAT_ONE) {
		_repeat = REPEAT_NO;
	}
	_config.setValue("playback/repeat", _repeat);
}

void Player::equalizerValue(int band, double *val) {
	if (_equalizer == NULL) {
		*val = 0;
		return;
	}
	if (band < 0 || band > 9) {
		*val = -24;
		return;
	}
	if (_equalizer_enabled) {
		QList<Phonon::EffectParameter> plist = _equalizer->parameters();
		QVariant var = _equalizer->parameterValue(plist[band]);
		*val = var.toDouble();
	}
}

void Player::enableEqualizer() {
	if (_equalizer == NULL)
		return;
	_equalizer_enabled = true;
	_path.insertEffect(_equalizer);
	Config config;
	config.setEqualizerEnabled(true);
}

void Player::disableEqualizer() {
	if (_equalizer == NULL)
		return;
	_equalizer_enabled = false;
	_path.removeEffect(_equalizer);
	Config config;
	config.setEqualizerEnabled(false);
}

void Player::setEqualizerValue(int band, double value) {
	if (_equalizer == NULL)
		return;
	if (band < 0 || band > 9 || value < -24 || value > 12) {
		return;
	}
	QList<Phonon::EffectParameter> plist = _equalizer->parameters();
	_equalizer->setParameterValue(plist[band], QVariant::fromValue(value));
	Config config;
	config.setEqualizerValue(QString("band%1").arg(band), value);
}

QString Player::artist() {
	return _track.metadata().artist();
}

QString Player::album() {
	return _track.metadata().album();
}

QString Player::title() {
	return _track.metadata().title();
}

Track Player::current() {
	return _track;
}

void Player::pause() {
	if (_state == PLAYER_PLAYING) {
		_player->pause();
		_state = PLAYER_PAUSED;
		emit stateChanged(_state);
	}
}

void Player::playIfPaused() {
	if (_state == PLAYER_PAUSED) {
		play();
	}
}

void Player::_to_history(Track t) {
	if (!t.source().isEmpty()) {
		_history.push_front(t);
	}
	_truncate_history();
	foreach (Track t, _history) {
	}
}

void Player::_truncate_history() {
	while (_history.size() > 50 || _history.size() > _playlist.tracks().size()/2) {
		_history.removeLast();
	}
}
