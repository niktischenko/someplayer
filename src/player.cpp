#include "player.h"

Player::Player() : QObject()
{
object = new Phonon::MediaObject(this);
	output = new Phonon::AudioOutput(Phonon::MusicCategory, this);
	Phonon::createPath(object, output);
}

Player::~Player()
{
}

void Player::playSong(QString filename)
{
	object->setCurrentSource(Phonon::MediaSource(filename));
	object->play();
}
