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
