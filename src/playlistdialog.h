#ifndef PLAYLISTDIALOG_H
#define PLAYLISTDIALOG_H

#include <QDialog>

namespace Ui {
    class PlaylistDialog;
}

class PlaylistDialog : public QDialog
{
    Q_OBJECT

public:
    explicit PlaylistDialog(QList<QString> playlistNames, QWidget *parent = 0);
    ~PlaylistDialog();

    QList<QString> selected();

private:
    Ui::PlaylistDialog *ui;

private slots:
};

#endif // PLAYLISTDIALOG_H
