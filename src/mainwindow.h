#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include <QMenu>
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
	void openMedia();
	void aboutQt();
	void about();
	void player();
	void library();
	void showBusyWidget(QString);
private slots:
	void _add_directory();
	void _save_playlist();
	void _clear_current_playlist();
private:
	PlayerForm *_player_form;
	LibraryForm *_library_form;
	BusyWidget *_busy_widget;
	QMenu *_library_menu;
	QMenu *_player_menu;
	Library *_library;
};

#endif // MAINWINDOW_H
