#include "dialogtrsesettings.h"
#include "ui_dialogtrsesettings.h"
#include <QFileDialog>
#include <QStringList>
#include <QDir>


DialogTRSESettings::DialogTRSESettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTRSESettings)
{
    ui->setupUi(this);
}

void DialogTRSESettings::FillFromIni()
{
    ui->leDasm->setText(m_ini->getString("dasm"));
    ui->leEmulator->setText(m_ini->getString("emulator"));
    ui->leExomizer->setText(m_ini->getString("exomizer"));
    ui->leZeropages->setText(  fromStringList(m_ini->getStringList("zeropages")));

    QDir directory("themes/");
    QStringList themes = directory.entryList(QStringList() << "*.ini");
    ui->cmbTheme->clear();
    QStringList washed;
    for (QString s: themes) {
        QString name = s.split(".")[0];
      //  qDebug() << name;
        washed << name;
    }
    ui->cmbTheme->addItems(washed);
    ui->cmbTheme->setCurrentText(m_ini->getString("theme").split(".")[0]);

    ui->leFontSize->setText(QString::number((int)m_ini->getdouble("font_size")));
    ui->leTabWidth->setText(QString::number((int)m_ini->getdouble("tab_width")));

}


void DialogTRSESettings::FillToIni()
{
    m_ini->setString("dasm", ui->leDasm->text());
    m_ini->setString("exomizer", ui->leExomizer->text());
    m_ini->setString("emulator", ui->leEmulator->text());
    m_ini->setString("theme", ui->cmbTheme->currentText() + ".ini");

    m_ini->setFloat("font_size", ui->leFontSize->text().toInt());
    m_ini->setFloat("tab_width", ui->leTabWidth->text().toInt());
    m_ini->setStringList("zeropages", toStringList(ui->leZeropages->text()));

}

DialogTRSESettings::~DialogTRSESettings()
{
    delete ui;
}

void DialogTRSESettings::on_pushButton_clicked()
{
    FillToIni();
    m_ini->Save();
    close();
}

void DialogTRSESettings::on_btnEmulator_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Emulator location"), m_ini->getString("project_path"), "*.exe");
    ui->leEmulator->setText(filename);

}

void DialogTRSESettings::on_btnExomizer_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Exomizer location"), m_ini->getString("project_path"), "*.exe");
    ui->leExomizer->setText(filename);

}

void DialogTRSESettings::on_btnDasm_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,
        tr("Dasm location"), m_ini->getString("project_path"), "*.exe");
    ui->leDasm->setText(filename);

}
