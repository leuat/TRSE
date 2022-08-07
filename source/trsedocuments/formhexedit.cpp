#include "formhexedit.h"
#include "ui_formhexedit.h"

FormHexEdit::FormHexEdit(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormHexEdit)
{
    ui->setupUi(this);
    m_programEndingType = "bin";
    m_fileExtension = "bin";

}

FormHexEdit::~FormHexEdit()
{
    delete ui;
}

bool FormHexEdit::Load(QString filename) {
    m_data = Util::loadBinaryFile(filename);
    m_rawFilename = filename;
    ui->scrollArea->setData(new DataStorageArray(m_data));
    return true;
}

void FormHexEdit::Save(QString filename)
{
    m_data = ui->scrollArea->m_pdata->m_data;
    Util::SaveByteArray(m_data,filename);

}

void FormHexEdit::Reload()
{
    if (!ui->scrollArea->m_isChanged)
        Load(m_rawFilename);
}

void FormHexEdit::InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) {
    TRSEDocument::InitDocument(t,ini,iniProject);
    ui->scrollArea->m_doc = this;
    //ui->sc

    m_colors.Load(Util::GetSystemPrefix() + "themes/" + m_iniFile->getString("theme"));
    ui->scrollArea->m_colors = &m_colors;
}


void FormHexEdit::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->scrollArea->BYTES_PER_LINE = arg1.toInt();
    ui->scrollArea->update();
    ui->scrollArea->viewport()->update();

}

void FormHexEdit::on_sldSize_valueChanged(int value)
{
    ui->scrollArea->m_size = 1.0 + (value/100.0);
//    qDebug() << ui->scrollArea->m_size << value;
    ui->scrollArea->update();
    ui->scrollArea->viewport()->update();
}

void FormHexEdit::on_btnExport_clicked()
{
    m_data = ui->scrollArea->m_pdata->m_data;
    int start = Util::NumberFromStringHex(ui->leStart->text());
    if (start<0) start = 0;
    int end = Util::NumberFromStringHex(ui->leEnd->text());
    if (end>m_data.length())
        end = m_data.length();
    int len = end-start;
    if (len<=0)
        return;
    QByteArray ba = m_data.mid(start,len);
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::AnyFile);
    QString f = "Binary files (*.*)";
    QString filename = dialog.getSaveFileName(NULL, "Save file as",m_currentDir,f);

    if (filename=="")
        return;

    Util::SaveByteArray(ba,filename);

}
