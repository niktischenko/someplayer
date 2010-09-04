#ifndef TRACK_H
#define TRACK_H

#include "someplayer.h"
#include "trackmetainformation.h"
#include <QUrl>

// represents some track: metainformation + source url

namespace SomePlayer {
	namespace DataObjects {

		class Track
		{
		public:
			Track();
			Track(TrackMetadata metadata, QUrl source);
			TrackMetadata metadata(); //read-write
			QUrl source() const;
			void setSource (QUrl source);

		private:
			TrackMetadata _metadata;
			QUrl _source;
		};

	};
};


#endif // TRACK_H
