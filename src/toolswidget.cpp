/*
 * SomePlayer - An alternate music player for Maemo 5
 * Copyright (C) 2010 Nikolay (somebody) Tischenko <niktischenko@gmail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "toolswidget.h"
#include "ui_toolswidget.h"
#include <QDesktopWidget>
#include "config.h"

using namespace SomePlayer::Storage;

ToolsWidget::ToolsWidget(QWidget *parent) :
		QWidget(parent),
		ui(new Ui::ToolsWidget)
{
	ui->setupUi(this);
	_fullscreen = false;
	connect (ui->fscreenButton, SIGNAL(clicked()), this, SLOT(_fullscreen_button()));
	connect (ui->nextButton, SIGNAL(clicked()), this, SIGNAL(nextSearch()));
	connect (ui->prevButton, SIGNAL(clicked()), this, SIGNAL(prevSearch()));
	connect (ui->searchLine, SIGNAL(textEdited(QString)), this, SIGNAL(search(QString)));
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
}

ToolsWidget::~ToolsWidget()
{
	delete ui;
}

void ToolsWidget::_fullscreen_button() {
	_fullscreen = !_fullscreen;
	emit toggleFullscreen(_fullscreen);
	ui->fscreenButton->setIcon(QIcon(_fullscreen ? ":/icons/"+_icons_theme+"/window.png" : ":/icons/"+_icons_theme+"/fullscreen.png"));
}

void ToolsWidget::reset() {
	ui->searchLine->setText("");
}

void ToolsWidget::setFocus() {
	ui->searchLine->setFocus();
}

void ToolsWidget::updateIcons() {
	Config config;
	_icons_theme = config.getValue("ui/iconstheme").toString();
	ui->fscreenButton->setIcon(QIcon(_fullscreen ? ":/icons/"+_icons_theme+"/window.png" : ":/icons/"+_icons_theme+"/fullscreen.png"));
	ui->nextButton->setIcon(QIcon(":/icons/"+_icons_theme+"/forward.png"));
	ui->prevButton->setIcon(QIcon(":/icons/"+_icons_theme+"/back.png"));
}

void ToolsWidget::show() {
	updateIcons();
	QWidget::show();
}
