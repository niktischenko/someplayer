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

#ifndef CONFIG_H
#define CONFIG_H

#include <QSettings>

namespace SomePlayer {
	namespace Storage {
		class Config
		{
		public:
		    Config();
		    ~Config();
		    QString applicationDir();
		    QVariant getValue(QString key);
		    void setValue(QString key, QVariant value);
		    QStringList getEqualizerPresets();
		    double getEqualizerValue(QString band, QString preset="");
		    void setEqualizerValue(QString band, double value);
		    void saveEqualizer(QString preset);
		    bool equalizerEnabled();
		    void setEqualizerEnabled(bool);
		private:
		    QSettings *_settings;
		    QSettings *_equalizer_settings;
		};
	};
};

#endif // CONFIG_H
