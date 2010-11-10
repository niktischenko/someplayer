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

#include "timerdialog.h"
#include "ui_timerdialog.h"

TimerDialog::TimerDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TimerDialog)
{
    ui->setupUi(this);
    ui->disableCheckBox->setChecked(false);
    ui->disableCheckBox->setVisible(false);
}

TimerDialog::~TimerDialog()
{
    delete ui;
}

void TimerDialog::init() {
	setTime(0, 0, 0);
}

void TimerDialog::setTime(int h, int m, int s) {
	if (h < 0 || h > 12 || m < 0 || m > 59 || s < 0 || s > 59)
		return;
	ui->hoursListWidget->setCurrentRow(h);
	ui->hoursListWidget->scrollTo(ui->hoursListWidget->model()->index(h, 0));
	ui->minutesListWidget->setCurrentRow(m);
	ui->minutesListWidget->scrollTo(ui->minutesListWidget->model()->index(m, 0));
	ui->secondsListWidget->setCurrentRow(s);
	ui->secondsListWidget->scrollTo(ui->secondsListWidget->model()->index(s, 0));
}

void TimerDialog::getTime(int *h, int *m, int *s) {
	(*h) = ui->hoursListWidget->currentRow();
	(*m) = ui->minutesListWidget->currentRow();
	(*s) = ui->secondsListWidget->currentRow();
}

bool TimerDialog::timerDisabled() {
	return ui->disableCheckBox->isChecked();
}

void TimerDialog::showDisable() {
	ui->disableCheckBox->setVisible(true);
}

void TimerDialog::updateTranslations() {
	ui->retranslateUi(this);
}
