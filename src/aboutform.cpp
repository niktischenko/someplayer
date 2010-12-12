#include "aboutform.h"
#include "ui_aboutform.h"
#include "config.h"

using namespace SomePlayer::Storage;

AboutForm::AboutForm(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::AboutForm)
{
	ui->setupUi(this);
	updateIcons();
	updateTranslations();
	setAttribute(Qt::WA_Maemo5StackedWindow);
	setWindowFlags(Qt::Window | windowFlags());
}

AboutForm::~AboutForm()
{
	delete ui;
}

void AboutForm::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	ui->logoLabel->setPixmap(QPixmap(":/icons/"+_icons_theme+"/player.png"));
}

void AboutForm::updateTranslations() {
	ui->retranslateUi(this);
}
