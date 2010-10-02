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


#include "settingsdialog.h"
#include "ui_settingsdialog.h"

using namespace SomePlayer::Storage;

SettingsDialog::SettingsDialog(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::SettingsDialog)
{
	ui->setupUi(this);
	Config config;
	QString iconsTheme = config.getValue("ui/iconstheme").toString();
	QString albumSorting = config.getValue("ui/albumsorting").toString();
	QString showTrackLenght = config.getValue("ui/showtracklenght").toString();
	QString portraitEnabled = config.getValue("ui/portraitmode").toString();
	ui->iconsWButton->setChecked(true);		// "white" by default
	ui->albumsSortAButton->setChecked(true);	// defaule sorting
	ui->showTrackLenghtYButton->setChecked(true);	// show by default
	ui->portraitEnableButton->setChecked(true);	// enabled by default
	if (iconsTheme == "black") {
		ui->iconsBButton->setChecked(true);
	}
	if (albumSorting == "date") {
		ui->albumsSortDButton->setChecked(true);
	}
	if (showTrackLenght == "no") {
		ui->showTrackLenghtNButton->setChecked(true);
	}
	if (portraitEnabled == "disabled") {
		ui->portraitDisableButton->setChecked(true);
	}
	connect (ui->iconsBButton, SIGNAL(toggled(bool)), this, SLOT(_set_icons_black(bool)));
	connect (ui->iconsWButton, SIGNAL(toggled(bool)), this, SLOT(_set_icons_white(bool)));
	connect (ui->albumsSortAButton, SIGNAL(toggled(bool)), this, SLOT(_set_album_sorting_alphabet(bool)));
	connect (ui->albumsSortDButton, SIGNAL(toggled(bool)), this, SLOT(_set_album_sorting_date(bool)));
	connect (ui->showTrackLenghtNButton, SIGNAL(toggled(bool)), this, SLOT(_set_track_lenght_show_no(bool)));
	connect (ui->showTrackLenghtYButton, SIGNAL(toggled(bool)), this, SLOT(_set_track_lenght_show_yes(bool)));
	connect (ui->portraitDisableButton, SIGNAL(toggled(bool)), this, SLOT(_set_portrait_disabled(bool)));
	connect (ui->portraitEnableButton, SIGNAL(toggled(bool)), this, SLOT(_set_portrait_enabled(bool)));

	// disabled to 1.4.0
	ui->albumSortingGroupBox->setVisible(false);
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
}

void SettingsDialog::_set_icons_black(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/iconstheme", "black");
}

void SettingsDialog::_set_icons_white(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/iconstheme", "white");
}

void SettingsDialog::_set_album_sorting_date(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/albumsorting", "date");
}

void SettingsDialog::_set_album_sorting_alphabet(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/albumsorting", "alphabet");
}

void SettingsDialog::_set_track_lenght_show_no(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/showtracklenght", "no");
}

void SettingsDialog::_set_track_lenght_show_yes(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/showtracklenght", "yes");
}

void SettingsDialog::_set_portrait_disabled(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/portraitmode", "disabled");
}

void SettingsDialog::_set_portrait_enabled(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/portraitmode", "enabled");
}
