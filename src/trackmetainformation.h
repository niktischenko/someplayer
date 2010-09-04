#ifndef TRACK_METAINFORMATION
#define TRACK_METAINFORMATION

#include <QString>
#include <QMap>
#include "someplayer.h"

// represents track metainformation only.
// now it contains TITLE, ARTIST, ALBUM, GENRE

namespace SomePlayer {
	namespace DataObjects {
		class TrackMetadata {

		public:
			TrackMetadata();
			TrackMetadata(QString title, QString artist, QString album, QString genre);

			QString title();
			QString artist();
			QString album();
			QString genre();

			void setTitle(QString title);
			void setArtist(QString artist);
			void setAlbum(QString album);
			void setGenre(QString genre);

		private:
			QMap<QString, QString> _metadata;
		};
	};
};

#endif
