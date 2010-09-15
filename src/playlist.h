#ifndef PLAYLIST
#define PLAYLIST

#include "track.h"
#include "someplayer.h"

#define PLAYLIST_BAD_NAME "Bad playlist"
#define PLAYLIST_CURRENT_NAME "Current"

// represents playlist: list of tracks, name

namespace SomePlayer {
	namespace DataObjects {
		class Playlist {
		public:
			Playlist();
			Playlist(const Playlist &playlist);

			QString name();
			QList<Track> tracks();

			void setName(QString name);
			void addTrack(Track track);
			void removeTrack(Track track);
			void removeTrackAt(int id);
			void clear();

		private:
			QString _name;
			QList<Track> _tracks;
		};
	};
};

#endif
