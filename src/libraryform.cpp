#include "libraryform.h"
#include "ui_libraryform.h"

LibraryForm::LibraryForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LibraryForm)
{
    ui->setupUi(this);
}

LibraryForm::~LibraryForm()
{
    delete ui;
}
