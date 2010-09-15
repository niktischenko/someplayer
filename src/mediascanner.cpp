#include "mediascanner.h"

using namespace SomePlayer::Storage;

#include <QMap>
#include <QDir>

MediaScanner::MediaScanner(QObject *parent) :
		QThread(parent), _stopped(false), _initialized(false)
{
	REGISTERED_FILE_EXTENSIONS << "mp3" << "flac" << "wma" << "acc";
}

void MediaScanner::run() {
	if (!_initialized)
		return;
	_foundMedia.clear();
	_scan_directory(_dir);
	emit scanFinish(_foundMedia);
	_stopped = true;
}

void MediaScanner::_scan_directory(QDir dir) {
	QFileInfoList items = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
	foreach (QFileInfo info, items) {
		if (info.isDir()) {
			QDir ndir(info.absoluteFilePath());
			_scan_directory(ndir);
		} else {
			QString suffix = info.suffix().toLower();
			if (REGISTERED_FILE_EXTENSIONS.contains(suffix)) {
				if (!_foundMedia.contains(info.absoluteFilePath()))
					_foundMedia << info.absoluteFilePath();
			}
		}
	}
}

void MediaScanner::stop() {
	_stopped = true;
	_initialized = false;
}

void MediaScanner::init(QString dir) {
	_stopped = false;
	_initialized = true;
	_dir = dir;
}
