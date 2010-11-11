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


#include "settingsform.h"
#include "ui_settingsform.h"
#include <QDebug>

using namespace SomePlayer::Storage;

SettingsForm::SettingsForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::SettingsForm)
{
	ui->setupUi(this);
	Config config;
	QString albumSorting = config.getValue("ui/albumsorting").toString();
	QString showTrackLenght = config.getValue("ui/showtracklenght").toString();
	QString orientation = config.getValue("ui/orientation").toString();
	QString icons_theme = config.getValue("ui/iconstheme").toString();
	QString gradient = config.getValue("ui/gradient").toString();
	QString language = config.getValue("ui/language").toString();
	QString track_color = config.getValue("ui/trackcolor").toString();
	ui->albumsSortDButton->setChecked(true);	// defaule sorting
	ui->showTrackLenghtYButton->setChecked(true);	// show by default
	ui->orientationLButton->setChecked(true);
	ui->iconsWButton->setChecked(true);
	ui->gradientYButton->setChecked(true);
	ui->engLangButton->setChecked(true);
	ui->cBlueButton->setChecked(true);
	if (albumSorting == "alphabet") {
		ui->albumsSortAButton->setChecked(true);
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
	if (language == "ru") {
		ui->ruLangButton->setChecked(true);
	}
	if (track_color == "black") {
		ui->cBlackButton->setChecked(true);
	} else if (track_color == "magenta") {
		ui->cMagentaButton->setChecked(true);
	} else if (track_color == "Red") {
		ui->cRedButton->setChecked(true);
	} else if (track_color == "yellow") {
		ui->cYellowButton->setChecked(true);
	} else if (track_color == "white") {
		ui->cWhiteButton->setChecked(true);
	} else if (track_color == "dark") {
		ui->cDarkButton->setChecked(true);
	} else if (track_color == "light") {
		ui->cLightButton->setChecked(true);
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
	connect (ui->engLangButton, SIGNAL(toggled(bool)), this, SLOT(_set_lang_en(bool)));
	connect (ui->ruLangButton, SIGNAL(toggled(bool)), this, SLOT(_set_lang_ru(bool)));
	connect (ui->cBlackButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_black(bool)));
	connect (ui->cBlueButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_blue(bool)));
	connect (ui->cDarkButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_dark(bool)));
	connect (ui->cLightButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_light(bool)));
	connect (ui->cMagentaButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_magenta(bool)));
	connect (ui->cRedButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_red(bool)));
	connect (ui->cWhiteButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_white(bool)));
	connect (ui->cYellowButton, SIGNAL(toggled(bool)), this, SLOT(_set_color_yellow(bool)));
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(Qt::Window | windowFlags());
}

SettingsForm::~SettingsForm()
{
	delete ui;
}

void SettingsForm::_set_album_sorting_date(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/albumsorting", "date");
	emit libraryOptionsChanged();
}

void SettingsForm::_set_album_sorting_alphabet(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/albumsorting", "alphabet");
	emit libraryOptionsChanged();
}

void SettingsForm::_set_track_lenght_show_no(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/showtracklenght", "no");
	emit libraryOptionsChanged();
}

void SettingsForm::_set_track_lenght_show_yes(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/showtracklenght", "yes");
	emit libraryOptionsChanged();
}

void SettingsForm::_set_orientation_auto(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/orientation", "auto");
	emit orientationChanged();
}

void SettingsForm::_set_orientation_landscape(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/orientation", "landscape");
	emit orientationChanged();
}

void SettingsForm::_set_orientation_portrait(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/orientation", "portrait");
	emit orientationChanged();
}

void SettingsForm::_set_icons_black(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/iconstheme", "black");
	emit iconsChanged();
}

void SettingsForm::_set_icons_white(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/iconstheme", "white");
	emit iconsChanged();
}

void SettingsForm::_set_gradient_no(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/gradient", "no");
	emit gradientChanged();
}

void SettingsForm::_set_gradient_yes(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/gradient", "yes");
	emit gradientChanged();
}

void SettingsForm::_set_color_black(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "black");
	emit trackColorChanged();
}

void SettingsForm::_set_color_blue(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "blue");
	emit trackColorChanged();
}

void SettingsForm::_set_color_dark(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "dark");
	emit trackColorChanged();
}

void SettingsForm::_set_color_light(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "light");
	emit trackColorChanged();
}

void SettingsForm::_set_color_magenta(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "magenta");
	emit trackColorChanged();
}

void SettingsForm::_set_color_red(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "red");
	emit trackColorChanged();
}

void SettingsForm::_set_color_white(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "white");
	emit trackColorChanged();
}

void SettingsForm::_set_color_yellow(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/trackcolor", "yellow");
	emit trackColorChanged();
}

void SettingsForm::_set_lang_en(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/language", "en");
	emit translationChanged();
}

void SettingsForm::_set_lang_ru(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("ui/language", "ru");
	emit translationChanged();
}

void SettingsForm::updateTranslations() {
	ui->retranslateUi(this);
}

void SettingsForm::landscapeMode() {
	ui->cButtonsLayout->removeWidget(ui->cBlueButton);
	ui->cButtonsLayout->removeWidget(ui->cMagentaButton);
	ui->cButtonsLayout->removeWidget(ui->cRedButton);
	ui->cButtonsLayout->removeWidget(ui->cYellowButton);
	ui->cButtonsLayout->removeWidget(ui->cWhiteButton);
	ui->cButtonsLayout->removeWidget(ui->cBlackButton);
	ui->cButtonsLayout->removeWidget(ui->cLightButton);
	ui->cButtonsLayout->removeWidget(ui->cDarkButton);
	ui->cButtonsLayout->addWidget(ui->cBlueButton, 0, 0);
	ui->cButtonsLayout->addWidget(ui->cMagentaButton, 0, 1);
	ui->cButtonsLayout->addWidget(ui->cRedButton, 0, 2);
	ui->cButtonsLayout->addWidget(ui->cYellowButton, 0, 3);
	ui->cButtonsLayout->addWidget(ui->cWhiteButton, 1, 0);
	ui->cButtonsLayout->addWidget(ui->cBlackButton, 1, 1);
	ui->cButtonsLayout->addWidget(ui->cLightButton, 1, 2);
	ui->cButtonsLayout->addWidget(ui->cDarkButton, 1, 3);

	ui->orientationGridLayout->removeWidget(ui->orientationAButton);
	ui->orientationGridLayout->removeWidget(ui->orientationLButton);
	ui->orientationGridLayout->removeWidget(ui->orientationPButton);
	ui->orientationGridLayout->addWidget(ui->orientationLButton);
	ui->orientationGridLayout->addWidget(ui->orientationAButton);
	ui->orientationGridLayout->addWidget(ui->orientationPButton);
}

void SettingsForm::portraitMode() {
	ui->cButtonsLayout->removeWidget(ui->cBlueButton);
	ui->cButtonsLayout->removeWidget(ui->cMagentaButton);
	ui->cButtonsLayout->removeWidget(ui->cRedButton);
	ui->cButtonsLayout->removeWidget(ui->cYellowButton);
	ui->cButtonsLayout->removeWidget(ui->cWhiteButton);
	ui->cButtonsLayout->removeWidget(ui->cBlackButton);
	ui->cButtonsLayout->removeWidget(ui->cLightButton);
	ui->cButtonsLayout->removeWidget(ui->cDarkButton);
	ui->cButtonsLayout->addWidget(ui->cBlueButton, 0, 0);
	ui->cButtonsLayout->addWidget(ui->cMagentaButton, 0, 1);
	ui->cButtonsLayout->addWidget(ui->cRedButton, 1, 0);
	ui->cButtonsLayout->addWidget(ui->cYellowButton, 1, 1);
	ui->cButtonsLayout->addWidget(ui->cWhiteButton, 2, 0);
	ui->cButtonsLayout->addWidget(ui->cBlackButton, 2, 1);
	ui->cButtonsLayout->addWidget(ui->cLightButton, 3, 0);
	ui->cButtonsLayout->addWidget(ui->cDarkButton, 3, 1);

	ui->orientationGridLayout->removeWidget(ui->orientationAButton);
	ui->orientationGridLayout->removeWidget(ui->orientationLButton);
	ui->orientationGridLayout->removeWidget(ui->orientationPButton);
	ui->orientationGridLayout->addWidget(ui->orientationLButton, 0, 0);
	ui->orientationGridLayout->addWidget(ui->orientationAButton, 0, 1);
	ui->orientationGridLayout->addWidget(ui->orientationPButton, 1, 0, 1, 2);
}
