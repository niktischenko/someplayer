#ifndef PLAYERFORM_H
#define PLAYERFORM_H

#include <QWidget>
#include "someplayer.h"
#include "playlist.h"
#include <QStandardItemModel>
#include "player/player.h"
#include <QSlider>
#include <QMenu>
#include <QTime>
#include "trackrenderer.h"

namespace Ui {
    class PlayerForm;
}

using SomePlayer::DataObjects::Library;
using SomePlayer::DataObjects::Playlist;
using SomePlayer::DataObjects::Track;
using SomePlayer::Playback::Player;
using SomePlayer::Playback::PlayerState;

class PlayerForm : public QWidget
{
    Q_OBJECT

public:
	explicit PlayerForm(Library *lib, QWidget *parent = 0);
    ~PlayerForm();
signals:
	void library();
	void showSearchPanel();
	void hideSearchPanel();

public slots:
	void reload();
	void search(QString &);
	void nextItem();
	void prevItem();
	void cancelSearch();

private slots:
	void _library();
	void _toggle_view();
	void _process_click(QModelIndex);
	void _track_changed(Track);
	void _tick(int, int);
	void _slider_released();
	void _custom_context_venu_requested ( const QPoint & pos );
	void _delete_track();
	void _enqueue_track();
	void _add_to_favorites();
	void _state_changed(PlayerState);
	void _toggle_repeat();
	void _toggle_random();

private:
    Ui::PlayerForm *ui;
	Library *_lib;
	Playlist _current_playlist;
	QTime *_time;
	QStandardItemModel *_model;
	Player *_player;
	QSlider *_seek_slider;
	QMenu *_context_menu;
	QString _search_pattern;

	TrackRenderer *_track_renderer;
	void _display_track(Track);
	int _search_current_id;
};

#endif // PLAYERFORM_H
