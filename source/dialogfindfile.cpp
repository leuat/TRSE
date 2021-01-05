#include "dialogfindfile.h"
#include "ui_dialogfindfile.h"
#include <QDebug>

DialogFindFile::DialogFindFile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogFindFile)
{
    ui->setupUi(this);
}

DialogFindFile::~DialogFindFile()
{
    delete ui;
}

void DialogFindFile::Init(QVector<QString> files)
{
    m_fullFile = files;
    m_shortFile = files;
    Update();
    ui->leSearch->setFocus();
    ui->lstItems->setCurrentRow(0);
}

void DialogFindFile::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Down) {
        if (cur<ui->lstItems->count()-1)
            cur++;
        ui->lstItems->setCurrentRow(cur);

    }
    if (e->key() == Qt::Key_Up) {
        if (cur>0)
            cur--;
        ui->lstItems->setCurrentRow(cur);
        qDebug() << "WOOT";

    }
    if (e->key() == Qt::Key_Escape) {
        m_selected="";
        close();
    }
}

void DialogFindFile::Update()
{
    ui->lstItems->clear();
    for (QString s: m_shortFile)
        if (s.contains(ui->leSearch->text())) {
            QListWidgetItem* i = new QListWidgetItem();
            i->setText(s);
            ui->lstItems->addItem(i);
        }

}

void DialogFindFile::on_leSearch_textChanged(const QString &arg1)
{
    Update();
    cur=0;
    ui->lstItems->setCurrentRow(cur);

}

void DialogFindFile::on_leSearch_returnPressed()
{
    m_selected = "";
    if (ui->lstItems->count()!=0)
        m_selected = ui->lstItems->item(cur)->text();
    close();
}

void DialogFindFile::on_lstItems_itemDoubleClicked(QListWidgetItem *item)
{
    m_selected = item->text();
    close();
}

void DialogFindFile::on_lstItems_itemPressed(QListWidgetItem *item)
{
 //   m_selected = item->text();
   // close();

}
