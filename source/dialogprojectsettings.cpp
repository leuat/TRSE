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
#include "dialogprojectsettings.h"
#include "ui_dialogprojectsettings.h"

DialogProjectSettings::DialogProjectSettings(QString dir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogProjectSettings)
{
    m_currentDir = dir;
    ui->setupUi(this);
}

DialogProjectSettings::~DialogProjectSettings()
{
    delete ui;
}

void DialogProjectSettings::on_btnClose_clicked()
{
    FillToIni();
    m_ini->Save();
    close();

}

void DialogProjectSettings::FillFromIni()
{
    ui->leZeropageScreenMemory->setText(m_ini->getString("zeropage_screenmemory"));

    ui->leDecrunchZp1->setText(m_ini->getString("zeropage_decrunch1"));
    ui->leDecrunchZp2->setText(m_ini->getString("zeropage_decrunch2"));
    ui->leDecrunchZp3->setText(m_ini->getString("zeropage_decrunch3"));
    ui->leDecrunchZp4->setText(m_ini->getString("zeropage_decrunch4"));

    ui->leInternalZp1->setText(m_ini->getString("zeropage_internal1"));
    ui->leInternalZp2->setText(m_ini->getString("zeropage_internal2"));
    ui->leInternalZp3->setText(m_ini->getString("zeropage_internal3"));
    ui->leInternalZp4->setText(m_ini->getString("zeropage_internal4"));

    ui->leZeropages->setText(  fromStringList(m_ini->getStringList("zeropages")));

    ui->leTempZP->setText(  fromStringList(m_ini->getStringList("temp_zeropages")));

    ui->cmbSystem->setCurrentText(m_ini->getString("system"));

    ui->cmbOutputType->setCurrentText(m_ini->getString("output_type"));
    //ui->leMainFile->setText(m_ini->getString("main_ras_file"));

    ui->chkPassLda->setChecked(m_ini->getdouble("post_optimizer_passlda")==1);
    ui->chkPassJmp->setChecked(m_ini->getdouble("post_optimizer_passjmp")==1);
    ui->chkPassLdaTax->setChecked(m_ini->getdouble("post_optimizer_passldatax")==1);

    ui->chkPassStaLda->setChecked(m_ini->getdouble("post_optimizer_passstalda")==1);
    ui->chkPassLdx->setChecked(m_ini->getdouble("post_optimizer_passldx")==1);


    if (m_ini->getString("system")=="C128") {
        ui->cmbColumns->setCurrentText(QString::number(m_ini->getdouble("columns")));
//        qDebug() << "WHOO1  " << m_ini->getFloat("columns");
    }


    if (m_ini->getString("system")=="VIC20") {
        ui->tabConfigs->setCurrentWidget(ui->tabVic20);
    }

    ui->cmbVic20Config->setCurrentText(m_ini->getString("vic_memory_config"));


    QStringList files = m_ini->getStringList("disk_files");
    QStringList names = m_ini->getStringList("disk_names");

/*    for (int r=0;r<names.count();r++) {
        ui->tabData->insertRow(r);
        ui->tabData->setItem(r,0,new QTableWidgetItem(names[r]));
        ui->tabData->setItem(r,1,new QTableWidgetItem(files[r]));
    }

*/
//    ui->chkPOEnabled->setChecked(m_ini->getdouble("post_optimize")==1);


    QStringList ras = Util::FindFilesOfType(m_currentDir+"/","ras");
//    qDebug() << ras << m_currentDir;
    ui->cmbMainRas->clear();
    ui->cmbMainRas->addItem("none");
    ui->cmbMainRas->addItems(ras);

    ui->cmbMainRas->setCurrentText(m_ini->getString("main_ras_file"));


    QStringList paw = Util::FindFilesOfType(m_currentDir+"/","paw");
//    qDebug() << ras << m_currentDir;
    ui->cmbPawInclude->clear();
    ui->cmbPawInclude->addItem("none");
    ui->cmbPawInclude->addItems(paw);

    ui->cmbPawInclude->setCurrentText(m_ini->getString("d64_paw_file"));

    ui->leBackgroundColor->setText(QString::number(m_ini->getdouble("background_color")));
    ui->leBorderColor->setText(QString::number(m_ini->getdouble("border_color")));
}

void DialogProjectSettings::FillToIni()
{
    m_ini->setStringList("zeropages", toStringList(ui->leZeropages->text()));
    m_ini->setStringList("temp_zeropages", toStringList(ui->leTempZP->text()));

    m_ini->setString("zeropage_screenmemory", Util::numToHex(Util::NumberFromStringHex(ui->leZeropageScreenMemory->text())));

    m_ini->setFloat("border_color", ui->leBorderColor->text().toInt());
    m_ini->setFloat("background_color", ui->leBackgroundColor->text().toInt());

    m_ini->setString("zeropage_decrunch1", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp1->text())));
    m_ini->setString("zeropage_decrunch2", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp2->text())));
    m_ini->setString("zeropage_decrunch3", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp3->text())));
    m_ini->setString("zeropage_decrunch4", Util::numToHex(Util::NumberFromStringHex(ui->leDecrunchZp4->text())));

    m_ini->setString("zeropage_internal1", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp1->text())));
    m_ini->setString("zeropage_internal2", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp2->text())));
    m_ini->setString("zeropage_internal3", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp3->text())));
    m_ini->setString("zeropage_internal4", Util::numToHex(Util::NumberFromStringHex(ui->leInternalZp4->text())));

    m_ini->setString("system", ui->cmbSystem->currentText());


    m_ini->setFloat("post_optimizer_passlda", ui->chkPassLda->isChecked());
    m_ini->setFloat("post_optimizer_passjmp", ui->chkPassJmp->isChecked());
    m_ini->setFloat("post_optimizer_passldatax", ui->chkPassLdaTax->isChecked());
    m_ini->setFloat("post_optimizer_passstalda", ui->chkPassStaLda->isChecked());
    m_ini->setFloat("post_optimizer_passldx", ui->chkPassLdx->isChecked());
    //  m_ini->setFloat("post_optimize", ui->chkPOEnabled->isChecked());

    if (m_ini->getString("system")=="C128") {
        m_ini->setString("columns", ui->cmbColumns->currentText());

    }
    if (m_ini->getString("system")=="VIC20") {

        m_ini->setString("vic_memory_config",ui->cmbVic20Config->currentText());
    }

    m_ini->setString("output_type", ui->cmbOutputType->currentText());
    m_ini->setString("main_ras_file", ui->cmbMainRas->currentText());
    m_ini->setString("d64_paw_file", ui->cmbPawInclude->currentText());

//    FillTabDataToIni();

}

/*void DialogProjectSettings::FillTabDataToIni()
{
    QStringList names;
    QStringList files;

    for (int r=0;r<ui->tabData->rowCount();r++) {
        names<< ui->tabData->item(r,0)->text();
        files<< ui->tabData->item(r,1)->text();

    }

    m_ini->setStringList("disk_files",files);
    m_ini->setStringList("disk_names",names);

}

void DialogProjectSettings::on_pushButton_clicked()
{
    int r =ui->tabData->rowCount();
    ui->tabData->insertRow(r);
    ui->tabData->setItem(r,0,new QTableWidgetItem(""));
    ui->tabData->setItem(r,1,new QTableWidgetItem(""));
    ui->tabData->setItem(r,2,new QTableWidgetItem(""));

}

void DialogProjectSettings::on_pushButton_2_clicked()
{
    ui->tabData->removeRow(ui->tabData->currentRow());

}

*/
