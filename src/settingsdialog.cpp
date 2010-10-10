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
	QString albumSorting = config.getValue("ui/albumsorting").toString();
	QString showTrackLenght = config.getValue("ui/showtracklenght").toString();
	QString orientation = config.getValue("ui/orientation").toString();
	QString icons_theme = config.getValue("ui/iconstheme").toString();
	QString gradient = config.getValue("ui/gradient").toString();
	ui->albumsSortAButton->setChecked(true);	// defaule sorting
	ui->showTrackLenghtYButton->setChecked(true);	// show by default
	ui->orientationLButton->setChecked(true);
	ui->iconsWButton->setChecked(true);
	ui->gradientYButton->setChecked(true);
	if (albumSorting == "date") {
		ui->albumsSortDButton->setChecked(true);
	}
	if (showTrackLenght == "no") {
		ui->showTrackLenghtNButton->setChecked(true);
	}
	if (orientation == "portrait") {
		ui->orientationPButton->setChecked(true);
	} else if (orientation == "auto") {
		ui->orientationAButton->setChecked(true);
	}
	if (icons_theme == "black") {
		ui->iconstBButton->setChecked(true);
	}
	if (gradient == "no") {
		ui->gradientNButton->setChecked(true);
	}
	connect (ui->albumsSortAButton, SIGNAL(toggled(bool)), this, SLOT(_set_album_sorting_alphabet(bool)));
	connect (ui->albumsSortDButton, SIGNAL(toggled(bool)), this, SLOT(_set_album_sorting_date(bool)));
	connect (ui->showTrackLenghtNButton, SIGNAL(toggled(bool)), this, SLOT(_set_track_lenght_show_no(bool)));
	connect (ui->showTrackLenghtYButton, SIGNAL(toggled(bool)), this, SLOT(_set_track_lenght_show_yes(bool)));
	connect (ui->orientationAButton, SIGNAL(toggled(bool)), this, SLOT(_set_orientation_auto(bool)));
	connect (ui->orientationLButton, SIGNAL(toggled(bool)), this, SLOT(_set_orientation_landscape(bool)));
	connect (ui->orientationPButton, SIGNAL(toggled(bool)), this, SLOT(_set_orientation_portrait(bool)));
	connect (ui->iconstBButton, SIGNAL(toggled(bool)), this, SLOT(_set_icons_black(bool)));
	connect (ui->iconsWButton, SIGNAL(toggled(bool)), this, SLOT(_set_icons_white(bool)));
	connect (ui->gradientNButton, SIGNAL(toggled(bool)), this, SLOT(_set_gradient_no(bool)));
	connect (ui->gradientYButton, SIGNAL(toggled(bool)), this, SLOT(_set_gradient_yes(bool)));

	// disabled to 1.4.0
	ui->albumSortingGroupBox->setVisible(false);
}

SettingsDialog::~SettingsDialog()
{
	delete ui;
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

void SettingsDialog::_set_orientation_auto(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/orientation", "auto");
}

void SettingsDialog::_set_orientation_landscape(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/orientation", "landscape");
}

void SettingsDialog::_set_orientation_portrait(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/orientation", "portrait");
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

void SettingsDialog::_set_gradient_no(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/gradient", "no");
}

void SettingsDialog::_set_gradient_yes(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/gradient", "yes");
}
