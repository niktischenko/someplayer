#include "player.h"
#include <phonon/MediaSource>

using namespace SomePlayer::Playback;
using namespace SomePlayer::DataObjects;

Player::Player(QObject *parent) :
    QObject(parent)
{
	_player = NULL;
}

void Player::setTrack(Track &track) {
	_current_track = track;
	_create_player();
	emit stateChanged(PLAYER_LOADING);
}

void Player::play() {
	if (_player) {
		_player->play();
		emit stateChanged(PLAYER_PLAYING);
	}
}

void Player::stop() {
	if (_player) {
		_player->stop();
		emit stateChanged(PLAYER_STOPPED);
	}
}

void Player::pause() {
	if (_player) {
		_player->pause();
		emit stateChanged(PLAYER_PAUSED);
	}
}

void Player::_create_player() {
	if (_player) {
		disconnect(_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(_stateChanged(Phonon::State,Phonon::State)));
		delete _player;
	}
	_player = Phonon::createPlayer(Phonon::MusicCategory, Phonon::MediaSource(_current_track.source()));
	_player->setTickInterval(1000);
	connect(_player, SIGNAL(stateChanged(Phonon::State,Phonon::State)), this, SLOT(_stateChanged(Phonon::State,Phonon::State)));
	connect(_player, SIGNAL(tick(qint64)), this, SLOT(_tick(qint64)));
	emit stateChanged(PLAYER_STOPPED);
}

void Player::_stateChanged(Phonon::State newState, Phonon::State oldState) {
}

void Player::_tick(qint64 ticks) {
	emit tick(ticks/1000, _current_track.metadata().length());
}
