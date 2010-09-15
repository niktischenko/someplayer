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
	_player_form = new PlayerForm(_library, ui->stackedWidget);
	_library_form = new LibraryForm(_library, ui->stackedWidget);
	_busy_widget = new BusyWidget(ui->stackedWidget);
	ui->stackedWidget->insertWidget(0, _player_form);
	ui->stackedWidget->insertWidget(1, _library_form);
	ui->stackedWidget->insertWidget(2, _busy_widget);
	_library_menu = new QMenu("Lirary");
	QAction *add_directory = _library_menu->addAction("Add directory");
	_player_menu = new QMenu("Player");
	connect(_player_form, SIGNAL(library()), this, SLOT(library()));
	connect(_library_form, SIGNAL(player()), this, SLOT(player()));
	connect(add_directory, SIGNAL(triggered()), this, SLOT(_add_directory()));
	connect(_library, SIGNAL(addingDone()), this, SLOT(library()));
	library();
}

MainWindow::~MainWindow()
{
	delete _player_form;
	delete _library_form;
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
	_player_form->reload();
	setWindowTitle("SomePlayer");
}

void MainWindow::library() {
	ui->menuBar->setEnabled(true);
	ui->stackedWidget->setCurrentIndex(1);
	setWindowTitle("SomePlayer Library");
	ui->menuBar->addMenu(_library_menu);
}

void MainWindow::_add_directory() {
	QString directory = QFileDialog::getExistingDirectory (this, "Select directory", "/home/user/MyDocs", QFileDialog::ShowDirsOnly );
	if (!directory.isEmpty()) {
		_busy_widget->setText("<H1>Scanning... Please wait</H1>");
		ui->menuBar->setEnabled(false);
		ui->stackedWidget->setCurrentIndex(2);
		_library->addDirectory(directory);
	}
}
