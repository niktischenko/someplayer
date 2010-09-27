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

#ifndef EQUALIZERDIALOG_H
#define EQUALIZERDIALOG_H

#include <QDialog>

namespace Ui {
	class EqualizerDialog;
}

class EqualizerDialog : public QDialog
{
	Q_OBJECT
public:
	explicit EqualizerDialog(QWidget *parent = 0);
	~EqualizerDialog();
signals:
	void valueChanged(int, int);
	void equalizerDisabled();
	void equalizerEnabled();

public slots:
	void setValue(int, int);
	void setEqualizerEnabled(bool);

private slots:
	void _value0_changed(int v) { emit valueChanged(0, v);}
	void _value1_changed(int v) { emit valueChanged(1, v);}
	void _value2_changed(int v) { emit valueChanged(2, v);}
	void _value3_changed(int v) { emit valueChanged(3, v);}
	void _value4_changed(int v) { emit valueChanged(4, v);}
	void _value5_changed(int v) { emit valueChanged(5, v);}
	void _value6_changed(int v) { emit valueChanged(6, v);}
	void _value7_changed(int v) { emit valueChanged(7, v);}
	void _value8_changed(int v) { emit valueChanged(8, v);}
	void _value9_changed(int v) { emit valueChanged(9, v);}

	void _state_changed();
private:
	Ui::EqualizerDialog *ui;
};

#endif // EQUALIZERDIALOG_H
