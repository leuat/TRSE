#ifndef DIALOGTRSEINSTRUMENTEDITOR_H
#define DIALOGTRSEINSTRUMENTEDITOR_H

#include <QDialog>
#include "ttrplayer.h"

namespace Ui {
class DialogTRSEInstrumentEditor;
}

class DialogTRSEInstrumentEditor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogTRSEInstrumentEditor(TTRPlayer* player,int instr, QWidget *parent = nullptr);
    ~DialogTRSEInstrumentEditor();
    TTRPlayer* m_player;
    int m_currentInstrument=-1;
    float const scale = 50.0;
    float const scale2 = 50.0;
private slots:
    void on_cmbInstruments_currentIndexChanged(int index);

    void on_pushButton_2_clicked();


    void on_sldAttack_sliderReleased();

    void on_sldAttack_sliderMoved(int position);

    void on_sldDecay_sliderMoved(int position);

    void on_sldRelease_sliderMoved(int position);

    void on_sldSustain_sliderMoved(int position);

    void on_sldAttack_2_sliderMoved(int position);

    void on_sldSustain_2_sliderMoved(int position);

    void on_sldDecay_2_sliderMoved(int position);

    void on_sldRelease_2_sliderMoved(int position);

    void on_pushButton_clicked();

    void on_btnNew_clicked();

private:
    Ui::DialogTRSEInstrumentEditor *ui;

    void UpdateList();
    void FillToGUI();
    void FillFromGUI();
    void PlayNote();

};

#endif // DIALOGTRSEINSTRUMENTEDITOR_H
