#include "busywidget.h"
#include "ui_busywidget.h"

BusyWidget::BusyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BusyWidget)
{
    ui->setupUi(this);
}

BusyWidget::~BusyWidget()
{
    delete ui;
}

void BusyWidget::setText(QString text) {
	ui->label->setText(text);
}
