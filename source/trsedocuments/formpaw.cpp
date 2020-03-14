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

#include "formpaw.h"
#include <QProcess>
#include "ui_formpaw.h"

FormPaw::FormPaw(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormPaw)
{
    ui->setupUi(this);
    ui->tabData->setColumnWidth(0, 200);
    ui->tabData->setColumnWidth(1, 500);
    ui->tabData->setColumnWidth(2, 160);
    ui->tabData->setColumnWidth(3, 40);

}

FormPaw::~FormPaw()
{
    delete ui;
}

void FormPaw::FillToIni()
{
    m_pawData.setString("ras_include_file",ui->leOutfile->text());
    m_pawData.setString("packed_address",Util::numToHex(Util::VerifyHexAddress(ui->lePackedAddress->text())));
    m_pawData.setString("output_dir",ui->leOutDir->text());
    m_pawData.setFloat("use_exomizer3", ui->chkExomizer3->isChecked()?1:0);
    m_pawData.setString("address_field", ui->leAddressField->text());

    QStringList data, data_tinycrunch;
    m_files.clear();

    for (int r=0;r<ui->tabData->rowCount();r++) {
//        for (int c=0;c<ui->tabData->columnCount();c++) {
        data << ui->tabData->item(r,0)->text();
        data << ui->tabData->item(r,1)->text();
        QString useTiny = "0";
        if (ui->tabData->item(r,3)!=nullptr)
            useTiny =  ui->tabData->item(r,3)->text();
        data_tinycrunch << useTiny;
           data<< Util::numToHex(Util::VerifyHexAddress(ui->tabData->item(r,2)->text()));

        PawFile pf(m_projectIniFile->getString("project_path")+"/"+ui->tabData->item(r,1)->text(),
                   m_projectIniFile->getString("project_path")+"/"+  m_pawData.getString("output_dir")+"/" + ui->tabData->item(r,0)->text() + "_c.bin",

                   Util::numToHex(Util::VerifyHexAddress(ui->tabData->item(r,2)->text())),
                   ui->tabData->item(r,0)->text()
                   );
        pf.incCFile = m_pawData.getString("output_dir")+"/" + ui->tabData->item(r,0)->text() + "_c.bin";
        pf.tinyCrunch = useTiny=="1";
        m_files.append(pf);
    }
    m_pawData.setStringList("data",data);
    m_pawData.setStringList("data_tinycrunch", data_tinycrunch);



}

void FormPaw::FillFiles()
{

}

void FormPaw::FillFromIni()
{
    ui->leOutfile->setText(m_pawData.getString("ras_include_file"));
    ui->lePackedAddress->setText(m_pawData.getString("packed_address"));
    ui->leOutDir->setText(m_pawData.getString("output_dir"));
    ui->leAddressField->setText(m_pawData.getString("address_field"));
    ui->chkExomizer3->setChecked(m_pawData.getdouble("use_exomizer3")==1);
    QStringList data = m_pawData.getStringList("data");
    QStringList dataTinycrunch = m_pawData.getStringList("data_tinycrunch");


    for (int r=0;r<data.count()/3;r++) {
        ui->tabData->insertRow(r);
        ui->tabData->setItem(r,0,new QTableWidgetItem(data[3*r+0]));
        ui->tabData->setItem(r,1,new QTableWidgetItem(data[3*r+1]));
        ui->tabData->setItem(r,2,new QTableWidgetItem(data[3*r+2]));
        if (r<dataTinycrunch.count())
           ui->tabData->setItem(r,3,new QTableWidgetItem(dataTinycrunch[r]));
    }

}

void FormPaw::Save(QString filename)
{
    FillToIni();
    m_pawData.Save(filename);

}

void FormPaw::Load(QString filename)
{
    m_pawData.Load(filename);
    FillFromIni();
}


void FormPaw::Build() {
    FillToIni();
    if (pt!=nullptr) {
        if (pt->isRunning())
            return;
        delete pt;
    }
    pt = new PawThread(&m_pawData, m_projectIniFile, m_iniFile, &m_files);
    connect(pt, SIGNAL(EmitTextUpdate()), this, SLOT(onTextUpdate()));

    pt->output="";

 //   QString isExomizer3 = "-P0";
    pt->isExomizer3 = "";
    if (m_pawData.getdouble("use_exomizer3")==1)
            pt->isExomizer3="-P0";

    QString ex = m_iniFile->getString("exomizer");

    if (!QFile::exists(ex)) {
        ui->leOutput->setText("Exomizer not correctly set up");
        return;
    }
    for (PawFile& pf: m_files) {
        if (!QFile::exists(pf.inFile)) {
            ui->leOutput->setText("Could not find file: " + pf.inFile);
            return;
        }
    }
    QString outFolder = m_projectIniFile->getString("project_path")+"/"+m_pawData.getString("output_dir");

    if (!QDir().exists(outFolder))
            QDir().mkdir(outFolder);


    pt->start();

    //pt->output +="Compression done!";
    //ui->leOutput->setText(pt->output);
    // Create include file

}

void FormPaw::BuildSingle()
{
    pt = new PawThread(&m_pawData, m_projectIniFile, m_iniFile, &m_files);

    pt->output="";

 //   QString isExomizer3 = "-P0";
    pt->isExomizer3 = "";
    if (m_pawData.getdouble("use_exomizer3")==1)
            pt->isExomizer3="-P0";

    QString ex = m_iniFile->getString("exomizer");

    if (!QFile::exists(ex)) {
//        ui->leOutput->setText("Exomizer not correctly set up");
        return;
    }
    for (PawFile& pf: m_files) {
        if (!QFile::exists(pf.inFile)) {
  //          ui->leOutput->setText("Could not find file: " + pf.inFile);
            return;
        }
    }
    QString outFolder = m_projectIniFile->getString("project_path")+"/"+m_pawData.getString("output_dir");

    if (!QDir().exists(outFolder))
            QDir().mkdir(outFolder);

    pt->m_ignoreIncludefile = true;
    pt->run();

}


/*void FormPaw::BuildRegular()
{
    FillToIni();
    ui->leOutput->setText("");
    output="";
    QString ex = m_iniFile->getString("exomizer");
 //   QString isExomizer3 = "-P0";
    QString isExomizer3 = "";
    if (m_pawData.getdouble("use_exomizer3")==1)
            isExomizer3="-P0";

    if (!QFile::exists(ex)) {
        ui->leOutput->setText("Exomizer not correctly set up");
        return;
    }
    for (PawFile& pf: m_files) {
        if (!QFile::exists(pf.inFile)) {
            ui->leOutput->setText("Could not find file: " + pf.inFile);
            return;
        }
    }
    QString outFolder = m_projectIniFile->getString("project_path")+"/"+m_pawData.getString("output_dir");

    if (!QDir().exists(outFolder))
            QDir().mkdir(outFolder);


    for (PawFile& pf: m_files) {
            QProcess processCompress;
            QString adr = pf.address;
            QString of = pf.inFile+"@" + adr.replace("$","0x");
            QStringList params = QStringList() << "mem" << "-lnone" <<of << "-o" << pf.cFile;
            if (isExomizer3!="")
                params<<isExomizer3;

            output+="Compressing :"+ pf.inFile + "\n";
            ui->leOutput->setText(output);

            processCompress.start(ex, params  );
            processCompress.waitForFinished();
            QString err(processCompress.readAllStandardError());
            QString std = processCompress.readAllStandardOutput();
            if (std.toLower().contains("error")) {
                output = "<b>ERROR</b><br>"+std;
                ui->leOutput->setText(output);
                return;
            }
            if (err.toLower().contains("error")) {
                output = "<b>ERROR</b><br>"+err;
                ui->leOutput->setText(output);
                return;
            }
            output+=err;// + processCompress.readAllStandardOutput();
  //          qDebug() << std;
            pf.packedSize = QFile(pf.cFile).size();

    }
    output +="Compression done!";
    ui->leOutput->setText(output);
    // Create include file
    CreateIncludefile();
}
*/
void FormPaw::CreateIncludefile()
{

}

void FormPaw::on_pushButton_clicked()
{
    int r =ui->tabData->rowCount();
    ui->tabData->insertRow(r);
    ui->tabData->setItem(r,0,new QTableWidgetItem(""));
    ui->tabData->setItem(r,1,new QTableWidgetItem(""));
    ui->tabData->setItem(r,2,new QTableWidgetItem(""));
    ui->tabData->setItem(r,3,new QTableWidgetItem("0"));
}

void FormPaw::onTextUpdate()
{
    ui->leOutput->setText(pt->output);
}

void FormPaw::on_pushButton_2_clicked()
{
    ui->tabData->removeRow(ui->tabData->currentRow());
}

void FormPaw::Clear()
{
    ui->leOutput->setText("");
}

void FormPaw::on_lePackedAddress_cursorPositionChanged(int arg1, int arg2)
{
    Clear();
}

void FormPaw::on_leOutDir_cursorPositionChanged(int arg1, int arg2)
{
    Clear();
}

void FormPaw::on_leOutfile_cursorPositionChanged(int arg1, int arg2)
{
    Clear();
}

void FormPaw::on_tabData_cellChanged(int row, int column)
{
    Clear();
}

void PawThread::CreateIncludefile()
{
    bool twoFiles = m_pawData->getString("ras_include_file").contains(';');

    if (!twoFiles) {

        QString incFile = m_projectData->getString("project_path")+"/" +m_pawData->getString("ras_include_file");
        output +="Include file written to :" + incFile + "\n";
        emit EmitTextUpdate();

        if (QFile::exists(incFile))
            QFile::remove(incFile);

        QFile file(incFile);
        file.open(QIODevice::ReadWrite);
        QTextStream stream(&file);

        int address = Util::NumberFromStringHex(m_pawData->getString("packed_address"));
        QVector<int> addresses;
        for (PawFile& pf : *m_files) {
            QString s = pf.name + " : IncBin(\""+pf.incCFile+"\", " + Util::numToHex(address) + ");\n";
            address+=pf.packedSize;
            addresses.append(address);
            stream<<s;
        }
        QString adr = m_pawData->getString("address_field") + " : array [" + QString::number(m_files->count()*2) +"] of byte = (\n";
        for (int i: addresses){
            adr+="\t"+QString::number(i&255) + ", ";
            adr+=QString::number((i>>8)&255);
            if (i!=addresses[addresses.count()-1])
                adr+=",";
            adr+="\n";
        }

        adr = adr+");";
        stream << adr;
        file.close();

    }
    else
    {

        QStringList fNames = m_pawData->getString("ras_include_file").split(';');

        QString incFile1 = m_projectData->getString("project_path")+"/" +fNames[0];
        QString incFile2 = m_projectData->getString("project_path")+"/" +fNames[1];

        output +="Include BIN file written to :" + incFile1 + "\n";
        output +="Include ARRAY file written to :" + incFile2 + "\n";
        emit EmitTextUpdate();

        if (QFile::exists(incFile1))
            QFile::remove(incFile1);
        if (QFile::exists(incFile2))
            QFile::remove(incFile2);

        QFile file1(incFile1);
        file1.open(QIODevice::ReadWrite);
        QTextStream stream1(&file1);

        QFile file2(incFile2);
        file2.open(QIODevice::ReadWrite);
        QTextStream stream2(&file2);

        int address = Util::NumberFromStringHex(m_pawData->getString("packed_address"));
        QVector<int> addresses;
        for (PawFile& pf : *m_files) {
            QString s = pf.name + " : IncBin(\""+pf.incCFile+"\", " + Util::numToHex(address) + ");\n";
            address+=pf.packedSize;
            addresses.append(address);
            stream1<<s;
        }

        QString adr = m_pawData->getString("address_field") + " : array [" + QString::number(m_files->count()*2) +"] of byte = (\n";
        for (int i: addresses){
            adr+="\t"+QString::number(i&255) + ", ";
            adr+=QString::number((i>>8)&255);
            if (i!=addresses[addresses.count()-1])
                adr+=",";
            adr+="\n";
        }

        adr = adr+");";
        stream2 << adr;

        file1.close();
        file2.close();

    }

}

void PawThread::run()
{
    QString ex = m_iniFile->getString("exomizer");
    QString tt = m_iniFile->getString("tinycrunch");



    for (PawFile& pf: *m_files) {
            QProcess processCompress;
            QString adr = pf.address;
            QString of = pf.inFile+"@" + adr.replace("$","0x");
            QStringList params = QStringList() << "mem" << "-lnone" <<of << "-o" << pf.cFile;
            if (isExomizer3!="")
                params << isExomizer3;

            if (pf.tinyCrunch) {
                params = QStringList() << tt << "--inPlace" << pf.inFile << pf.cFile;
                output+="Compressing :"+ pf.inFile + "\n";
                emit EmitTextUpdate();

                //ui->leOutput->setText(output);

                processCompress.start("python", params  );

            }
            else {
                output+="Compressing :"+ pf.inFile + "\n";
                emit EmitTextUpdate();

                //ui->leOutput->setText(output);

                processCompress.start(ex, params  );

            }

            processCompress.waitForFinished();
            QString err(processCompress.readAllStandardError());
            QString std = processCompress.readAllStandardOutput();

            qDebug() << err << std;

            if (std.toLower().contains("error")) {
                output = "<b>ERROR</b><br>"+std;
                emit EmitTextUpdate();
                //ui->leOutput->setText(output);
                return;
            }
            if (err.toLower().contains("error")) {
                output = "<b>ERROR</b><br>"+err;
                //ui->leOutput->setText(output);
                emit EmitTextUpdate();
                return;
            }
            output+=err;// + processCompress.readAllStandardOutput();
  //          qDebug() << std;
            pf.packedSize = QFile(pf.cFile).size();

    }
    if (!m_ignoreIncludefile)
        CreateIncludefile();
    output+="All done!\n";
    emit EmitTextUpdate();
}
