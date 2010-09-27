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

#ifndef EDITTAGSDIALOG_H
#define EDITTAGSDIALOG_H

#include <QDialog>
#include "someplayer.h"
#include "trackmetainformation.h"

using SomePlayer::DataObjects::TrackMetadata;
namespace Ui {
    class EditTagsDialog;
}

class EditTagsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditTagsDialog(QWidget *parent = 0);
    ~EditTagsDialog();
    void setTrackMetadata(TrackMetadata meta);
    TrackMetadata meta();

private:
    Ui::EditTagsDialog *ui;
    TrackMetadata _meta;
};

#endif // EDITTAGSDIALOG_H
