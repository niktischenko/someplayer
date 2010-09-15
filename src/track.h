#ifndef TRACK_H
#define TRACK_H

#include "someplayer.h"
#include "trackmetainformation.h"
#include "tagresolver.h"

// represents some track: metainformation + source url

namespace SomePlayer {
	namespace DataObjects {

		class Track : public QObject
		{
			Q_OBJECT

		public:
			Track();
			Track(const Track &track);
			Track(int id, TrackMetadata metadata, QString source);
			Track(QString source);
			~Track();
			Track &operator=(const Track &track);
			TrackMetadata metadata() const; //read-write
			QString source() const;
			int id() const;
			void setSource (QString source);
			int count() const; //count of plays
			void setCount(int count); //for restoring from database and counting from player
			bool operator == (const Track &track);

		private:
			TrackMetadata _metadata;
			QString _source;
			int _count;
			int _id;
			TagResolver *_resolver;
		private slots:
			void decoded(Track);
		};

	};
};


#endif // TRACK_H
