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

#include "dialoghelp.h"
#include "ui_dialoghelp.h"
#include <QString>
#include <QStringList>
#include <QFile>
#include <QDebug>

DialogHelp::DialogHelp(QWidget *parent, QString txt) :
    QDialog(parent),
    ui(new Ui::DialogHelp)
{
    ui->setupUi(this);
    m_curTopic = 0;
    m_curItem = 0;

    m_helpTypes.append(HelpType("m","Methods"));
    m_helpTypes.append(HelpType("r","Reserved words"));
    m_helpTypes.append(HelpType("c","Constants"));

    FillTopics();
    LoadItems(0);
    ui->leSearch->setText(txt);



}

DialogHelp::~DialogHelp()
{
    delete ui;
}

void DialogHelp::LoadItems(int idx)
{
    HelpType ht = m_helpTypes[idx];
    QString currentSystem = Syntax::StringFromSystem(Syntax::s.m_currentSystem).toLower();
    m_idx=0;
    ui->lstItems->clear();
    m_currentItems.clear();
    for (QString s: Syntax::s.m_syntaxData.split('\n')) {
        s= s.simplified();
        if (s.count()==0) continue;
        if (s.startsWith("#")) continue;
        s=s.replace(" ", "");

        QStringList data = s.split(";");
        if (data[0].toLower()== ht.id) {
            QString word = data[1];
            QString system = data[2].toLower();

            if (system.contains(currentSystem)) {
/*                QString val = word + "(";
                for (QString s: params) {
                    if (s=="b") val+="[byte variable]";
                    if (s=="i") val+="[integer variable]";
                    if (s=="n") val+="[numeric value]";
                    if (s=="a") val+="[address]";
                    if (s=="s") val+="[string address]";

                    val+=", ";

                }
                val.remove(val.length()-2,2);
                val+=");"*/
                AppendItem(ui->lstItems, word);
                m_currentItems.append(word);

//                ui->txtHelp->setText(txt);

            }

         }
    }
}

void DialogHelp::LoadItem(QString findword)
{
    for (QString s: Syntax::s.m_syntaxData.split('\n')) {
        s= s.simplified();
        if (s.count()==0) continue;
        if (s.startsWith("#")) continue;
        s=s.replace(" ", "");

        QStringList data = s.split(";");
        QString word = data[1];
        if (word!=findword)
            continue;
        QString type = data[0].toLower();
        m_currentType = type;
//        QString system = data[2].toLower();
        if (type=="m")
         {
                QStringList params = data[3].toLower().split(",");
                QString val = "<font size=+2><b>"+word + "(";
                for (QString s: params) {
                    if (s=="b") val+="[byte variable]";
                    if (s=="i") val+="[integer variable]";
                    if (s=="n") val+="[numeric value]";
                    if (s=="a") val+="[address]";
                    if (s=="s") val+="[string address]";

                    val+=", ";

                }
                val.remove(val.length()-2,2);
                val+=");</font></b><br><br>";

                QString fn =":resources/text/help/"+type+"/"+word.toLower()+".rtf";
                qDebug() << "looking for "<< fn;
                if (QFile::exists(fn)) {
                    QFile f(fn);
                    f.open(QFile::ReadOnly | QFile::Text);
                    QString s = f.readAll();
                    f.close();
                    val+=s;

                }


                ui->txtHelp->setText(val);

            }

        if (type=="c")
         {
                QString type = data[3].toLower();
                QString value = data[4].toUpper();

                QString val = "<font size=+2>"+word + "</font><br><br>";
                if (type=="a") val+="Address: ";
                if (type=="b") val+="Byte value: ";
                val+=value;


                ui->txtHelp->setText(val);



            }

         }
    }



void DialogHelp::FillTopics()
{
    m_idx=0;
    for (HelpType& ht:m_helpTypes)
        AppendItem(ui->lstTopic, ht.name);
}

void DialogHelp::AppendItem(QListWidget *w, QString text)
{
    QListWidgetItem* item= new QListWidgetItem();
    item->setData(Qt::UserRole,m_idx++);
    item->setText(text);
    w->addItem(item);

}


void DialogHelp::on_pushButton_clicked()
{
    close();
}



void DialogHelp::on_lstTopic_itemClicked(QListWidgetItem *item)
{
    m_curTopic = item->data(Qt::UserRole).toInt();
    LoadItems(m_curTopic);
}

void DialogHelp::on_lstItems_itemClicked(QListWidgetItem *item)
{
    int idx = item->data(Qt::UserRole).toInt();
    LoadItem(m_currentItems[idx]);
}

void DialogHelp::on_leSearch_textChanged(const QString &arg1)
{
}

void DialogHelp::on_lstItems_currentRowChanged(int currentRow)
{

}

