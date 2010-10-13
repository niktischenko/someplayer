/*
 * SomePlayer - An alternate music player for Maemo 5
 * Copyright (C) 2010 Nikolay (somebody) Tischenko <niktischenko@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "config.h"
#include <QString>
#include <QDir>

using namespace SomePlayer::Storage;

Config::Config()
{
	_settings = new QSettings(QString(applicationDir())+"/settings.ini", QSettings::IniFormat);
	if (_settings->value("ui/iconstheme").toString() == "")
		_settings->setValue("ui/iconstheme", "white");
	if (_settings->value("ui/albumsorting").toString() == "")
		_settings->setValue("ui/albumsorting", "date");
	if (_settings->value("ui/gradient").toString() == "")
		_settings->setValue("ui/gradient", "yes");
	if (_settings->value("playback/volume").isNull()) {
		_settings->setValue("playback/volume", 100);
	}
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
