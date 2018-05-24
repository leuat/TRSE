#ifndef DIALOGIMPORT_H
#define DIALOGIMPORT_H


#include "imageworker.h"
#include <QDialog>

namespace Ui {
class DialogImport;
}

class DialogImport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogImport(QWidget *parent = 0);
    ~DialogImport();
    void Initialize(LImage::Type imageType, LColorList::Type colorType);


    float m_contrast = 1;
    float m_shift = 0;
    float m_hsv = 0.5;
    float m_saturation = 0.5;
    float m_blur = 0;
    QPointF m_scale = QPointF(1,1);


    LImageQImage m_input;
    LImageQImage m_work;
    LImageQImage m_output;
    LImage* m_image = nullptr;


    LImage::Type m_imageType = LImage::Type::QImageBitmap;

    void Convert();
    void Blur();


    bool m_ok = false;


private slots:
    void on_hsContrast_sliderMoved(int position);

    void on_hsShift_sliderMoved(int position);

    void on_hsHsv_sliderMoved(int position);

    void on_hsSat_sliderMoved(int position);

    void on_hsBlur_sliderMoved(int position);

    void on_cmbForeground_activated(int index);

    void on_cmbBackground_activated(int index);




private slots:
    void UpdateOutput();

    void on_btnImport_clicked();

    void slotOk();



private:
    Ui::DialogImport *ui;
};

#endif // DIALOGIMPORT_H
