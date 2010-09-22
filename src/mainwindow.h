#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "playerform.h"
#include "libraryform.h"
#include "busywidget.h"
#include "library.h"

namespace Ui {
	class MainWindow;
}

using SomePlayer::DataObjects::Library;

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

private:
	Ui::MainWindow *ui;

public slots:
	void aboutQt();
	void about();
	void player();
	void library();
	void showBusyWidget(QString);
	void showSearchPanel();
	void hideSearchPanel();
private slots:
	void _add_directory();
	void _save_playlist();
	void _clear_current_playlist();
	void _toggle_search_line();
	void _search(QString);
	void _nextItem();
	void _prevItem();
	void _cancelSearch();
	void _toggle_full_screen();
private:
	PlayerForm *_player_form;
	LibraryForm *_library_form;
	BusyWidget *_busy_widget;
	Library *_library;
};

#endif // MAINWINDOW_H
