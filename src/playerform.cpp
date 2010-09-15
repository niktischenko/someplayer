#include "playerform.h"
#include "ui_playerform.h"
#include "library.h"
#include <QDebug>
#include <QTime>
#include <QSlider>

using namespace SomePlayer::DataObjects;
using namespace SomePlayer::Playback;

inline void __fill_list(QStandardItemModel *_model, Playlist playlist) {
	_model->clear();
	QList<Track> tracks = playlist.tracks();
	int count = tracks.count();
	_model->setRowCount(count);
	for (int i = 0; i < count; i++) {
		TrackMetadata meta = tracks.at(i).metadata();
		_model->setItem(i, 0, new QStandardItem(meta.title()));
		_model->setItem(i, 1, new QStandardItem(meta.artist()));
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
	connect(ui->playButton, SIGNAL(clicked()), _player, SLOT(play()));
	connect(ui->pauseButton, SIGNAL(clicked()), _player, SLOT(pause()));
	connect(ui->stopButton, SIGNAL(clicked()), _player, SLOT(stop()));
	connect(ui->nextButton, SIGNAL(clicked()), _player, SLOT(next()));
	connect(ui->prevButton, SIGNAL(clicked()), _player, SLOT(prev()));
	connect(_player, SIGNAL(trackChanged(Track)), this, SLOT(_track_changed(Track)));
	connect(_player, SIGNAL(tick(int,int)), this, SLOT(_tick(int,int)));
	_seek_slider = new QSlider(Qt::Horizontal);
	ui->progressLayout->insertWidget(1, _seek_slider);
	_seek_slider->setTracking(false);
	connect(_seek_slider, SIGNAL(sliderMoved(int)), _player, SLOT(seek(int)));
	_model = new QStandardItemModel(0, 2, this);
	ui->playlistView->setModel(_model);
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
}

void PlayerForm::_track_changed(Track track) {
	int id = _current_playlist.tracks().indexOf(track);
	QModelIndex index = _model->index(id, 0);
	ui->playlistView->setCurrentIndex(index);
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
}

void PlayerForm::_tick(int done, int all) {
	QTime time1(0, all/60, all%60);
	QTime time2(0, done/60, done%60);
	ui->allTimeLabel->setText(time1.toString("mm:ss"));
	ui->doneTimeLabel->setText(time2.toString("mm:ss"));
	_seek_slider->setValue(done);
}

void PlayerForm::_slider_released() {
	_player->seek(_seek_slider->value());
}
