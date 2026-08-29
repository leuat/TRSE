#include "dialognew8bplproject.h"
#include "ui_dialognew8bplproject.h"
#include <QProcess>
#include <QFile>

DialogNew8bplProject::DialogNew8bplProject(QString plab, QString path, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogNew8bplProject)
{
    ui->setupUi(this);
    m_path = path;
    m_8bplLoc = plab;
    m_values <<
        "VIC-20 Multicolour Character Mode"<<    "vic20-multicolor" <<
        "VIC-20 HFLI Picture (168x160, 8x2 colour)"  <<  "hfli" <<
                "C64 Multicolour Character Mode"<<    "c64-multicolor" <<
                "C64 Standard Hi-Res Character Mode" <<   "c64-hires"<<
                "C64 Extended Colour Mode (ECM)" <<   "c64-ecm"<<
                "C64 Multicolour Bitmap Mode" <<   "c64-multicolor-bitmap"<<
                "C64 Hi-Res Bitmap Mode" <<   "c64-hires-bitmap"<<
                "C64 Sprites" <<   "c64-sprite"<<
                "ZX Spectrum 48K Screen Mode" <<   "zx-spectrum-48k"<<
                "Amstrad CPC Mode 0 Screen Mode" <<   "amstrad-cpc-mode0"<<
                "Amstrad CPC Mode 1 Screen Mode" <<   "amstrad-cpc-mode1"<<
                "Amstrad CPC Mode 2 Screen Mode" <<   "amstrad-cpc-mode2"<<
                "Acorn Electron MODE 0 Screen Mode" <<   "acorn-electron-mode0"<<
                "Acorn Electron MODE 1 Screen Mode" <<   "acorn-electron-mode1"<<
                "Acorn Electron MODE 2 Screen Mode" <<   "acorn-electron-mode2"<<
                "Acorn Electron MODE 4 Screen Mode" <<   "acorn-electron-mode4"<<
        "Acorn Electron MODE 5 Screen Mode" <<   "acorn-electron-mode5";

    for (int i=0;i<m_values.count()/2;i++) {
        ui->cmbValues->addItem(m_values[i*2]);
    }
}

DialogNew8bplProject::~DialogNew8bplProject()
{
    delete ui;
}

void DialogNew8bplProject::on_buttonBox_accepted()
{
    QString fname = m_path + "/" + ui->leName->text() +".vicproj";
    if (QFile::exists(fname)) {
        ui->lblError->setText("Error: this project already exist, please select another name");
        return;
    }

    QProcess p;
    p.startDetached(m_8bplLoc,QStringList()<<"--new" << "--machine" << m_values[ui->cmbValues->currentIndex()*2+1]<<fname);
    close();
}


void DialogNew8bplProject::on_buttonBox_rejected()
{
    close();
}

