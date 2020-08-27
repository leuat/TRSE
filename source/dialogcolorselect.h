#ifndef DIALOGCOLORSELECT_H
#define DIALOGCOLORSELECT_H

#include <QDialog>
#include <QVector3D>
#include <math.h>
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/lcolor.h"

namespace Ui {
class DialogColorSelect;
}

class DialogColorSelect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogColorSelect(QWidget *parent = nullptr);
    ~DialogColorSelect();
    bool m_ok = true;
    QVector3D m_bpp;
    LColor m_color;
    int m_interval=1;
    void Init(LColor color, QVector3D bpp);
    int forceInterval(int val);
private slots:
    void UpdateGUI(QWidget* except);
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();

    void on_slRed_sliderMoved(int position);

    void on_slGreen_sliderMoved(int position);


    void on_slBlue_sliderMoved(int position);

    void on_leValue_textChanged(const QString &arg1);

    void on_leValue_editingFinished();

private:
    Ui::DialogColorSelect *ui;
};

#endif // DIALOGCOLORSELECT_H
