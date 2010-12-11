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

#ifndef DBUSCLIENT_H
#define DBUSCLIENT_H

#include <QObject>
#include <QtDBus/QtDBus>
#include <QTimer>
#include <QTime>

#define MM_KEY_UP (73)
#define MM_KEY_DOWN (74)

class DBusClient : public QObject
{
	Q_OBJECT
public:
	explicit DBusClient(QObject *parent = 0);
signals:
	void displayStateChanged(bool);
	void zoomKeyPressed(quint32);
public slots:
	void enableKeys();
	void disableKeys();
	void setVolume(quint32 volume);
	quint32 getVolume();
private slots:
	void _unpause_keys();
	void _display_handler(QString);
	void _zoom_keys_handler(quint32, quint32);
private:
	QTimer _timer;
	QDBusMessage _unpause_keys_message;
	QDBusMessage _set_volume_message;
	QDBusMessage _get_volume_message;
};

#endif // DBUSCLIENT_H
