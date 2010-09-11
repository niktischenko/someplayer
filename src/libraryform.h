#ifndef LIBRARYFORM_H
#define LIBRARYFORM_H

#include <QWidget>
#include "someplayer.h"

#include <QStandardItemModel>
#include <QStandardItem>
#include <QModelIndex>

namespace Ui {
    class LibraryForm;
}

using SomePlayer::DataObjects::Library;
using SomePlayer::DataObjects::Track;

enum LibraryFormListState {STATE_NONE, STATE_ARTIST, STATE_ALBUM, STATE_TRACK, STATE_PLAYLIST, STATE_PLAYLIST_TRACK};

class LibraryForm : public QWidget
{
    Q_OBJECT

public:
	explicit LibraryForm(Library *lib, QWidget *parent = 0);
    ~LibraryForm();
signals:
	void player();
private slots:
	void _player();
	void _view_button();
	void _dynamic_button();
	void _playlists_button();
	void _add_button();
	void _back_button();
	void _process_list_click(QModelIndex);

private:
    Ui::LibraryForm *ui;
	Library *_lib;
	QStandardItemModel *_model;
	LibraryFormListState _state;
	QString _current_artist;
	QString _current_album;
	QList<Track> _current_tracks;

	void _add_artist(QString artist);
	void _add_album(QString artist, QString album);
	void _add_track(Track track);
	void _add_playlist(QString name);
};

#endif // LIBRARYFORM_H
