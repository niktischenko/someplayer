#include "playerform.h"
#include "ui_playerform.h"

using namespace SomePlayer::DataObjects;

PlayerForm::PlayerForm(Library* lib, QWidget *parent) :
	_lib(lib),
    QWidget(parent),
    ui(new Ui::PlayerForm)
{
    ui->setupUi(this);
	connect(ui->libraryButton, SIGNAL(clicked()), this, SLOT(_library()));
}

PlayerForm::~PlayerForm()
{
    delete ui;
}

void PlayerForm::_library() {
	emit library();
}
