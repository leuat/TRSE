#include "formrtf.h"
#include "ui_formrtf.h"

FormRTF::FormRTF(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormRTF)
{
    ui->setupUi(this);
}

FormRTF::~FormRTF()
{
    delete ui;
}

bool FormRTF::Load(QString filename)
{
    QFile file(filename);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QString text = file.readAll();
        text = text.replace("@path;",m_currentDir);
        ui->txt->setHtml(text);
    }
    file.close();
    return true;
}

void FormRTF::InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pro)
{
    TRSEDocument::InitDocument(t,ini, pro);
}
