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

#include "equalizerdialog.h"
#include "ui_equalizerdialog.h"
#include "config.h"

using namespace SomePlayer::Storage;

EqualizerDialog::EqualizerDialog(QWidget *parent) :
		QDialog(parent),
		ui(new Ui::EqualizerDialog)
{
	ui->setupUi(this);
	ui->band0Slider->setMinimum(-120);
	ui->band0Slider->setValue(0);
	ui->band1Slider->setMinimum(-120);
	ui->band1Slider->setValue(0);
	ui->band2Slider->setMinimum(-120);
	ui->band2Slider->setValue(0);
	ui->band3Slider->setMinimum(-120);
	ui->band3Slider->setValue(0);
	ui->band4Slider->setMinimum(-120);
	ui->band4Slider->setValue(0);
	ui->band5Slider->setMinimum(-120);
	ui->band5Slider->setValue(0);
	ui->band6Slider->setMinimum(-120);
	ui->band6Slider->setValue(0);
	ui->band7Slider->setMinimum(-120);
	ui->band7Slider->setValue(0);
	ui->band8Slider->setMinimum(-120);
	ui->band8Slider->setValue(0);
	ui->band9Slider->setMinimum(-120);
	ui->band9Slider->setValue(0);

	ui->band0Slider->setMaximum(120);
	ui->band1Slider->setMaximum(120);
	ui->band2Slider->setMaximum(120);
	ui->band3Slider->setMaximum(120);
	ui->band4Slider->setMaximum(120);
	ui->band5Slider->setMaximum(120);
	ui->band6Slider->setMaximum(120);
	ui->band7Slider->setMaximum(120);
	ui->band8Slider->setMaximum(120);
	ui->band9Slider->setMaximum(120);

	ui->masterSlider->setMinimum(-120);
	ui->masterSlider->setMaximum(120);
	ui->masterSlider->setValue(0);

	connect(ui->band0Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value0_changed(int)));
	connect(ui->band1Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value1_changed(int)));
	connect(ui->band2Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value2_changed(int)));
	connect(ui->band3Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value3_changed(int)));
	connect(ui->band4Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value4_changed(int)));
	connect(ui->band5Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value5_changed(int)));
	connect(ui->band6Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value6_changed(int)));
	connect(ui->band7Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value7_changed(int)));
	connect(ui->band8Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value8_changed(int)));
	connect(ui->band9Slider, SIGNAL(sliderMoved(int)), this, SLOT(_value9_changed(int)));
	connect(ui->masterSlider, SIGNAL(valueChanged(int)), this, SLOT(_value_master_changed(int)));
	connect(ui->enableToggleButton, SIGNAL(clicked()), this, SLOT(_state_changed()));
	connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(_save_preset()));
	connect(ui->presetComboBox, SIGNAL(activated(QString)), this, SLOT(_load_preset(QString)));

	Config config;
	if (config.equalizerEnabled()) {
		setEqualizerEnabled(true);
		_load_preset("");
	}
}

EqualizerDialog::~EqualizerDialog()
{
	delete ui;
}

void EqualizerDialog::setValue(int band, int val) {
	switch (band) {
	case 0:
		ui->band0Slider->setValue(val);	break;
	case 1:
		ui->band1Slider->setValue(val);	break;
	case 2:
		ui->band2Slider->setValue(val);	break;
	case 3:
		ui->band3Slider->setValue(val);	break;
	case 4:
		ui->band4Slider->setValue(val);	break;
	case 5:
		ui->band5Slider->setValue(val);	break;
	case 6:
		ui->band6Slider->setValue(val);	break;
	case 7:
		ui->band7Slider->setValue(val);	break;
	case 8:
		ui->band8Slider->setValue(val);	break;
	case 9:
		ui->band9Slider->setValue(val);	break;
	}

}

void EqualizerDialog::_state_changed() {
	if (ui->enableToggleButton->isChecked()) {
		setEqualizerEnabled(true);
		emit equalizerEnabled();
	} else {
		setEqualizerEnabled(false);
		emit equalizerDisabled();
	}
}

void EqualizerDialog::setEqualizerEnabled(bool enabled) {
	ui->band0Slider->setEnabled(enabled);
	ui->band1Slider->setEnabled(enabled);
	ui->band2Slider->setEnabled(enabled);
	ui->band3Slider->setEnabled(enabled);
	ui->band4Slider->setEnabled(enabled);
	ui->band5Slider->setEnabled(enabled);
	ui->band6Slider->setEnabled(enabled);
	ui->band7Slider->setEnabled(enabled);
	ui->band8Slider->setEnabled(enabled);
	ui->band9Slider->setEnabled(enabled);
	ui->masterSlider->setEnabled(enabled);
	ui->saveButton->setEnabled(enabled);
	ui->presetComboBox->setEnabled(enabled);
	ui->enableToggleButton->setChecked(enabled);
}

void EqualizerDialog::_value_master_changed(int v) {
	ui->band0Slider->setValue(v);
	ui->band1Slider->setValue(v);
	ui->band2Slider->setValue(v);
	ui->band3Slider->setValue(v);
	ui->band4Slider->setValue(v);
	ui->band5Slider->setValue(v);
	ui->band6Slider->setValue(v);
	ui->band7Slider->setValue(v);
	ui->band8Slider->setValue(v);
	ui->band9Slider->setValue(v);
	for (int i = 0; i < 10; i++) {
		emit valueChanged(i, v);
	}
}

void EqualizerDialog::reloadPresets() {
	Config config;
	QStringList presets = config.getEqualizerPresets();
	ui->presetComboBox->clear();
	ui->presetComboBox->addItems(presets);
}

void EqualizerDialog::_save_preset() {
	Config config;
	QString name = ui->presetComboBox->currentText().trimmed();
	QStringList presets = config.getEqualizerPresets();
	if (!presets.contains(name))
		presets.append(name);
	config.saveEqualizer(name);
	reloadPresets();
}

void EqualizerDialog::_load_preset(QString name) {
	Config config;
	ui->band0Slider->setValue((int)(config.getEqualizerValue("band0", name)*10 + 0.5));
	_value0_changed(ui->band0Slider->value());
	ui->band1Slider->setValue((int)(config.getEqualizerValue("band1", name)*10 + 0.5));
	_value1_changed(ui->band1Slider->value());
	ui->band2Slider->setValue((int)(config.getEqualizerValue("band2", name)*10 + 0.5));
	_value2_changed(ui->band2Slider->value());
	ui->band3Slider->setValue((int)(config.getEqualizerValue("band3", name)*10 + 0.5));
	_value3_changed(ui->band3Slider->value());
	ui->band4Slider->setValue((int)(config.getEqualizerValue("band4", name)*10 + 0.5));
	_value4_changed(ui->band4Slider->value());
	ui->band5Slider->setValue((int)(config.getEqualizerValue("band5", name)*10 + 0.5));
	_value5_changed(ui->band5Slider->value());
	ui->band6Slider->setValue((int)(config.getEqualizerValue("band6", name)*10 + 0.5));
	_value6_changed(ui->band6Slider->value());
	ui->band7Slider->setValue((int)(config.getEqualizerValue("band7", name)*10 + 0.5));
	_value7_changed(ui->band7Slider->value());
	ui->band8Slider->setValue((int)(config.getEqualizerValue("band8", name)*10 + 0.5));
	_value8_changed(ui->band8Slider->value());
	ui->band9Slider->setValue((int)(config.getEqualizerValue("band9", name)*10 + 0.5));
	_value9_changed(ui->band9Slider->value());
}

void EqualizerDialog::landscapeMode() {
	ui->thorizontalLayout->insertWidget(0, ui->enableToggleButton);
	ui->thorizontalLayout->addWidget(ui->saveButton);
	ui->bhorizontalLayout->removeItem(ui->chorizontalSpacer);
}

void EqualizerDialog::portraitMode() {
	ui->bhorizontalLayout->insertWidget(0, ui->enableToggleButton);
	ui->bhorizontalLayout->addItem(ui->chorizontalSpacer);
	ui->bhorizontalLayout->addWidget(ui->saveButton);
}

void EqualizerDialog::updateTranslations() {
	ui->retranslateUi(this);
}
