#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "playerform.h"
#include "libraryform.h"
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
private:
	PlayerForm *_playerForm;
	LibraryForm *_libraryForm;
	Library *_library;
};

#endif // MAINWINDOW_H
