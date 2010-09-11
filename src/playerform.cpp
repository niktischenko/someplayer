#include "playerform.h"
#include "ui_playerform.h"

PlayerForm::PlayerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerForm)
{
    ui->setupUi(this);
	connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(_library()));
}

PlayerForm::~PlayerForm()
{
    delete ui;
}

void PlayerForm::_library() {
	emit library();
}
