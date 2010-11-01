#ifndef PLAYERFACTORY_H
#define PLAYERFACTORY_H

#include <QObject>
#include "../someplayer.h"
#include "abstractplayer.h"

namespace SomePlayer {
	namespace Playback {
		namespace Factory {
			AbstractPlayer *constructPlayer(QObject *parent);
		};
	};
};


#endif // PLAYERFACTORY_H
