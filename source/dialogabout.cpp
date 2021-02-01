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

#include "dialogabout.h"
#include "ui_dialogabout.h"
#include "source/LeLib/data.h"
#include <QFile>
#include <QDate>
#include <QFontDatabase>

DialogAbout::DialogAbout(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAbout)
{
    ui->setupUi(this);
    /*ui->txtAbout->setOpenExternalLinks(true);
    ui->txtAbout->setText( ui->txtAbout->toHtml().replace("@version",Data::data.version));
    ui->txtAbout->setText( ui->txtAbout->toHtml().replace("@date", QDate::currentDate().toString()));
*/

    QFont fnt = QFont(Data::data.fontFamily, 11);
    QFont fntH = QFont(Data::data.fontFamily, 18);
 /*   this->setFont(fnt);

    for (auto q : this->findChildren<QWidget*>())
        q->setFont(fnt);
*/
    setWindowFlags(Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint);

    this->setFixedWidth(this->width());
    //this->setFixedHeight(this->height());
    ui->lblHeader->setFont(fntH);
    ui->lblHeader->setText("Version "+Data::data.version);
    ui->lblDate->setText("Released: "+QDate::currentDate().toString());
    ui->lblHeader2->setFont(fntH);
    ui->lblHeader2->setStyleSheet("QLabel { color : yellow; }");
    ui->lblHeader->setStyleSheet("QLabel { color : yellow; }");
    ui->lblHeader3->setStyleSheet("QLabel { color : yellow; }");

    ui->lblHeader3->setFont(fntH);

    QPalette pal = palette();

    // set black background
    pal.setColor(QPalette::Window, QColor(45,44,157));
    setAutoFillBackground(true);

    setPalette(pal);

/*    QFile f(":resources/text/about.txt");
  //  if (QFile::exists(":resources/text/about.txt"))
      //  exit(1);
    f.open(QIODevice::ReadOnly | QFile::Text) ;
    ui->txtAbout->setText(f.readAll());
    f.close();
*/

}

DialogAbout::~DialogAbout()
{
    delete ui;
}

void DialogAbout::SetText(QString title, QString text)
{
    setWindowTitle(title);
    //ui->txtAbout->setText(text);
}

void DialogAbout::on_pushButton_clicked()
{
    close();
}
