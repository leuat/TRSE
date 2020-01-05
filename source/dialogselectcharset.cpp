#include "dialogselectcharset.h"
#include "ui_dialogselectcharset.h"

DialogSelectCharset::DialogSelectCharset(CharsetImage* charset, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSelectCharset)
{
    ui->setupUi(this);
    m_charset = charset;
    UpdateCharSet();
    setFocusPolicy(Qt::StrongFocus);
}

DialogSelectCharset::~DialogSelectCharset()
{
    delete ui;
}

void DialogSelectCharset::keyPressEvent(QKeyEvent *e)
{
      QDialog::keyPressEvent(e);
}

void DialogSelectCharset::UpdateCharSet()
{
    if (m_charset==nullptr)
        return;

    int size=32;


    QVector<QPixmap> maps;
    m_charset->ToQPixMaps(maps);
    CharsetImage* charmap = m_charset;



    int width = charmap->m_charWidthDisplay;
    //   ui->lstCharMap->setViewMode(QListView::IconMode);
    ui->lstCharMap->setColumnCount(width);
    ui->lstCharMap->setRowCount(1+maps.count()/width);
    int cnt=0;
    int j=0;
    int i=0;

    ui->lstCharMap->setIconSize(QSize(size,size));
    QStringList lst;
    for (int i=0;i<width;i++)
        lst<<"";

    ui->lstCharMap->setHorizontalHeaderLabels(lst);
    int kk= 0;

    ui->lstCharMap->horizontalHeader()->setMinimumSectionSize(1);
    ui->lstCharMap->verticalHeader()->setMinimumSectionSize(1);
    for (int i=0;i<ui->lstCharMap->rowCount();i++) {
         ui->lstCharMap->setRowHeight(i,size);
        //ui->lstCharMap->setCol
    }
    for (int i=0;i<width;i++) {
        ui->lstCharMap->setColumnWidth(i,size);
        //ui->lstCharMap->setCol

    }


    for (int k=0;k<maps.count();k++) {

        QPixmap q = maps[k];

        QTableWidgetItem *itm = ui->lstCharMap->item(j,i);
        if (itm == nullptr) {
            itm = new QTableWidgetItem(q,nullptr,Qt::DisplayRole);
            ui->lstCharMap->setItem(j,i,itm);

        }
        itm->setData(Qt::UserRole, kk);
        cnt++;
        i++;
        kk++;
        if (i>=width) {
            i=0;
            j++;

        }
    }
    ui->lstCharMap->setShowGrid(false);
    ui->lstCharMap->verticalHeader()->setVisible(false);
    m_start=true;
}

void DialogSelectCharset::on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    if (!m_start)
        return;
    m_char = current->data(Qt::UserRole).toInt();
    if (m_char==-1)
        return;
    setResult(QDialog::Accepted);
    accept();
}
