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

void FormPaw::Build()
{
    FillToIni();
    ui->leOutput->setText("");
    output="";
    QString ex = m_iniFile->getString("exomizer");
 //   QString isExomizer3 = "-P0";
    QString isExomizer3 = "";

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
            output+="Compressing :"+ pf.inFile + "\n";
            ui->leOutput->setText(output);

            processCompress.start(ex, params  );
            processCompress.waitForFinished();
            QString std(processCompress.readAllStandardError());
//            output+=std + processCompress.readAllStandardError();
  //          qDebug() << std;
            pf.packedSize = QFile(pf.cFile).size();

    }
    output +="Compression done!";
    ui->leOutput->setText(output);
    // Create include file
    CreateIncludefile();
}

void FormPaw::CreateIncludefile()
{
    QString incFile = m_projectIniFile->getString("project_path")+"/" +m_pawData.getString("ras_include_file");
    output +="Include file written to :" + incFile;
    ui->leOutput->setText(output);



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
    int address = Util::NumberFromStringHex(m_pawData.getString("packed_address"));
    for (PawFile& pf : m_files) {
        QString s = pf.name + " : IncBin(\""+pf.incCFile+"\", " + Util::numToHex(address) + ");\n";
        address+=pf.packedSize;
        stream<<s;
    }

    file.close();

}

void FormPaw::on_pushButton_clicked()
{
    int r =ui->tabData->rowCount();
    ui->tabData->insertRow(r);
    ui->tabData->setItem(r,0,new QTableWidgetItem(""));
    ui->tabData->setItem(r,1,new QTableWidgetItem(""));
    ui->tabData->setItem(r,2,new QTableWidgetItem(""));
}

void FormPaw::on_pushButton_2_clicked()
{
    ui->tabData->removeRow(ui->tabData->currentRow());
}
