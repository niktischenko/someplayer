#ifndef SOMEPLAYER
#define SOMEPLAYER

// common settings, constants and so on

namespace SomePlayer {
	namespace DataObjects {
		class Track;
		class TrackMetadata;
		class TagResolver;
		class Playlist;
	};
	namespace Storage {
	};
	namespace Playback {
	};
};

// common includes

#include <QString>
#include <QStringList>
#include <QList>
#include <QMap>
#include <QUrl>

#define _DYNAMIC_PLAYLIST_MAX_COUNT_ 50

#endif
