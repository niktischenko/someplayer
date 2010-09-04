#ifndef TRACK_METAINFORMATION
#define TRACK_METAINFORMATION

#include "someplayer.h"

// represents track metainformation only.
// now it contains TITLE, ARTIST, ALBUM, GENRE

namespace SomePlayer {
	namespace DataObjects {
		class TrackMetadata {

		public:
			TrackMetadata();
			TrackMetadata(QString title, QString artist, QString album);
			TrackMetadata(const TrackMetadata &metadata);

			QString title();
			QString artist();
			QString album();

			void setTitle(QString title);
			void setArtist(QString artist);
			void setAlbum(QString album);

		private:
			QMap<QString, QString> _metadata;
		};
	};
};

#endif
