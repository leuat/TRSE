#include "dialogchip8.h"
#include "ui_dialogchip8.h"
#include <QTimer>
#include <QDebug>

dialogchip8::dialogchip8(QString file,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dialogchip8)
{
    ui->setupUi(this);
    c8.Init();
    c8.loadROM(file);

    timer = QSharedPointer<QTimer>(new QTimer());
    timer->setInterval(1); //Time in milliseconds
    //timer->setSingleShot(false); //Setting this to true makes the timer run only once
    connect(timer.get(), &QTimer::timeout, this, [=](){
        c8.Step();
        ui->lblScreen->setPixmap(c8.getScreen(ui->lblScreen->width(),ui->lblScreen->height()));
    });
    timer->start(); //Call start() AFTER connect
}

dialogchip8::~dialogchip8()
{
     delete ui;
}

void dialogchip8::stop()
{

}

void dialogchip8::on_pushButton_clicked()
{
    close();
}

