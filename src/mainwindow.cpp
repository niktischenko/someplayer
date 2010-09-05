#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "player.h"

#include "library.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openMedia()));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openMedia()
{
//	SomePlayer::DataObjects::Library *l = new SomePlayer::DataObjects::Library("/tmp", "/tmp");
//	l->addDirectory("/mnt/music/Three Days Grace");
}
