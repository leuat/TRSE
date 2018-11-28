#include "dialogeffects.h"
#include "ui_dialogeffects.h"

DialogEffects::DialogEffects(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogEffects)
{
    ui->setupUi(this);

    Create();
}

DialogEffects::~DialogEffects()
{

    Abort();

    delete ui;
}

void DialogEffects::Abort()
{
    if (m_effect!=nullptr) {
        m_effect->m_abort = true;
        m_effect->quit();
        m_effect->wait();
        m_effect->terminate();
        m_effect->wait();
        m_effect->deleteLater();
//        delete m_effect;
    }

}

void DialogEffects::Create()
{
    if (m_effect!=nullptr)
        delete m_effect;

    if (ui->comboBox->currentText()=="Twister") {
        m_effect = new DemoEffectTwister(ui->glParams);
    }
    connect(m_effect,SIGNAL(SignalImageUpdate()),this,SLOT(UpdateImage()));

    m_effect->FillToGUI();
    m_effect->start();
}

void DialogEffects::UpdateImage()
{
    if (m_effect==nullptr)
        return;
    QPixmap p = QPixmap::fromImage(m_effect->m_img);
    ui->lblImage->setPixmap(p);




}

void DialogEffects::on_pushButton_clicked()
{
    Abort();
    close();
}

void DialogEffects::on_btnToggleAnim_clicked()
{
    if (m_effect==nullptr)
        return;
    m_effect->m_toggleAnim=!m_effect->m_toggleAnim;
}

void DialogEffects::on_btnToggleC64_clicked()
{
    if (m_effect==nullptr)
        return;
    m_effect->m_toggleC64=!m_effect->m_toggleC64;

}

void DialogEffects::on_btnSave_clicked()
{
    if (m_effect==nullptr)
        return;
    bool c64=m_effect->m_toggleC64;
    bool anim=m_effect->m_toggleAnim;
    m_effect->m_toggleC64 = true;
    m_effect->m_toggleAnim = false;

    m_effect->Save("twister.bin");

    m_effect->m_toggleC64 = c64;
    m_effect->m_toggleAnim = anim;

}
