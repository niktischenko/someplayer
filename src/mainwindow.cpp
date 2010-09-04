#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include "player.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openMedia()));
	player = new Player();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::openMedia()
{
	QString filename = QFileDialog::getOpenFileName(this, "Open media", "/home/user/");
	player->playSong(filename);

}
