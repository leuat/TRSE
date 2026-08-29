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

#include "dialogexport.h"
#include "ui_dialogexport.h"
#include <QLabel>
#include <QLineEdit>

DialogExport::DialogExport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogExport)
{
    ui->setupUi(this);

}

void DialogExport::Init(LImage *img)
{
    m_image = img;
    FillParams();

}

DialogExport::~DialogExport()
{
    delete ui;
}

void DialogExport::FillParams()
{
    int row =0;
    for (QString key : m_image->m_exportParams.keys()) {
        float val = m_image->m_exportParams[key];
        QLabel *lab = new QLabel(key);
        ui->grdParams->addWidget(lab,row,0);
        QLineEdit *le = new QLineEdit(QString::number(val));
        ui->grdParams->addWidget(lab,row,0);
        ui->grdParams->addWidget(le,row,1);
        m_les[key] = le;
        row++;
        QString comment = m_image->m_exportParamsComments[key];
        if (comment!=nullptr) {
            QLabel *lab = new QLabel(comment);
            ui->grdParams->addWidget(lab,row,0);
            row++;
        }

    }
}

void DialogExport::Apply()
{
    for (QString key : m_les.keys()) {
        m_image->m_exportParams[key] = m_les[key]->text().toFloat();
    }
}

void DialogExport::on_pushButton_clicked()
{
    isOk=true;
    Apply();
    close();
}

void DialogExport::on_pushButton_2_clicked()
{
    isOk=false;
    close();
}
