#include "playerform.h"
#include "ui_playerform.h"
#include "library.h"
#include <QDebug>
#include <QTime>
#include <QSlider>
#include "trackrenderer.h"

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Playback;

inline void __fill_list(QStandardItemModel *_model, Playlist playlist) {
	_model->clear();
	QList<Track> tracks = playlist.tracks();
	int count = tracks.count();
	_model->setRowCount(count);
	for (int i = 0; i < count; i++) {
		TrackMetadata meta = tracks.at(i).metadata();
		QString t = meta.title()+"#_#"+meta.artist()+"#_#"+meta.album();
		_model->setItem(i, 0, new QStandardItem(t));
	}
}

PlayerForm::PlayerForm(Library* lib, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlayerForm)
{
	_lib = lib;
	_player = new Player(this);
    ui->setupUi(this);
	connect(ui->libraryButton, SIGNAL(clicked()), this, SLOT(_library()));
	connect(ui->viewButton, SIGNAL(clicked()), this, SLOT(_toggle_view()));
	connect(ui->playlistView, SIGNAL(clicked(QModelIndex)), this, SLOT(_process_click(QModelIndex)));
	connect(ui->playpauseButton, SIGNAL(clicked()), _player, SLOT(toggle()));
	connect(ui->stopButton, SIGNAL(clicked()), _player, SLOT(stop()));
	connect(ui->nextButton, SIGNAL(clicked()), _player, SLOT(next()));
	connect(ui->prevButton, SIGNAL(clicked()), _player, SLOT(prev()));
	connect(_player, SIGNAL(trackChanged(Track)), this, SLOT(_track_changed(Track)));
	connect(_player, SIGNAL(tick(int,int)), this, SLOT(_tick(int,int)));
	connect(ui->randomButton, SIGNAL(clicked()), _player, SLOT(toggleRandom()));
	connect(ui->repeatButton, SIGNAL(clicked()), _player, SLOT(toggleRepeat()));
	ui->randomButton->setChecked(_player->random());
	ui->repeatButton->setChecked(_player->repeat());
	_seek_slider = new QSlider(Qt::Horizontal);
	ui->progressLayout->insertWidget(1, _seek_slider);
	_seek_slider->setTracking(false);
	connect(_seek_slider, SIGNAL(sliderReleased()), this, SLOT(_slider_released()));
	connect(ui->playlistView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(_custom_context_venu_requested(QPoint)));
	_model = new QStandardItemModel(0, 2, this);
	ui->playlistView->setModel(_model);
	_context_menu = new QMenu(ui->playlistView);
	QAction *delete_action = _context_menu->addAction("Delete");
	connect(delete_action, SIGNAL(triggered()), this, SLOT(_delete_track()));
	QAction *enqueue_action = _context_menu->addAction("Enqueue");
	connect(enqueue_action, SIGNAL(triggered()), this, SLOT(_enqueue_track()));
	QAction *add_to_favorites = _context_menu->addAction("Add to favorites");
	connect(add_to_favorites, SIGNAL(triggered()), this, SLOT(_add_to_favorites()));

	_track_renderer = new TrackRenderer(this);
	ui->playlistView->setItemDelegateForColumn(0, _track_renderer);
	connect(_player, SIGNAL(trackDone(Track)), _lib, SLOT(updateTrackCount(Track)));
}

PlayerForm::~PlayerForm()
{
    delete ui;
}

void PlayerForm::_library() {
	emit library();
}

void PlayerForm::reload() {
	_current_playlist = _lib->getCurrentPlaylist();
	_player->setPlaylist(_current_playlist);
	__fill_list(_model, _current_playlist);
}

void PlayerForm::_toggle_view() {
	int index = ui->stackedWidget->currentIndex();
	index = (!index % 2);
	ui->stackedWidget->setCurrentIndex(index);
}

void PlayerForm::_process_click(QModelIndex index) {
	int id = index.row();
	_player->stop();
	_player->setTrackId(id);
	_player->play();
	_track_renderer->setActiveRow(id);
	ui->playlistView->hide();
	ui->playlistView->show();
}

void PlayerForm::_track_changed(Track track) {
	int id = _current_playlist.tracks().indexOf(track);
	QModelIndex index = _model->index(id, 0);
	ui->playlistView->setCurrentIndex(index);
	ui->playlistView->scrollTo(index);
	_track_renderer->setActiveRow(id);
	ui->playlistView->hide();
	ui->playlistView->show();
	_display_track(track);
}

void PlayerForm::_display_track(Track track) {
	ui->countLabel->setText(QString("%1/%2").
							arg(_current_playlist.tracks().indexOf(track)+1).
							arg(_current_playlist.tracks().count()));
	ui->titleLabel->setText(QString("<h3>%1</h3>").arg(track.metadata().title()));
	ui->artistAlbumLabel->setText(QString("<h3>%1</h3><br/>%2").
								  arg(track.metadata().artist()).
								  arg(track.metadata().album()));
	_seek_slider->setMinimum(0);
	_seek_slider->setMaximum(track.metadata().length());
	_tick(0, track.metadata().length());
}

void PlayerForm::_tick(int done, int all) {
	QTime time(0, all/60, all%60);
	ui->allTimeLabel->setText(time.toString("mm:ss"));
	time.setHMS(0, done/60, done%60);
	ui->doneTimeLabel->setText(time.toString("mm:ss"));
	_seek_slider->setValue(done);
}

void PlayerForm::_slider_released() {
	_player->seek(_seek_slider->value());
}

void PlayerForm::_custom_context_venu_requested(const QPoint &pos) {
	_context_menu->exec(pos);
}

void PlayerForm::_delete_track() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	int id = idx.first().row();
	_current_playlist.removeTrackAt(id);
	_lib->saveCurrentPlaylist(_current_playlist);
	reload();
}

void PlayerForm::_enqueue_track() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	int id = idx.first().row();
	_player->enqueue(id);
}

void PlayerForm::_add_to_favorites() {
	QList<QModelIndex> idx = ui->playlistView->selectionModel()->selectedIndexes();
	int id = idx.first().row();
	_lib->addToFavorites(_current_playlist.tracks().at(id));
}
