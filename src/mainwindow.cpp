#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QInputDialog>
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
	connect(ui->actionAbout_Qt, SIGNAL(triggered()), this, SLOT(aboutQt()));
	connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
	setAnimated(true);
	_player_form = new PlayerForm(_library, ui->stackedWidget);
	_library_form = new LibraryForm(_library, ui->stackedWidget);
	_busy_widget = new BusyWidget(ui->stackedWidget);
	ui->stackedWidget->insertWidget(0, _player_form);
	ui->stackedWidget->insertWidget(1, _library_form);
	ui->stackedWidget->insertWidget(2, _busy_widget);
	QAction *add_directory = ui->menuLibrary->addAction("Add directory");
	QAction *save_playlist = ui->menuLibrary->addAction("Save playlist");
	QAction *clear_playlist = ui->menuLibrary->addAction("Clear current playlist");
	connect(_player_form, SIGNAL(library()), this, SLOT(library()));
	connect(_library_form, SIGNAL(player()), this, SLOT(player()));
	connect(add_directory, SIGNAL(triggered()), this, SLOT(_add_directory()));
	connect(save_playlist, SIGNAL(triggered()), this, SLOT(_save_playlist()));
	connect(clear_playlist, SIGNAL(triggered()), this, SLOT(_clear_current_playlist()));
	connect(_library, SIGNAL(done()), this, SLOT(library()));
	connect(_library_form, SIGNAL(done()), this, SLOT(library()));
	connect(_library_form, SIGNAL(busy(QString)), this, SLOT(showBusyWidget(QString)));
	connect(ui->searchButton, SIGNAL(clicked()), this, SLOT(_toggle_search_line()));
	connect(_player_form, SIGNAL(showSearchPanel()), this, SLOT(showSearchPanel()));
	connect(_player_form, SIGNAL(hideSearchPanel()), this, SLOT(hideSearchPanel()));
	connect(ui->searchLine, SIGNAL(textChanged(QString)), this, SLOT(_search(QString)));
	connect(ui->nextButton, SIGNAL(clicked()), this, SLOT(_nextItem()));
	connect(ui->prevButton, SIGNAL(clicked()), this, SLOT(_prevItem()));
	hideSearchPanel();
	library();
}

MainWindow::~MainWindow()
{
	delete _player_form;
	delete _library_form;
	delete ui;
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
	showSearchPanel();
	setWindowTitle("SomePlayer Library");
}

void MainWindow::_add_directory() {
	QString directory = QFileDialog::getExistingDirectory (this, "Select directory", "/home/user/MyDocs", QFileDialog::ShowDirsOnly );
	if (!directory.isEmpty()) {
		showBusyWidget("<H1>Scanning... Please wait</H1>");
		_library->addDirectory(directory);
	}
}

void MainWindow::_save_playlist() {
	QString name = QInputDialog::getText(this, "Playlist name", "Name:");
	Playlist playlist = _library->getCurrentPlaylist();
	playlist.setName(name);
	_library->savePlaylist(playlist);
}

void MainWindow::_clear_current_playlist() {
	Playlist playlist = _library->getCurrentPlaylist();
	playlist.clear();
	_library->saveCurrentPlaylist(playlist);
	_player_form->reload();
}

void MainWindow::showBusyWidget(QString caption) {
	_busy_widget->setText(caption);
	ui->menuBar->setEnabled(false);
	ui->stackedWidget->setCurrentIndex(2);
}

void MainWindow::_toggle_search_line() {
	if (ui->searchLine->isVisible()) {
		ui->searchLine->setText("");
		ui->searchLine->hide();
		ui->nextButton->hide();
		ui->prevButton->hide();
		_cancelSearch();
	} else {
		ui->searchLine->show();
		ui->nextButton->show();
		ui->prevButton->show();
	}
}

void MainWindow::showSearchPanel() {
	ui->searchButton->show();
}

void MainWindow::hideSearchPanel() {
	ui->searchLine->setText("");
	ui->searchLine->hide();
	ui->nextButton->hide();
	ui->prevButton->hide();
	ui->searchButton->hide();
	_cancelSearch();
}

void MainWindow::_search(QString pattern) {
	if (ui->stackedWidget->currentIndex() == 0) { // player
		_player_form->search(pattern);
	}
}

void MainWindow::_nextItem() {
	if (ui->stackedWidget->currentIndex() == 0) { // player
		_player_form->nextItem();
	}
}

void MainWindow::_prevItem() {
	if (ui->stackedWidget->currentIndex() == 0) { // player
		_player_form->prevItem();
	}
}

void MainWindow::_cancelSearch() {
	if (ui->stackedWidget->currentIndex() == 0) { // player
		_player_form->cancelSearch();
	}
}
