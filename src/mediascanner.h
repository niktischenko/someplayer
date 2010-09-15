#ifndef MEDIASCANNER_H
#define MEDIASCANNER_H

#include "someplayer.h"
#include "track.h"
#include <QThread>
#include <QDir>
#include <QDirIterator>

namespace SomePlayer {
	namespace Storage {

		class MediaScanner : public QThread
		{

			Q_OBJECT
		public:
			explicit MediaScanner(QObject *parent = 0);
			void run();

		signals:
			void scanFinish(QStringList);
		public slots:
			void stop();
			void init(QString);
		private:
			QDir _dir;
			bool _stopped;
			bool _initialized;
			QStringList REGISTERED_FILE_EXTENSIONS;
			QStringList _foundMedia;

			void _scan_directory(QDir);
		};

	};
};

#endif // MEDIASCANNER_H
