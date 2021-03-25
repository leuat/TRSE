#include "dialogtrseinstrumenteditor.h"
#include "ui_dialogtrseinstrumenteditor.h"

DialogTRSEInstrumentEditor::DialogTRSEInstrumentEditor(TTRPlayer* player,int instr,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogTRSEInstrumentEditor)
{
    ui->setupUi(this);
    m_player = player;
    m_currentInstrument = instr;
    UpdateList();
}

DialogTRSEInstrumentEditor::~DialogTRSEInstrumentEditor()
{
    delete ui;
}

void DialogTRSEInstrumentEditor::FillToGUI()
{

    auto cur =  m_player->m_instruments.m_instruments[m_currentInstrument];
    ui->leName->setText(cur->name);

    ui->sldAttack->setValue(cur->a*scale);
    ui->sldDecay->setValue(cur->d*scale);
    ui->sldRelease->setValue(cur->r*scale);
    ui->sldSustain->setValue(cur->s*scale);


    ui->sldAttack_2->setValue(cur->w0*scale2);
    ui->sldDecay_2->setValue(cur->w1*scale2);
    ui->sldRelease_2->setValue(cur->w2*scale2);
    ui->sldSustain_2->setValue(cur->w3*scale2);

}

void DialogTRSEInstrumentEditor::FillFromGUI()
{
    auto cur =  m_player->m_instruments.m_instruments[m_currentInstrument];
    cur->a = ui->sldAttack->value()/scale;
    cur->d = ui->sldDecay->value()/scale;
    cur->s = ui->sldSustain->value()/scale;
    cur->r = ui->sldRelease->value()/scale;
    cur->w0 = ui->sldAttack_2->value()/scale2;
    cur->w1 = ui->sldDecay_2->value()/scale2;
    cur->w2 = ui->sldSustain_2->value()/scale2;
    cur->w3 = ui->sldRelease_2->value()/scale2;
    cur->name = ui->leName->text();
}

void DialogTRSEInstrumentEditor::PlayNote()
{
    FillFromGUI();
    if (m_player->m_isPlaying)
        return;
    auto cur =  m_player->m_instruments.m_instruments[m_currentInstrument];
    m_player->PlayNote(0,48,256,cur);
   // void TTRPlayer::PlayNote(int channel, int midi_note, int velocity, QSharedPointer<TRSEInstrument> ins)
}

void DialogTRSEInstrumentEditor::on_cmbInstruments_currentIndexChanged(int index)
{
    if (index==-1)
        return;
    m_currentInstrument = index;
    FillToGUI();
}

void DialogTRSEInstrumentEditor::on_pushButton_2_clicked()
{
    PlayNote();
}

void DialogTRSEInstrumentEditor::UpdateList()
{
    int j = m_currentInstrument;
    m_currentInstrument = -1;
    QComboBox* cmb;
    cmb = ui->cmbInstruments;
    cmb->clear();
    for (int i=0;i<m_player->m_instruments.m_instruments.count();i++) {
        cmb->addItem(m_player->m_instruments.m_instruments[i]->name);
    }
    m_currentInstrument = j;
    if (m_currentInstrument==-1)
        m_currentInstrument=0;
    if (m_currentInstrument!=-1)
        cmb->setCurrentIndex(m_currentInstrument);

}


void DialogTRSEInstrumentEditor::on_sldAttack_sliderReleased()
{
//    PlayNote();
}

void DialogTRSEInstrumentEditor::on_sldAttack_sliderMoved(int position)
{
    PlayNote();
}

void DialogTRSEInstrumentEditor::on_sldDecay_sliderMoved(int position)
{
    PlayNote();
}

void DialogTRSEInstrumentEditor::on_sldRelease_sliderMoved(int position)
{
    PlayNote();
}

void DialogTRSEInstrumentEditor::on_sldSustain_sliderMoved(int position)
{
    PlayNote();
}

void DialogTRSEInstrumentEditor::on_sldAttack_2_sliderMoved(int position)
{
    PlayNote();
}

void DialogTRSEInstrumentEditor::on_sldSustain_2_sliderMoved(int position)
{
    PlayNote();

}

void DialogTRSEInstrumentEditor::on_sldDecay_2_sliderMoved(int position)
{
    PlayNote();
}

void DialogTRSEInstrumentEditor::on_sldRelease_2_sliderMoved(int position)
{
    PlayNote();
}
