#ifndef PLAYERFORM_H
#define PLAYERFORM_H

#include <QWidget>

namespace Ui {
    class PlayerForm;
}

class PlayerForm : public QWidget
{
    Q_OBJECT

public:
	explicit PlayerForm(QWidget *parent = 0);
    ~PlayerForm();
signals:
	void library();

private slots:
	void _library();

private:
    Ui::PlayerForm *ui;
};

#endif // PLAYERFORM_H
