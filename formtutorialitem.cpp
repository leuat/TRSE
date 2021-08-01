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
    QString style="<html><head><style>"\
"img {"\
"  display: block;"\
"  margin-left: auto;"\
"  margin-right: auto;"\
"}"\
"h2 {"\
"  color: rgb(255,255,220);"\
"  margin-left: 0px;"\
"}"\
"</style></head>";
    ui->txtInfo->setText(style+description);
//    setMouseTracking(true);
//    ui->lblText->setText(description);
}

void FormTutorialItem::leaveEvent(QEvent *event)
{
    ui->txtInfo->setStyleSheet("background-color: rgb(30,30,30);");
}

void FormTutorialItem::enterEvent(QEnterEvent *event)
{
//    ui->txtInfo->setStyleSheet("background-color: red;");
    ui->txtInfo->setStyleSheet("background-color: rgb(40,40,40);;");

}

void FormTutorialItem::on_pushButton_clicked()
{
    emit emitLoadTutorialProject(m_tutorialFile);
}

