/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef DIALOGMESSAGE_H
#define DIALOGMESSAGE_H

#include <QDialog>
#include <QString>
#include <QImage>

namespace Ui {
class DialogMessage;
}

class DialogMessage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogMessage(QWidget *parent = 0);
    ~DialogMessage();

    void SetMessage(QString msg, QImage img);

    void SetHelp();

    bool ignoreIsChecked();

private slots:
    void on_pushButton_clicked();

private:
    Ui::DialogMessage *ui;
};

#endif // DIALOGMESSAGE_H
