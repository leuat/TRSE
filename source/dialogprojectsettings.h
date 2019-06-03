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

#ifndef DIALOGPROJECTSETTINGS_H
#define DIALOGPROJECTSETTINGS_H

#include <QDialog>
#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"
#include "source/messages.h"

namespace Ui {
class DialogProjectSettings;
}

class DialogProjectSettings : public QDialog
{
    Q_OBJECT
public:
    CIniFile* m_ini;
    QString m_currentDir;
    void SetInit(CIniFile* ini) {
        m_ini = ini;
        FillFromIni();
    }

    QString fromStringList(QStringList lst) {
        QString whole ="";
        for (QString s: lst) {
            whole+=s;
            if (lst.last()!=s)
                whole=whole+", ";
        }
        return whole;
    }

    QStringList toStringList(QString s) {
        QStringList lst = s.split(",");
        return lst;
    }


public:
    explicit DialogProjectSettings(QString dir, QWidget *parent = nullptr);
    ~DialogProjectSettings();

private slots:
    void on_btnClose_clicked();

  //  void on_pushButton_clicked();

   // void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    void FillFromIni();
    void FillToIni();
//    void FillTabDataToIni();
    void Close();
    Ui::DialogProjectSettings *ui;
};

#endif // DIALOGPROJECTSETTINGS_H
