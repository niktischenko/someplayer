#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

#include "player/player.h"

#include "library.h"

using namespace SomePlayer::DataObjects;

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	_library = new Library(_DATABASE_PATH_, _PLAYLISTS_PATH_);
	ui->setupUi(this);
	connect(ui->actionOpen, SIGNAL(triggered()), this, SLOT(openMedia()));
	connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	connect(ui->actionPlayer, SIGNAL(triggered()), this, SLOT(player()));
	connect(ui->actionLibrary, SIGNAL(triggered()), this, SLOT(library()));
	setAnimated(true);
	_playerForm = new PlayerForm(_library, ui->stackedWidget);
	_libraryForm = new LibraryForm(_library, ui->stackedWidget);
	ui->stackedWidget->insertWidget(0, _playerForm);
	ui->stackedWidget->insertWidget(1, _libraryForm);
	connect(_playerForm, SIGNAL(library()), this, SLOT(library()));
	connect(_libraryForm, SIGNAL(player()), this, SLOT(player()));
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
	_playerForm->show();
	ui->stackedWidget->setCurrentIndex(0);
	setWindowTitle("SomePlayer");
}

void MainWindow::library() {
	_libraryForm->show();
	ui->stackedWidget->setCurrentIndex(1);
	setWindowTitle("SomePlayer Library");
}
