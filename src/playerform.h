#ifndef PLAYERFORM_H
#define PLAYERFORM_H

#include <QWidget>
#include "someplayer.h"

namespace Ui {
    class PlayerForm;
}

using SomePlayer::DataObjects::Library;

class PlayerForm : public QWidget
{
    Q_OBJECT

public:
	explicit PlayerForm(Library *lib, QWidget *parent = 0);
    ~PlayerForm();
signals:
	void library();

private slots:
	void _library();

private:
    Ui::PlayerForm *ui;
	Library *_lib;
};

#endif // PLAYERFORM_H
