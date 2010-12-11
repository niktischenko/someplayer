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

#include "fmtxsettingsdialog.h"
#include "ui_fmtxsettingsdialog.h"
#include "someplayer.h"

using namespace SomePlayer::Storage;

FmtxSettingsDialog::FmtxSettingsDialog(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::FmtxSettingsDialog)
{
	ui->setupUi(this);
	Config config;
	QString station_name = config.getValue("fmtx/station_name").toString();
	if (station_name.isEmpty()) {
		station_name = "S.P.";
		config.setValue("fmtx/station_name", station_name);
	}
	int freq = config.getValue("fmtx/frequency").toInt();
	ui->stationNameLineEdit->setText(station_name);
	int freq_g = freq/1000;
	int freq_s = (freq % 1000)/10;
	freq_g = freq_g > 107 ? 107 : freq_g;
	freq_g = freq_g < 88 ? 88 : freq_g;
	ui->freqGWidget->scrollTo(ui->freqGWidget->model()->index(freq_g-88, 0));
	ui->freqGWidget->setCurrentRow(freq_g-88);
	ui->freqSWidget->scrollTo(ui->freqSWidget->model()->index(freq_s/5, 0));
	ui->freqSWidget->setCurrentRow(freq_s/5);
}

FmtxSettingsDialog::~FmtxSettingsDialog()
{
	delete ui;
}

QString FmtxSettingsDialog::stationName() {
	return ui->stationNameLineEdit->text();
}

int FmtxSettingsDialog::frequency() {
	return (88+ui->freqGWidget->currentRow())*1000+10*(ui->freqSWidget->currentRow()*5);
}
