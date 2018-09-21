#include "formpaw.h"
#include <QProcess>
#include "ui_formpaw.h"

FormPaw::FormPaw(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormPaw)
{
    ui->setupUi(this);
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

    QStringList data;
    m_files.clear();

    for (int r=0;r<ui->tabData->rowCount();r++) {
        for (int c=0;c<ui->tabData->columnCount();c++)
            if (c!=2)
                data<< ui->tabData->item(r,c)->text();
            else
                data<< Util::numToHex(Util::VerifyHexAddress(ui->tabData->item(r,c)->text()));

        PawFile pf(m_projectIniFile->getString("project_path")+"/"+ui->tabData->item(r,1)->text(),
                   m_projectIniFile->getString("project_path")+"/"+  m_pawData.getString("output_dir")+"/" + ui->tabData->item(r,0)->text() + "_c.bin",

                   Util::numToHex(Util::VerifyHexAddress(ui->tabData->item(r,2)->text())),
                   ui->tabData->item(r,0)->text()
                   );
        pf.incCFile = m_pawData.getString("output_dir")+"/" + ui->tabData->item(r,0)->text() + "_c.bin";

        m_files.append(pf);
    }
    m_pawData.setStringList("data",data);

}

void FormPaw::FillFromIni()
{
    ui->leOutfile->setText(m_pawData.getString("ras_include_file"));
    ui->lePackedAddress->setText(m_pawData.getString("packed_address"));
    ui->leOutDir->setText(m_pawData.getString("output_dir"));
    ui->leAddressField->setText(m_pawData.getString("address_field"));
    ui->chkExomizer3->setChecked(m_pawData.getdouble("use_exomizer3")==1);
    QStringList data = m_pawData.getStringList("data");


    for (int r=0;r<data.count()/3;r++) {
        ui->tabData->insertRow(r);
        ui->tabData->setItem(r,0,new QTableWidgetItem(data[3*r+0]));
        ui->tabData->setItem(r,1,new QTableWidgetItem(data[3*r+1]));
        ui->tabData->setItem(r,2,new QTableWidgetItem(data[3*r+2]));
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
    QString incFile = m_projectData->getString("project_path")+"/" +m_pawData->getString("ras_include_file");
    output +="Include file written to :" + incFile + "\n";
    emit EmitTextUpdate();
 //   ui->leOutput->setText(output);



    if (QFile::exists(incFile))
        QFile::remove(incFile);

    QFile file(incFile);
    file.open(QIODevice::ReadWrite);
    QTextStream stream(&file);

/*    for d in data:
        s = "\t" + d[0] + " : IncBin(\""+d[3]+"\", " + hex(d[4]).replace("0x","$")+");"
        print s
        text_file.write(s+"\n")
  */
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

void PawThread::run()
{
    QString ex = m_iniFile->getString("exomizer");

    for (PawFile& pf: *m_files) {
            QProcess processCompress;
            QString adr = pf.address;
            QString of = pf.inFile+"@" + adr.replace("$","0x");
            QStringList params = QStringList() << "mem" << "-lnone" <<of << "-o" << pf.cFile;
            if (isExomizer3!="")
                params<<isExomizer3;

            output+="Compressing :"+ pf.inFile + "\n";
            emit EmitTextUpdate();

            //ui->leOutput->setText(output);

            processCompress.start(ex, params  );
            processCompress.waitForFinished();
            QString err(processCompress.readAllStandardError());
            QString std = processCompress.readAllStandardOutput();
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
    CreateIncludefile();
    output+="All done!\n";
    emit EmitTextUpdate();
}
