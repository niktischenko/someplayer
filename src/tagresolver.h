#ifndef TAGRESOLVER_H
#define TAGRESOLVER_H

#include <QObject>
#include "someplayer.h"
#include "track.h"
#include <phonon/MediaObject>
#include <phonon/MediaSource>

using SomePlayer::DataObjects::Track;

namespace SomePlayer {
	namespace DataObjects {

		class TagResolver : public QObject
		{

			Q_OBJECT
		public:
			explicit TagResolver(QObject *parent = 0);
		public slots:
			void decode (QStringList files);

		signals:
			void decoded(Track);
			void done();

		private slots:
			void metaStateChanged(Phonon::State newState, Phonon::State /*oldState*/);
		private:
			QStringList _files;
			Phonon::MediaObject *_metaObject;
			QList<Phonon::MediaSource> _sources;

		};
	};
};
#endif // TAGRESOLVER_H
