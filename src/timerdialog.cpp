#include "timerdialog.h"
#include "ui_timerdialog.h"
#include <QDebug>

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
