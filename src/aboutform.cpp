#include "aboutform.h"
#include "ui_aboutform.h"
#include "config.h"
#include <QWebView>
#include <QDebug>
#include "someplayer.h"

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
	connect(ui->onlineHelpButton, SIGNAL(clicked()), this, SLOT(onlineHelp()));
	ui->donateButton->hide();
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
	ui->versionLabel->setText(QString("<b>%1</b>").arg(_SOMEPLAYER_VERSION_));
}

void AboutForm::onlineHelp() {
	Config config;
	QWebView *view = new QWebView(this);
	view->setAttribute(Qt::WA_Maemo5StackedWindow);
	view->setWindowFlags(Qt::Window | view->windowFlags());
	QString lang = config.getValue("ui/language").toString();
	QString icons_theme = config.getValue("ui/iconstheme").toString();
	QColor background = palette().color(QPalette::Active, QPalette::Background);
	QColor text = palette().color(QPalette::Active, QPalette::Text);
	QString paramBg = QString("%1:%2:%3").arg(background.red()).arg(background.blue()).arg(background.green());
	QString paramText = QString("%1:%2:%3").arg(text.red()).arg(text.blue()).arg(text.green());
	QString url = QString("http://someplayer.some-body.ru/help.php?bg=%1&text=%2&lang=%3&iconstheme=%4")
		      .arg(paramBg).arg(paramText).arg(lang).arg(icons_theme);
	view->load(QUrl(url));
	view->show();
}

void AboutForm::donate() {
}
