#ifndef LIBRARYFORM_H
#define LIBRARYFORM_H

#include <QWidget>

namespace Ui {
    class LibraryForm;
}

class LibraryForm : public QWidget
{
    Q_OBJECT

public:
    explicit LibraryForm(QWidget *parent = 0);
    ~LibraryForm();

private:
    Ui::LibraryForm *ui;
};

#endif // LIBRARYFORM_H
