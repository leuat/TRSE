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

#ifndef DIALOGHELP_H
#define DIALOGHELP_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QListWidgetItem>

namespace Ui {
class DialogHelp;
}

class HelpTopic {
public:
    QString m_text;
    QString m_info;
    QVector<HelpTopic*> m_topics;

};

class Helper {
public:
    HelpTopic m_topics;
    void LoadFromResource();
    QStringList m_lst;
    int m_curIdx = 0;
    int m_curTab = 0;
    void Build(HelpTopic& t, int curTab);
};


class DialogHelp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHelp(QWidget *parent, QString search);
    ~DialogHelp();
    Helper m_helper;

    int m_curTopic, m_curItem;


    void FillTopics();
    void FillItems(int idx);
    void FillHelpText();
private slots:
    void on_pushButton_clicked();

    void on_lstTopic_itemClicked(QListWidgetItem *item);

    void on_lstItems_itemClicked(QListWidgetItem *item);

    void on_leSearch_textChanged(const QString &arg1);

    void on_lstItems_currentRowChanged(int currentRow);

private:
    Ui::DialogHelp *ui;
};

#endif // DIALOGHELP_H
