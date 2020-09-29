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

DialogHelp::DialogHelp(QWidget *parent, QString txt, QPalette pal) :
    QDialog(parent),
    ui(new Ui::DialogHelp)
{
    ui->setupUi(this);
    m_curTopic = 0;
    m_curItem = 0;

    m_helpTypes.append(HelpType("m","Methods"));
    m_helpTypes.append(HelpType("r","Reserved words"));
    m_helpTypes.append(HelpType("c","Constants"));
    m_helpTypes.append(HelpType("p","Platform Info"));
    m_helpTypes.append(HelpType("f","Fjong Ray tracer"));

     setWindowFlags(Qt::Window);
    FillTopics();
    LoadItems(0);
    if (txt=="")
        txt="Getting_Started";
    ui->leSearch->setText(txt);

    CIniFile colors;
    colors.Load(Util::path + "themes/" + "dark_standard.ini");
//    m_highlighter = new Highlighter(colors, 0, nullptr);

    if (txt!="")
        SearchForItem(txt);

/*    setPalette(pal);
    this->setPalette(pal);


    setStyleSheet("background-color:WHITE");
*/
//    QApplication::setPalette(pal, "DialogHelp");

}

DialogHelp::~DialogHelp()
{
    delete ui;
}




void DialogHelp::LoadItems(int idx)
{
    HelpType ht = m_helpTypes[idx];
    QString currentSystem = AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system).toLower();
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
            bool isFjong = data[0]=="f";
            if (word.toLower().startsWith("init")) continue;
            if (!isFjong && !AbstractSystem::isSupported(Syntax::s.m_currentSystem->m_system, system))
                continue;
            if (system.contains(currentSystem)||isFjong) {
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
    ui->lstItems->sortItems();

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
        QString system = data[2].toLower();
        if (type!="f" && !AbstractSystem::isSupported(Syntax::s.m_currentSystem->m_system, system))
            continue;
        if (type=="f")
         {
                QStringList params = data[2].toLower().split(",");
                QString val = "<h2 style=\"color: skyblue\">" + word + "( ";
                int paramNo = 1;
                for (QString s: params) {
                    if (s=="f") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> float ]";
                    if (s=="s") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> string ]";
                    val+=", ";
                    paramNo++;

                }
                val.remove(val.length()-2,2);
                val+=" );</h2>";

                QString fn =":resources/text/help/"+type+"/"+word.toLower()+".rtf";
                if (QFile::exists(fn)) {
                    QFile f(fn);
                    f.open(QFile::ReadOnly | QFile::Text);
                    QString s = f.readAll();
                    f.close();

                    s=s.replace("<code>","<pre><code style=\"color: #E0B050\">");
                    s=s.replace("</code>","</code></pre>");

                    s=s.replace("<h3>","<h3 style=\"color: yellow;font-size: 16pt;margin: 35px 0px 20px\">");

                    val+="<div style=\"font-size: 10pt\">" + s + "</div>";

             //       m_highlighter->HighlightText(val);
           //         qDebug() << val;
                }


                ui->txtHelp->setText(val);

            }


        if (type=="m")
         {
                QStringList params = data[3].toLower().split(",");
                QString val = "<h2 style=\"color: skyblue\">" + word + "( ";
                int paramNo = 1;
                for (QString s: params) {
                    if (s=="b") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> byte ]";
                    if (s=="i") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> integer ]";
                    if (s=="n") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> numeric ]";
                    if (s=="a") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> address ]";
                    if (s=="s") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> string address ]";
                    if (s=="p") val+="[ <span style=\"vertical-align:super\">" + QString::number( paramNo ) + ":</span> procedure ]";

                    val+=", ";
                    paramNo++;

                }
                val.remove(val.length()-2,2);
                val+=" );</h2>";


                QString fn =":resources/text/help/"+type+"/"+word.toLower()+".rtf";
                qDebug() << QFile::exists(fn) << fn;
                if (QFile::exists(fn)) {
                    QString s = Util::loadTextFile(fn);

                    s=s.replace("<code>","<pre><code style=\"color: #E0B050\">");
                    s=s.replace("</code>","</code></pre>");

                    s=s.replace("<h3>","<h3 style=\"color: yellow;font-size: 16pt;margin: 35px 0px 20px\">");

                    val+="<div style=\"font-size: 10pt\">" + s + "</div>";

             //       m_highlighter->HighlightText(val);
           //         qDebug() << val;
                }


                ui->txtHelp->setText(val);

            }
        // Fjong type


        if (type=="c")
         {
                QString ptype = data[3].toLower();
                QString value = data[4].toUpper();

                QString val = "<h2 style=\"color: skyblue\">" + word + "</h2><h3 style=\"color: yellow\">";
                if (ptype=="a") val += "Address: ";
                if (ptype=="b") val += "Byte value: ";
                val += value + "</h3>";

                QString fn =":resources/text/help/"+type+"/"+word.toLower()+".rtf";
                if (QFile::exists(fn)) {
                    QFile f(fn);
                    f.open(QFile::ReadOnly | QFile::Text);
                    QString s = f.readAll();
                    f.close();

                    s=s.replace("<code>","<pre><code style=\"color: #E0B050\">");
                    s=s.replace("</code>","</code></pre>");

                    s=s.replace("<h3>","<h3 style=\"color: yellow;font-size: 16pt;margin: 35px 0px 20px\">");

                    val+="<div style=\"font-size: 10pt\">" + s + "</div>";

                }

                ui->txtHelp->setText(val);
            }



        if (type=="r")
         {
                QString val = "<h2 style=\"color: skyblue\">" + word + "</h2>";
                val += "<h3 style=\"color: yellow\">" + data[2] + "</h3>";

                QString fn =":resources/text/help/"+type+"/"+word.toLower()+".rtf";
                if (QFile::exists(fn)) {
                    QFile f(fn);
                    f.open(QFile::ReadOnly | QFile::Text);
                    QString s = f.readAll();
                    f.close();

                    s=s.replace("<code>","<pre><code style=\"color: #E0B050\">");
                    s=s.replace("</code>","</code></pre>");

                    s=s.replace("<h3>","<h3 style=\"color: yellow;font-size: 16pt;margin: 35px 0px 20px\">");

                    val+="<div style=\"font-size: 10pt\">" + s + "</div>";

                }

                ui->txtHelp->setText(val);
            }



        if (type=="p")
         {
                QString title = QString(word).replace("_", " ");
                QString val = "<h2 style=\"color: skyblue\">" + title + "</h2>"; //.replace("_", " ")

                QString fn =":resources/text/help/"+type+"/"+word.toLower()+".rtf";
                if (QFile::exists(fn)) {
                    QFile f(fn);
                    f.open(QFile::ReadOnly | QFile::Text);
                    QString s = f.readAll();
                    f.close();

                    s=s.replace("<code>","<pre><code style=\"color: #E0B050\">");
                    s=s.replace("</code>","</code></pre>");

                    s=s.replace("<h3>","<h3 style=\"color: yellow;font-size: 16pt;margin: 35px 0px 20px\">");

                    val+="<div style=\"font-size: 10pt\">" + s + "</div>";

                }

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

void DialogHelp::SearchForItem(QString item)
{
    QString currentSystem = AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system).toLower();
    m_idx=0;
    ui->lstItems->clear();
    m_currentItems.clear();
    for (QString s: Syntax::s.m_syntaxData.split('\n')) {

        s= s.simplified();
        if (s.count()==0) continue;
        if (s.startsWith("#")) continue;
        s=s.replace(" ", "");

        QStringList data = s.split(";");
        if (data[1].toLower().startsWith(item.toLower())) {
            //HelpType ht = m_helpTypes[idx];
            int idx;
            for (int i=0;i<m_helpTypes.count();i++)
                if (m_helpTypes[i].id.toLower()==data[0].toLower())
                    idx=i;

            if (idx>=0) {
                ui->lstTopic->setCurrentRow(idx);
            }
            LoadItems(idx);
            auto items =  ui->lstItems->findItems(item,Qt::MatchContains);
            if (m_currentSearchItem>=items.count())
                m_currentSearchItem=0;
            if (items.count()!=0) {
                ui->lstItems->setCurrentItem(items[m_currentSearchItem]);
                data[1] = items[m_currentSearchItem]->text();
            }
            LoadItem(data[1]);
            return;
        }
    }
    LoadItems(0);
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
    m_currentSearchItem = 0; // Reset counter
    SearchForItem(arg1);
}

void DialogHelp::on_lstItems_currentRowChanged(int currentRow)
{

    if (ui->lstItems->item(currentRow)==nullptr)
        return;

    int idx = ui->lstItems->item(currentRow)->data(Qt::UserRole).toInt();
    LoadItem(m_currentItems[idx]);

}


void DialogHelp::on_leSearch_returnPressed()
{
    m_currentSearchItem++;
    SearchForItem(ui->leSearch->text());
}
