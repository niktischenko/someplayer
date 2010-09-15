#ifndef PLAYERFORM_H
#define PLAYERFORM_H

#include <QWidget>
#include "someplayer.h"
#include "playlist.h"
#include <QStandardItemModel>
#include "player/player.h"
#include <QSlider>

namespace Ui {
    class PlayerForm;
}

using SomePlayer::DataObjects::Library;
using SomePlayer::DataObjects::Playlist;
using SomePlayer::DataObjects::Track;
using SomePlayer::Playback::Player;

class PlayerForm : public QWidget
{
    Q_OBJECT

public:
	explicit PlayerForm(Library *lib, QWidget *parent = 0);
    ~PlayerForm();
signals:
	void library();

public slots:
	void reload();

private slots:
	void _library();
	void _toggle_view();
	void _process_click(QModelIndex);
	void _track_changed(Track);
	void _tick(int, int);
	void _slider_released();

private:
    Ui::PlayerForm *ui;
	Library *_lib;
	Playlist _current_playlist;
	QStandardItemModel *_model;
	Player *_player;
	QSlider *_seek_slider;

	void _display_track(Track);
};

#endif // PLAYERFORM_H
