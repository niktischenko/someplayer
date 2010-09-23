#include "playlistdialog.h"
#include "ui_playlistdialog.h"
#include <QStandardItemModel>

PlaylistDialog::PlaylistDialog(QList<QString> playlistNames, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlaylistDialog)
{
    ui->setupUi(this);
    QStandardItemModel *model = new QStandardItemModel(this);
    foreach (QString str, playlistNames) {
	    model->appendRow(new QStandardItem(str));
    }
    ui->listView->setModel(model);
}

PlaylistDialog::~PlaylistDialog()
{
    delete ui;
}

QList<QString> PlaylistDialog::selected() {
	QList<QString> _selected;
	QList<QModelIndex> idx = ui->listView->selectionModel()->selectedIndexes();
	foreach (QModelIndex id, idx) {
		_selected.append(id.data().toString());
	}
	return _selected;
}
