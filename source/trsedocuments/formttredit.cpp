#include "formttredit.h"
#include "ui_formttredit.h"
#include "source/trsetracker/dialogtrseinstrumenteditor.h"

FormTTREdit::FormTTREdit(QWidget *parent) :
    TRSEDocument(parent),
    ui(new Ui::FormTTREdit)
{
    ui->setupUi(this);
    m_programEndingType = "trt";
    m_fileExtension = "trt";

    QComboBox* cmb = ui->cbmSystem;
    cmb->clear();
    for (int i=0;i<m_ttr.systemTypes.count();i++) {
        cmb->addItem(m_ttr.systemTypes[i]);
    }

}

FormTTREdit::~FormTTREdit()
{
    delete ui;
}

void FormTTREdit::SaveCurrentInstrument()
{
    if (m_currentInstrument!=-1) {
        QByteArray ba;
        QStringList lst = ui->leInstrumentData->text().split(",");
        for (QString& s:lst)
            s = "$"+s;
        ba.append(Util::HexQStringListToByteArray(lst));
//        qDebug() << lst << ba;
//        ba.resize(11);
        QString txt = ui->leInstrument->text();
        int cur = 0;
        while (ba.count()<16) {
            if (cur<txt.length())
                ba.append(txt[cur].toLatin1());
            else
                ba.append((uchar)0);
            cur++;
        }
        m_ttr.m_instruments[m_currentInstrument] = ba;
    }

}

void FormTTREdit::InitTRT(int channels, int rows)
{
    m_ttr.Initialize(channels, rows);
    FillGUIFromData();
    ReloadPatterns();

}

bool FormTTREdit::Load(QString filename) {
    m_ttr.Load(filename);
    m_rawFilename = filename;
    FillGUIFromData();
    ReloadPatterns();
    return true;
}

void FormTTREdit::Save(QString filename)
{
    ApplyCurrentOrder();
    m_ttr.Save(filename);

}

void FormTTREdit::Reload()
{
//    if (!ui->scrollArea->m_isChanged)
//        Load(m_rawFilename);
}

void FormTTREdit::InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) {
    TRSEDocument::InitDocument(t,ini,iniProject);
    //ui->scrollArea->m_doc = this;
    //ui->sc

    m_colors.Load(Util::GetSystemPrefix() + "themes/" + m_iniFile->getString("theme"));
    m_player.Initialize(&m_ttr);

}

void FormTTREdit::FillGUIFromData()
{
    ui->leText->setText(m_ttr.m_text);
    //ui->leInstruments->setText(Util::toString(Util::ByteArrayToHexQStringList(m_ttr.m_instruments)));
    //ui->leOrders->setText(Util::toString(Util::ByteArrayToHexQStringList(m_ttr.m_orders)));

    ReloadOrders();
    ReloadInstruments();

    LoadPredefinedInstruments();


    ReloadPatterns();

    ui->cbmSystem->setCurrentIndex(m_ttr.m_header[TTRFile::HEADER_POS_CURRENTTYPE]);

}
void FormTTREdit::LoadPredefinedInstruments()
{
    QStringList lst = Util::loadTextFile(":resources/text/adlib_instruments.txt").split("\n");
    for (QString s:lst) {
        if (s.trimmed()=="")
            continue;
        if (s.startsWith("#"))
            continue;
        QStringList d = s.split(" ");
        QString name = d[0];
/*        QStringList data = d[1].split(",");
        QByteArray ba = Util::HexQStringListToByteArray(data);
        qDebug() << ba;*/
        m_instruments[name] = d[1];

    }

    QComboBox* cmb = ui->cmbPredefinedInstrument;
    cmb->clear();
    for (QString s: m_instruments.keys())
        cmb->addItem(s);


}

void FormTTREdit::acceptSound(int channel, QByteArray sound)
{

    m_player.PlayNote(channel, sound);
}

void FormTTREdit::ReloadPatterns()
{
    Util::clearLayout(ui->lyTrackers);
    m_curPatterns.clear();
    m_curPatternValues.clear();
    ui->cmbPatterns->clear();
    for (int i=0;i<m_ttr.m_patterns.count();i++)
        ui->cmbPatterns->addItem(QString::number(i));

    QVector<QByteArray> curPatt = m_ttr.getCurrentPattern();



    for (int i=0;i<curPatt.count();i++) {
        WidgetPattern* wp = new WidgetPattern();
        //ui->scrollArea->m_colors = &m_colors;
        m_curPatterns.append(wp);
        ui->lyTrackers->addWidget(wp);
        wp->SetData(this,&m_colors, QSharedPointer<DataStorageArray>(
                        new DataStorageArray(curPatt[i])),m_ttr.m_orders[m_ttr.m_currentOrder][i],i);



        QComboBox* cmb = wp->getPatternCmb();
        cmb->clear();
        for (int i=0;i<m_ttr.m_patterns.count();i++)
            cmb->addItem(QString::number(i));

        cmb->setCurrentIndex((uchar)m_ttr.m_orders[m_ttr.m_currentOrder][i]);
        connect(wp, SIGNAL(emitReloadPatterns()), this, SLOT(UpdatePatterns()));
        connect(wp, SIGNAL(emitUpdatePatterns(WidgetPattern*, int)), this, SLOT(HandleUpdatePatterns(WidgetPattern*, int)));
        connect(wp, SIGNAL(emitMove(int,int,int)), this, SLOT(HandleMove(int,int,int)));
        connect(wp, SIGNAL(emitSound(int,QByteArray)), this, SLOT(acceptSound(int, QByteArray)));
        m_curPatternValues.append((uchar)m_ttr.m_orders[m_ttr.m_currentOrder][i]);

    }
    ui->grpOrder->setTitle("Orders : "+QString::number(m_ttr.m_orders.count()));
    ui->grpPatterns->setTitle("Patterns : "+QString::number(m_ttr.m_patterns.count()));
    //    ui->lyTrackers->re
}

void FormTTREdit::UpdatePatterns()
{
    ApplyCurrentOrder();
    // Update pattern index
    for (int i=0;i<m_curPatterns.count();i++) {
        m_ttr.m_orders[m_ttr.m_currentOrder][i] = m_curPatterns[i]->getPatternCmb()->currentIndex();
    }
    ReloadOrders();
    ReloadPatterns();
}

void FormTTREdit::HandleUpdatePatterns(WidgetPattern* wp, int pattern)
{
//    qDebug() << "Ready to handle pattern " << pattern;
    ApplyCurrentOrder();
    QByteArray data = wp->getData();
    for (auto w : m_curPatterns) {
        if (w!=wp)
            if (w->m_curPattern==pattern)
                w->UpdateData(data);
        w->RefreshAll();
    }
}

void FormTTREdit::HandleMove(int dir, int pos, int col)
{
//    qDebug() << "READY TO MOVE FROM "<<col;

    int i =(col+dir)%m_ttr.m_noChannels;
    if (i<0) i=m_ttr.m_noChannels-1;
    WidgetPattern* wp = m_curPatterns[i];
    //qDebug() << pos;
    wp->SetCursorPosition(pos);
    for (auto w: m_curPatterns)
        w->RefreshAll();
//    wp->Set
}

void FormTTREdit::ApplyCurrentOrder()
{
    SaveCurrentInstrument();
    m_ttr.m_text = ui->leText->toPlainText();
    if (m_curPatterns.count()==0)
        return;
    QByteArray order;
    for (int i=0;i<m_curPatterns.count();i++) {
//        int pat = m_curPatterns[i]->getPatternCmb()->currentIndex();
        int pat = m_curPatternValues[i];
        m_ttr.m_patterns[pat] = m_curPatterns[i]->getData();
        order.append(pat);
    }
    m_ttr.m_orders[m_ttr.m_currentOrder] = order;
}



void FormTTREdit::on_cmbPattern_currentIndexChanged(int index)
{
    if (!ignoreApplyPatterns)
        ApplyCurrentOrder();
    m_ttr.m_currentOrder = index;
    ReloadPatterns();
}

void FormTTREdit::on_cmbInstruments_currentIndexChanged(int index)
{

    // First, save old
    SaveCurrentInstrument();
    QString txt = "";
    if (index<0)
        return;
    for (int i=11;i<m_ttr.m_instruments[index].count();i++)
        txt+=QChar(m_ttr.m_instruments[index][i]);

/*    unsigned int d = ((uchar)m_ttr.m_instruments[index][0]) |
            ((uchar)m_ttr.m_instruments[index][1])<<8|
            ((uchar)m_ttr.m_instruments[index][2])<<16|
            ((uchar)m_ttr.m_instruments[index][3])<<24;
*/

    ui->leInstrument->setText(txt);
//    ui->leInstrumentData->setText(QString::number(d,16));
    QByteArray ba = m_ttr.m_instruments[index].mid(0,11);
    ui->leInstrumentData->setText(Util::toString(Util::ByteArrayToHexQStringList(ba)));
    m_currentInstrument = index;



    //qDebug() << index;
    int j = m_currentInstrument;
    m_currentInstrument = -1;
    QComboBox* cmb;
    cmb = ui->cmbTRSEInstrument;
    cmb->clear();
    for (int i=0;i<m_player.m_instruments.m_instruments.count();i++) {
        cmb->addItem(m_player.m_instruments.m_instruments[i]->name);
    }
    m_currentInstrument = j;
    if (m_currentInstrument!=-1)
        cmb->setCurrentIndex(m_ttr.getTRSEInstrument(m_currentInstrument));


}

void FormTTREdit::on_btnNewPattern_clicked()
{
    m_ttr.InsertPattern(m_ttr.m_patterns.count());
    ReloadPatterns();
}

void FormTTREdit::on_cmbPredefinedInstrument_currentIndexChanged(int index)
{




}

void FormTTREdit::on_btnSetInstrument_clicked()
{

//    QByteArray ba = m_ttr.getPredefinedInstrument(ui->cmbPredefinedInstrument->currentIndex());

    ui->leInstrumentData->setText(m_instruments[ui->cmbPredefinedInstrument->currentText()]);
    ui->leInstrument->setText(ui->cmbPredefinedInstrument->currentText());
}

void FormTTREdit::on_btnNewOrder_clicked()
{
    ApplyCurrentOrder();
    m_ttr.InsertOrder(m_ttr.m_orders.count());
    ignoreApplyPatterns = true;
    ReloadOrders();
    ignoreApplyPatterns = false;
}

void FormTTREdit::on_btnDeleteOrder_clicked()
{
    ApplyCurrentOrder();
    m_ttr.DeleteOrder();
    ignoreApplyPatterns = true;
    ReloadOrders();
    ignoreApplyPatterns = false;
}

void FormTTREdit::ReloadOrders()
{
    int keep = m_ttr.m_currentOrder;
/*    QComboBox* cmb = ui->cmbPattern;
    cmb->clear();
    for (int i=0;i<m_ttr.m_orders.count();i++) {
        cmb->addItem(QString::number(i));
    }
    if (keep!=-1)
        cmb->setCurrentIndex(keep);

    */
    QListWidget* lst = ui->lstOrders;
    lst->clear();
    for (int i=0;i<m_ttr.m_orders.count();i++) {
        QString s = QStringLiteral("%1").arg(i, 2, 10, QLatin1Char('0'));
        s+=": ";
        for (uchar b : m_ttr.m_orders[i])
            s+= QStringLiteral("%1").arg(b, 2, 10, QLatin1Char('0')) + " - ";

        s.remove(s.length()-3,3);
        lst->addItem(s);
    }
    if (keep!=-1)
        lst->setCurrentRow(keep);

}

void FormTTREdit::ReloadInstruments()
{

    QComboBox* cmb;
    cmb = ui->cmbInstruments;
    cmb->clear();
    for (int i=0;i<m_ttr.m_instruments.count();i++) {
        cmb->addItem(QString::number(i));
    }

    if (m_ttr.m_currentInstrument!=-1)
        cmb->setCurrentIndex(m_ttr.m_currentInstrument);





}
void FormTTREdit::on_cmbTRSEInstrument_currentIndexChanged(int index)
{
//    qDebug() << "BB" <<m_currentInstrument <<index;
    if (index<0)
        return;

    m_ttr.setTRSEInstrument(m_currentInstrument,index);
}

void FormTTREdit::on_btnDeletePattern_clicked()
{
    m_ttr.DeletePattern(ui->cmbPatterns->currentIndex());
    ReloadPatterns();

}

void FormTTREdit::on_btnNewOrder_2_clicked()
{
    m_ttr.AddInstrument();
    ReloadInstruments();
}

void FormTTREdit::on_cbmSystem_currentIndexChanged(int index)
{
    m_ttr.m_header[TTRFile::HEADER_POS_CURRENTTYPE] = (uchar)index;
}

void FormTTREdit::on_btnPlay_clicked()
{
    if (m_player.m_isPlaying) {
        m_player.Stop();
        ui->btnPlay->setText("Play");
    }
    else {
        m_player.m_curOrder = m_ttr.m_currentOrder;
        m_player.StartPlaying();
        ui->btnPlay->setText("Stop");
    }
}

void FormTTREdit::on_lstOrders_currentRowChanged(int currentRow)
{
    m_ttr.m_currentOrder = currentRow;
    ReloadPatterns();
}


void FormTTREdit::on_btnTRSEInstruments_clicked()
{
    DialogTRSEInstrumentEditor* d = new DialogTRSEInstrumentEditor(&m_player,m_ttr.getTRSEInstrument(m_currentInstrument));
    d->exec();
    ReloadInstruments();
    on_cmbInstruments_currentIndexChanged(ui->cmbTRSEInstrument->currentIndex());
}
