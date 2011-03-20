#include "playerfactory.h"
#include "player.h"

using namespace SomePlayer::Playback;
using namespace SomePlayer::Storage;


AbstractPlayer *SomePlayer::Playback::Factory::constructPlayer(QObject *parent) {
	AbstractPlayer *player = NULL;
	Config config;
	QString player_type = config.getValue("playback/player").toString();
	if (player_type == "inner") {
		player = new Player(parent);
	}
	return player;
}
