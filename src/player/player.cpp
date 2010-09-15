#include "player.h"
#include <phonon/MediaSource>

using namespace SomePlayer::Playback;
using namespace SomePlayer::DataObjects;

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
	srand(seed);
	_random = false;
	_repeat = false;
}

void Player::setTrackId(int id) {
	_current = id;
	_history.push(_current);
	_set_source();
	_state = PLAYER_LOADING;
	emit stateChanged(_state);
}

void Player::play() {
	_player->play();
	_state = PLAYER_PLAYING;
	emit stateChanged(_state);
}

void Player::stop() {
	_player->stop();
	_state = PLAYER_STOPPED;
	emit stateChanged(_state);
}

void Player::pause() {
	_player->pause();
	_state = PLAYER_PAUSED;
	emit stateChanged(_state);
}

void Player::next() {
	_history.push(_current);
	if (_random) {
		_current = rand() % _playlist.tracks().count();
	} else {
		_current = (_current + 1) % _playlist.tracks().count();
	}
	if (_history.count()-1 == _playlist.tracks().count() && !_repeat) {
		_history.clear();
		stop();
	} else {
		_set_source();
		play();
	}
}

void Player::_set_source() {
	qDebug() << "id: " << _current << " all: " << _playlist.tracks().count();
	Track track = _playlist.tracks().at(_current);
	_player->setCurrentSource(Phonon::MediaSource(track.source()));
	emit trackChanged(track);
}

void Player::prev() {
	if (_history.count() > 0)
		_current = _history.pop();
	_set_source();
	play();
}

void Player::_stateChanged(Phonon::State newState, Phonon::State oldState) {
	qDebug() << "state changed: " << oldState << "->" << newState;
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
	emit tick(ticks/1000, _playlist.tracks().at(_current).metadata().length());
}

void Player::setPlaylist(Playlist playlist) {
	_playlist = playlist;
	_history.clear();
}

void Player::seek(int s) {
	qDebug() << "seeking " << s;
	_player->seek(s*1000);
}
