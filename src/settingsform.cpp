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
#include <QFile>
#include "someplayer.h"
#include "fmtxsettingsdialog.h"

using namespace SomePlayer::Storage;

SettingsForm::SettingsForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::SettingsForm)
{
	ui->setupUi(this);
	Config config;

	_color_map << "blue" << "magenta" << "red" << "yellow" <<
			"white" << "black" << "light" << "dark";
	_lang_map << "en" << "ru" << "zh_CN" << "zh_TW";

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
	ui->langComboBox->setCurrentIndex(_lang_map.indexOf(language));
	ui->colorComboBox->setCurrentIndex(_color_map.indexOf(track_color));
	ui->pauseHPNoButton->setChecked(true);
	ui->hwKeysBox->setChecked(false);
	ui->hwkeysLabel->setEnabled(false);
	ui->hwTControlButton->setEnabled(false);
	ui->hwVolumeButton->setEnabled(false);
	ui->fmtxGroupBox->setChecked(false);
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
	if (config.getValue("hw/hpautopause").toString() == "yes") {
		ui->pauseHPYesButton->setChecked(true);
	}
	if (config.getValue("hw/zoomkeys").toString() == "enabled") {
		ui->hwKeysBox->setChecked(true);
		ui->hwkeysLabel->setEnabled(true);
		ui->hwTControlButton->setEnabled(true);
		ui->hwVolumeButton->setEnabled(true);
	}
	QString behavior = config.getValue("hw/zoom_action").toString();
	if (behavior == "volume") {
		ui->hwVolumeButton->setChecked(true);
	} else if (behavior == "track") {
		ui->hwTControlButton->setChecked(true);
	} else {
		ui->hwVolumeButton->setChecked(true);
		config.setValue("hw/zoom_action", "volume");
	}
	if (config.getValue("fmtx/enabled").toString() == "yes") {
		ui->fmtxGroupBox->setChecked(true);
		emit fmtxSettingsChanged();
	}
	if (!QFile::exists(QString(_APPLICATION_PATH_)+"/someplayer_ru.qm")) {
		ui->langBox->hide();
	} // refactor this when more translations will be added
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
	connect (ui->pauseHPNoButton, SIGNAL(toggled(bool)), this, SLOT(_set_pause_hp_no(bool)));
	connect (ui->pauseHPYesButton, SIGNAL(toggled(bool)), this, SLOT(_set_pause_hp_yes(bool)));
	connect (ui->hwKeysBox, SIGNAL(toggled(bool)), this, SLOT(_toggle_hw_settings(bool)));
	connect (ui->hwVolumeButton, SIGNAL(toggled(bool)), this, SLOT(_set_hw_volume_control(bool)));
	connect (ui->hwTControlButton, SIGNAL(toggled(bool)), this, SLOT(_set_hw_track_control(bool)));
	connect (ui->uiButton, SIGNAL(clicked()), this, SLOT(_toggle_view_ui()));
	connect (ui->hwButton, SIGNAL(clicked()), this, SLOT(_toggle_view_hw()));
	connect (ui->libraryButton, SIGNAL(clicked()), this, SLOT(_toggle_view_lib()));
	connect (ui->fmtxSettingsButton, SIGNAL(clicked()), this, SLOT(_open_fmtx_settings()));
	connect (ui->fmtxGroupBox, SIGNAL(toggled(bool)), this, SLOT(_toggle_fmtx_settings(bool)));
	connect (ui->colorComboBox, SIGNAL(activated(int)), this, SLOT(_set_color(int)));
	connect (ui->langComboBox, SIGNAL(activated(int)), this, SLOT(_set_language(int)));
	_toggle_view_ui();
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

void SettingsForm::_set_color(int code) {
	Config config;
	config.setValue("ui/trackcolor", _color_map[code]);
	emit trackColorChanged();
}

void SettingsForm::_set_language(int code) {
	Config config;
	config.setValue("ui/language", _lang_map[code]);
	emit translationChanged();
}

void SettingsForm::_set_pause_hp_no(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("hw/hpautopause", "no");
}

void SettingsForm::_set_pause_hp_yes(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("hw/hpautopause", "yes");
}

void SettingsForm::_toggle_hw_settings(bool checked) {
	Config config;
	config.setValue("hw/zoomkeys", checked ? "enabled" : "disabled");
	ui->hwkeysLabel->setEnabled(checked);
	ui->hwTControlButton->setEnabled(checked);
	ui->hwVolumeButton->setEnabled(checked);
	emit hwZoomPolicyChanged();
}

void SettingsForm::_set_hw_track_control(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("hw/zoom_action", "track");
}

void SettingsForm::_set_hw_volume_control(bool checked) {
	if (!checked) return;
	Config config;
	config.setValue("hw/zoom_action", "volume");
}

void SettingsForm::_toggle_view_ui() {
	ui->hwButton->setChecked(false);
	ui->libraryButton->setChecked(false);
	ui->uiButton->setChecked(true);
	ui->stackedWidget->setCurrentIndex(0);
}

void SettingsForm::_toggle_view_lib() {
	ui->hwButton->setChecked(false);
	ui->uiButton->setChecked(false);
	ui->libraryButton->setChecked(true);
	ui->stackedWidget->setCurrentIndex(1);
}

void SettingsForm::_toggle_view_hw() {
	ui->uiButton->setChecked(false);
	ui->libraryButton->setChecked(false);
	ui->hwButton->setChecked(true);
	ui->stackedWidget->setCurrentIndex(2);
}

void SettingsForm::updateTranslations() {
	Config config;
	ui->retranslateUi(this);
	ui->langComboBox->setCurrentIndex(_lang_map.indexOf(config.getValue("ui/language").toString()));
	ui->colorComboBox->setCurrentIndex(_color_map.indexOf(config.getValue("ui/trackcolor").toString()));
}

void SettingsForm::landscapeMode() {
	ui->orientationGridLayout->removeWidget(ui->orientationAButton);
	ui->orientationGridLayout->removeWidget(ui->orientationLButton);
	ui->orientationGridLayout->removeWidget(ui->orientationPButton);
	ui->orientationGridLayout->addWidget(ui->orientationLButton);
	ui->orientationGridLayout->addWidget(ui->orientationAButton);
	ui->orientationGridLayout->addWidget(ui->orientationPButton);
}

void SettingsForm::portraitMode() {
	ui->orientationGridLayout->removeWidget(ui->orientationAButton);
	ui->orientationGridLayout->removeWidget(ui->orientationLButton);
	ui->orientationGridLayout->removeWidget(ui->orientationPButton);
	ui->orientationGridLayout->addWidget(ui->orientationLButton, 0, 0);
	ui->orientationGridLayout->addWidget(ui->orientationAButton, 0, 1);
	ui->orientationGridLayout->addWidget(ui->orientationPButton, 1, 0, 1, 2);
}

void SettingsForm::_open_fmtx_settings() {
	FmtxSettingsDialog dialog(this);
	dialog.exec();
	Config config;
	config.setValue("fmtx/station_name", dialog.stationName());
	config.setValue("fmtx/frequency", dialog.frequency());
	emit fmtxSettingsChanged();
}

void SettingsForm::_toggle_fmtx_settings(bool checked) {
	Config config;
	config.setValue("fmtx/enabled", checked ? "yes" : "no");
	emit fmtxSettingsChanged();
}
