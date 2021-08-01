#include "formtutorialitem.h"
#include "ui_formtutorialitem.h"

FormTutorialItem::FormTutorialItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormTutorialItem)
{
    ui->setupUi(this);
}

FormTutorialItem::~FormTutorialItem()
{
    delete ui;
}

void FormTutorialItem::Initialise(QString name, QString description, QString image, QString file)
{
    m_tutorialFile = file;
/*    ui->lblName->setText(name);
    ui->lblDesc->setText(description);
    m_tutorialFile = file;
    ui->lblImage->setPixmap(QPixmap(file));*/
    ui->txtInfo->setText(description);
//    ui->lblText->setText(description);
}

void FormTutorialItem::on_pushButton_clicked()
{
    emit emitLoadTutorialProject(m_tutorialFile);
}

