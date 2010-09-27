#ifndef TIMERDIALOG_H
#define TIMERDIALOG_H

#include <QDialog>

namespace Ui {
    class TimerDialog;
}

class TimerDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TimerDialog(QWidget *parent = 0);
    ~TimerDialog();
    void getTime(int *, int *, int *);
    bool timerDisabled();

public slots:
    void init();
    void setTime (int, int, int);
    void showDisable();


private:
    Ui::TimerDialog *ui;
};

#endif // TIMERDIALOG_H
