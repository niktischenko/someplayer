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

#ifndef DBUSADAPTOR
#define DBUSADAPTOR

#include <QtCore/QObject>
#include <QtDBus/QtDBus>
#include <QTime>

#define _DBUS_ACTION_TIMEOUT_ (500)

class QByteArray;
template<class T> class QList;
template<class Key, class Value> class QMap;
class QString;
class QStringList;
class QVariant;

/*
 * Adaptor class for interface ru.somebody.someplayer
 */
class DBusAdaptop: public QDBusAbstractAdaptor
{
	Q_OBJECT
	Q_CLASSINFO("D-Bus Interface", "ru.somebody.someplayer")
	Q_CLASSINFO("D-Bus Introspection", ""
		    "  <interface name=\"ru.somebody.someplayer\">\n"
		    "    <method name=\"prev\">\n"
		    "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
		    "    </method>\n"
		    "    <method name=\"next\">\n"
		    "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
		    "    </method>\n"
		    "    <method name=\"toggle\">\n"
		    "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
		    "    </method>\n"
		    "    <method name=\"stop\">\n"
		    "      <annotation value=\"true\" name=\"org.freedesktop.DBus.Method.NoReply\"/>\n"
		    "    </method>\n"
		    "    <method name=\"artist\">\n"
		    "      <arg direction=\"out\" type=\"s\"/>\n"
		    "    </method>\n"
		    "    <method name=\"album\">\n"
		    "      <arg direction=\"out\" type=\"s\"/>\n"
		    "    </method>\n"
		    "    <method name=\"title\">\n"
		    "      <arg direction=\"out\" type=\"s\"/>\n"
		    "    </method>\n"
		    "    <method name=\"title_artist_album\">\n"
		    "      <arg direction=\"out\" type=\"s\"/>\n"
		    "    </method>\n"
		    "    <method name=\"state\">\n"
		    "      <arg direction=\"out\" type=\"s\"/>\n"
		    "    </method>\n"
		    "    <method name=\"albumart\">\n"
		    "      <arg direction=\"out\" type=\"s\"/>\n"
		    "    </method>\n"
		    "  </interface>\n"
		    "")
public:
	DBusAdaptop(QObject *parent);
	virtual ~DBusAdaptop();

public: // PROPERTIES
public Q_SLOTS: // METHODS
	QString album();
	QString artist();
	Q_NOREPLY void next();
	Q_NOREPLY void prev();
	Q_NOREPLY void stop();
	QString title();
	QString title_artist_album();
	QString state();
	QString albumart();
	Q_NOREPLY void toggle();
	Q_NOREPLY void pause();
	Q_NOREPLY void playIfPaused();

	void processBTSignal(QString, QString);
Q_SIGNALS: // SIGNALS
	void stateChanged();
	void albumArt(QString path);
private:
	QTime _time;
};

#endif
