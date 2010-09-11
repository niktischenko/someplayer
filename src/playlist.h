#ifndef PLAYLIST
#define PLAYLIST

#include "track.h"
#include "someplayer.h"

// represents playlist: list of tracks, name

namespace SomePlayer {
	namespace DataObjects {
		class Playlist {
		public:
			Playlist();
			Playlist(const Playlist &playlist);

			Playlist &operator=(const Playlist &playlist);

			QString name() const;
			const QList<Track> &tracks() const;

			void setName(QString name);
			void addTrack(Track track);

		private:
			QString _name;
			QList<Track> _tracks;
		};
	};
};

#endif
