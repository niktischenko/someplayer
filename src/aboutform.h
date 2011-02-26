#ifndef ABOUTFORM_H
#define ABOUTFORM_H

#include <QWidget>
#include "someplayer.h"

namespace Ui {
	class AboutForm;
}

class AboutForm : public QWidget
{
	Q_OBJECT

public:
	explicit AboutForm(QWidget *parent = 0);
	~AboutForm();

public slots:
	void updateTranslations();
	void updateIcons();

private slots:
	void onlineHelp();
	void donate();

private:
	Ui::AboutForm *ui;
	QString _icons_theme;
};

#endif // ABOUTFORM_H
