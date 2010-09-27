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

using namespace SomePlayer::Playback;
using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Storage;

Player::Player(QObject *parent) :
    QObject(parent)
{
	_player = new Phonon::MediaObject(this);
	_output = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	_player->setTickInterval(1000);
	connect(_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(_stateChanged(Phonon::State,Phonon::State)));
	connect(_player, SIGNAL(tick(qint64)), this, SLOT(_tick(qint64)));
	Phonon::createPath(_player, _output);
	int seed = reinterpret_cast<int> (_player);
	qsrand(seed);
	_random = _config.getValue("playback/random").toBool();
	_repeat = _config.getValue("playback/repeat").toBool();
	_current = -1;
}

void Player::setTrackId(int id) {
	_current = id;
	if (!_history.isEmpty() && _history.top() != _current || _history.isEmpty()) {
		_history.push(_current);
	}
	_track = _playlist.tracks().at(_current);
	_set_source();
	_state = PLAYER_LOADING;
	emit stateChanged(_state);
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
	_history.push(_current % count);
	if (!_queue.isEmpty()) {
		_current = _queue.dequeue();
	} else if (!_prev_history.isEmpty()) {
		_current = _prev_history.pop();
	} else {
		if (_random) {
			_current = (count + (qrand()  + qrand() + qrand()) % count) % count;
		} else {
			_current = _current + 1;
		}
	}
	if (_random && _history.count() >= count && !_repeat ||
		!_repeat && _current >= count) {
		_history.clear();
		stop();
	} else {
		_current %= count;
		_track = _playlist.tracks().at(_current);
		_set_source();
		play();
	}
}

void Player::_set_source() {
	_player->setCurrentSource(Phonon::MediaSource(_track.source()));
	emit trackChanged(_track);
}

void Player::prev() {
	if (_history.count() > 0) {
		_prev_history.push(_current);
		_current = _history.pop();
		_track = _playlist.tracks().at(_current);
	}
	_set_source();
	play();
}

void Player::_stateChanged(Phonon::State newState, Phonon::State /*oldState*/) {
	switch (newState) {
	case Phonon::PlayingState:
		if (_state == PLAYER_LOADING) {
			_state = PLAYER_PLAYING;
			emit stateChanged(_state);
		}
		break;
	case Phonon::StoppedState:
		break;
	case Phonon::LoadingState:
		break;
	case Phonon::PausedState:
		if (_state == PLAYER_PLAYING) {
			next();
		} else if (_state == PLAYER_ERROR) {
			play();
		}
		break;
	case Phonon::BufferingState:
		break;
	case Phonon::ErrorState:
		_state = PLAYER_ERROR;
		qDebug() << _player->errorString();
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
	_history.clear();
	_prev_history.clear();
	_queue.clear();
}

void Player::seek(int s) {
	_player->seek(s*1000);
}

void Player::play() {
	if (_playlist.tracks().isEmpty())
		return;
	_state = PLAYER_PLAYING;
	emit stateChanged(_state);
	if (_current == -1) {
		_current = 0;
		_track = _playlist.tracks().at(0);
		_set_source();
	}
	_player->play();
}

void Player::enqueue(int id) {
	_queue.enqueue(id);
}

void Player::toggleRandom() {
	_random = !_random;
	_config.setValue("playback/random", _random);
}

void Player::toggleRepeat() {
	_repeat = !_repeat;
	_config.setValue("playback/repeat", _repeat);
}

void Player::setVolume(int v) {
	_output->setVolume(v*0.01);
}
