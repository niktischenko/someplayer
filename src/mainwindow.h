#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "playerform.h"
#include "libraryform.h"

namespace Ui {
	class MainWindow;
}

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
};

#endif // MAINWINDOW_H
