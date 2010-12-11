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

#include "dbusclient.h"
#include <QDebug>

DBusClient::DBusClient(QObject *parent) :
		QObject(parent)
{
	_timer.setInterval(60000);
	_timer.setSingleShot(false);
	//dbus-send  --system --type=method_call --dest=com.nokia.mce /com/nokia/mce/request com.nokia.mce.request.req_keypad_off_pause
	_unpause_keys_message = QDBusMessage::createMethodCall("com.nokia.mce",
							       "/com/nokia/mce/request",
							       "com.nokia.mce.request",
							       "req_keypad_off_pause");

	//signal sender=:1.13 -> dest=(null destination) serial=4373 path=/com/nokia/mce/signal; interface=com.nokia.mce.signal; member=display_status_ind
	//string "off"
	QDBusConnection::systemBus().connect(QString(), QString(),
					     "com.nokia.mce.signal", "display_status_ind", this, SLOT(_display_handler(QString)));
	//signal sender=:1.46 -> dest=(null destination) serial=2966 path=/com/nokia/tklock/signal; interface=com.nokia.tklock.signal; member=mm_key_press
	//uint32 74
	//uint32 65476
	QDBusConnection::systemBus().connect(QString(), QString(),
					     "com.nokia.tklock.signal", "mm_key_press", this, SLOT(_zoom_keys_handler(quint32,quint32)));

}

void DBusClient::enableKeys() {
	connect(&_timer, SIGNAL(timeout()), this, SLOT(_unpause_keys()));
	_unpause_keys();
	_timer.start();
}

void DBusClient::disableKeys() {
	disconnect(&_timer, SIGNAL(timeout()), this, SLOT(_unpause_keys()));
	_timer.stop();
}

void DBusClient::_unpause_keys() {
	QDBusConnection::systemBus().call(_unpause_keys_message);
}

void DBusClient::_display_handler(QString state) {
	emit displayStateChanged(state == "on");
}

void DBusClient::_zoom_keys_handler(quint32 code, quint32 /*ignored*/) {
	emit zoomKeyPressed(code);
}

void DBusClient::setVolume(quint32 volume) {
	QDBusMessage msg = QDBusMessage::createMethodCall("com.nokia.mafw.renderer.Mafw-Gst-Renderer-Plugin.gstrenderer",
							  "/com/nokia/mafw/renderer/gstrenderer",
							  "com.nokia.mafw.extension",
							  "set_extension_property");
	QList<QVariant> args;
	args << "volume" << QVariant::fromValue(QDBusVariant(volume));
	msg.setArguments(args);
	QDBusConnection::sessionBus().send(msg);
}

quint32 DBusClient::getVolume() {
	QDBusInterface iface("com.nokia.mafw.renderer.Mafw-Gst-Renderer-Plugin.gstrenderer",
			     "/com/nokia/mafw/renderer/gstrenderer",
			     "com.nokia.mafw.extension",
			     QDBusConnection::sessionBus());
	QDBusMessage reply = iface.call("get_extension_property", "volume");
	QList<QVariant> values = reply.arguments();
	QVariant volume = values.takeAt(1);
	QDBusVariant var = qvariant_cast<QDBusVariant>(volume);
	volume = var.variant();
	return volume.toUInt();
}
