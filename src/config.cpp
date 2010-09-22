#include "config.h"
#include <QString>
#include <QDir>

using namespace SomePlayer::Storage;

Config::Config()
{
	_settings = new QSettings(QString(applicationDir())+"/settings.ini", QSettings::IniFormat);
}

Config::~Config() {
	delete _settings;
}

QString Config::applicationDir() {
	QString path = QDir::homePath()+"/.someplayer";
	QDir appdir(path);
	if (!appdir.exists(path)) {
		appdir.mkdir(path);
	}
	return path;
}

QVariant Config::getValue(QString key) {
	return _settings->value(key);
}

void Config::setValue(QString key, QVariant value) {
	_settings->setValue(key, value);
}
