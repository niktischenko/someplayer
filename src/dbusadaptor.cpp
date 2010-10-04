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

/*
 * Implementation of adaptor class DBusAdaptop
 */

DBusAdaptop::DBusAdaptop(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

DBusAdaptop::~DBusAdaptop()
{
    // destructor
}

QString DBusAdaptop::album()
{
    // handle method call ru.somebody.someplayer.album
    QString out0;
    QMetaObject::invokeMethod(parent(), "album", Q_RETURN_ARG(QString, out0));
    return out0;
}

QString DBusAdaptop::artist()
{
    // handle method call ru.somebody.someplayer.artist
    QString out0;
    QMetaObject::invokeMethod(parent(), "artist", Q_RETURN_ARG(QString, out0));
    return out0;
}

void DBusAdaptop::next()
{
    // handle method call ru.somebody.someplayer.next
    QMetaObject::invokeMethod(parent(), "next");
}

void DBusAdaptop::prev()
{
    // handle method call ru.somebody.someplayer.prev
    QMetaObject::invokeMethod(parent(), "prev");
}

void DBusAdaptop::stop()
{
    // handle method call ru.somebody.someplayer.stop
    QMetaObject::invokeMethod(parent(), "stop");
}

QString DBusAdaptop::title()
{
    // handle method call ru.somebody.someplayer.title
    QString out0;
    QMetaObject::invokeMethod(parent(), "title", Q_RETURN_ARG(QString, out0));
    return out0;
}

void DBusAdaptop::toggle()
{
    // handle method call ru.somebody.someplayer.toggle
    QMetaObject::invokeMethod(parent(), "toggle");
}

