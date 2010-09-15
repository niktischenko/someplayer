#ifndef BUSYWIDGET_H
#define BUSYWIDGET_H

#include <QWidget>

namespace Ui {
    class BusyWidget;
}

class BusyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BusyWidget(QWidget *parent = 0);
    ~BusyWidget();
	void setText(QString text);

private:
    Ui::BusyWidget *ui;
};

#endif // BUSYWIDGET_H
