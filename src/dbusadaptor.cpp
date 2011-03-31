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

#include "dbusadaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>
#include <QDebug>
#include "config.h"

/*
 * Implementation of adaptor class DBusAdaptop
 */

DBusAdaptop::DBusAdaptop(QObject *parent)
	: QDBusAbstractAdaptor(parent)
{
	// constructor
	setAutoRelaySignals(true);
	// handling signals from bluetooth headset
	_time = QTime::currentTime();
	if (!QDBusConnection::systemBus().connect(QString(), QString(),
						  "org.freedesktop.Hal.Device", "Condition", this, SLOT(processBTSignal(QString, QString)))) {
		qWarning() << "Can not connect to HAL";
	}
	if (!QDBusConnection::systemBus().connect(QString(), QString(), "org.bluez.AudioSink", "PropertyChanged", this, SLOT(processBTConnect(QString, QDBusVariant)))) {
		qWarning() << "Can not connect to HAL 2";
	}
	setAutoRelaySignals(true);
}

DBusAdaptop::~DBusAdaptop()
{
	// destructor
}

QString DBusAdaptop::album() {
	// handle method call ru.somebody.someplayer.album
	QString out0;
	QMetaObject::invokeMethod(parent(), "album", Q_RETURN_ARG(QString, out0));
	return out0;
}

QString DBusAdaptop::artist() {
	// handle method call ru.somebody.someplayer.artist
	QString out0;
	QMetaObject::invokeMethod(parent(), "artist", Q_RETURN_ARG(QString, out0));
	return out0;
}

QString DBusAdaptop::title_artist_album() {
	// handle method call ru.somebody.someplayer.title_artist_album
	QString album, title, artist;
	QMetaObject::invokeMethod(parent(), "artist", Q_RETURN_ARG(QString, artist));
	QMetaObject::invokeMethod(parent(), "title", Q_RETURN_ARG(QString, title));
	QMetaObject::invokeMethod(parent(), "album", Q_RETURN_ARG(QString, album));
	return QString("%1\n%2\n%3").arg(title).arg(artist).arg(album);
}

void DBusAdaptop::next() {
	// handle method call ru.somebody.someplayer.next
	QMetaObject::invokeMethod(parent(), "next");
}

void DBusAdaptop::prev() {
	// handle method call ru.somebody.someplayer.prev
	QMetaObject::invokeMethod(parent(), "prev");
}

void DBusAdaptop::stop() {
	// handle method call ru.somebody.someplayer.stop
	QMetaObject::invokeMethod(parent(), "stop");
}

QString DBusAdaptop::title() {
	// handle method call ru.somebody.someplayer.title
	QString out0;
	QMetaObject::invokeMethod(parent(), "title", Q_RETURN_ARG(QString, out0));
	return out0;
}

void DBusAdaptop::toggle() {
	// handle method call ru.somebody.someplayer.toggle
	QMetaObject::invokeMethod(parent(), "toggle");
}

QString DBusAdaptop::state() {
	// handle method call ru.somebody.someplayer.state
	QString out0;
	QMetaObject::invokeMethod(parent(), "stateText", Q_RETURN_ARG(QString, out0));
	return out0;
}

QString DBusAdaptop::albumart() {
	// handle method call ru.somebody.someplayer.albumart
	QString out0;
	QMetaObject::invokeMethod(parent(), "albumart", Q_RETURN_ARG(QString, out0));
	return out0;
}

void DBusAdaptop::processBTSignal(QString event, QString state) {
	QTime t = QTime::currentTime();
	long msec = _time.msecsTo(t);
	if (msec > _DBUS_ACTION_TIMEOUT_) {
		if (event == "ButtonPressed") {
			if (state == "next-song") {
				next();
			} else if (state == "previous-song") {
				prev();
			} else if (state == "play-cd" || state == "pause-cd") {
				toggle();
			} else if (state == "connection") {
				SomePlayer::Storage::Config config;
				if (config.getValue("hw/hpautopause").toString() != "yes") {
					return;
				}
				bool present = QDBusInterface ("org.freedesktop.Hal",
							       "/org/freedesktop/Hal/devices/platform_headphone",
							       "org.freedesktop.Hal.Device",
							       QDBusConnection::systemBus()).call ("GetProperty", "button.state.value").arguments().at(0).toBool();
				if (!present) {
					pause();
				} else {
					QTimer::singleShot(1000, this, SLOT(playIfPaused()));
				}

			}
		}
	}
	_time = t;
}

void DBusAdaptop::pause() {
	QMetaObject::invokeMethod(parent(), "pause");
}

void DBusAdaptop::playIfPaused() {
	QMetaObject::invokeMethod(parent(), "playIfPaused");
}

void DBusAdaptop::processBTConnect(QString stateName, QDBusVariant state) {
	SomePlayer::Storage::Config config;
	if (config.getValue("hw/hpautopause").toString() != "yes") {
		return;
	}
	if (stateName == "State") {
		if (state.variant().toString() == "disconnected") {
			pause();
		} else if (state.variant().toString() == "connected") {
			QTimer::singleShot(1000, this, SLOT(playIfPaused()));
		}
	}
}
