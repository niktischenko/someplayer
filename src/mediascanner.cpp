#include "mediascanner.h"

using namespace SomePlayer::Storage;

#include <QMap>

MediaScanner::MediaScanner(QObject *parent) :
		QThread(parent), _stopped(false), _initialized(false)
{
	REGISTERED_FILE_EXTENSIONS << "mp3" << "flac" << "wma" << "acc";
	_iterator = NULL;
}

void MediaScanner::run() {
	if (!_initialized)
		return;
	_foundMedia.clear();
	while(!_stopped && _iterator->hasNext()) {
		QString entry(_iterator->next());
		QFileInfo info(entry);
		if (info.isReadable()) {
			QString suffix = info.suffix().toLower();
			if (REGISTERED_FILE_EXTENSIONS.contains(suffix)) {
				if (!_foundMedia.contains(info.absoluteFilePath()))
					_foundMedia << info.absoluteFilePath();
			}
		}
	}
	emit scanFinish(_foundMedia);
	_stopped = true;
}

void MediaScanner::stop() {
	_stopped = true;
	_initialized = false;
}

void MediaScanner::init(QString dir) {
	_stopped = false;
	_initialized = true;
	if (!_iterator)
		delete _iterator;
	_iterator = new QDirIterator(QDir(dir), QDirIterator::Subdirectories | QDirIterator::FollowSymlinks);
}
