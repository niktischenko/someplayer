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
	connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui->actionPlayer, SIGNAL(triggered()), this, SLOT(player()));
	connect(ui->actionLibrary, SIGNAL(triggered()), this, SLOT(library()));
	setAnimated(true);
	_playerForm = new PlayerForm();
	_libraryForm = new LibraryForm();
	ui->stackedWidget->insertWidget(0, _playerForm);
	ui->stackedWidget->insertWidget(1, _libraryForm);
	_playerForm->setAttribute(Qt::WA_Maemo5StackedWindow);
	_libraryForm->setAttribute(Qt::WA_Maemo5StackedWindow);
	connect(_playerForm, SIGNAL(library()), this, SLOT(library()));
	library();
}

MainWindow::~MainWindow()
{
	delete _playerForm;
	delete _libraryForm;
	delete ui;
}

void MainWindow::openMedia()
{
//	SomePlayer::DataObjects::Library *l = new SomePlayer::DataObjects::Library("/tmp", "/tmp");
//	l->addDirectory("/mnt/music/Three Days Grace");
}

void MainWindow::aboutQt() {
	QMessageBox::aboutQt(this, "About Qt");
}

void MainWindow::about() {
	QMessageBox::about(this, "About SomePlayer", "Alternate music player for Maemo 5 "
					   "written in C++ with Qt4\n\n"
					   "Author: Nikolay Tischenko aka \"somebody\" <niktischenko@gmail.com>");
}

void MainWindow::player() {
	ui->stackedWidget->setCurrentIndex(0);
	setWindowTitle("SomePlayer");
}

void MainWindow::library() {
	ui->stackedWidget->setCurrentIndex(1);
	setWindowTitle("SomePlayer Library");
}
