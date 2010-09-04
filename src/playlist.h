#ifndef PLAYLIST
#define PLAYLIST

#include <QString>
#include <QList>
#include "track.h"
#include "someplayer.h"

// represents playlist: list of tracks, name

namespace SomePlayer {
	namespace DataObjects {
		class Playlist {
		public:
			Playlist();

			QString name() const;
			QList<Track> tracks() const;

			void setName(QString name);
			void addTrack(Track track);

		private:
			QString _name;
			QList<Track> _tracks;
		};
	};
};

#endif
