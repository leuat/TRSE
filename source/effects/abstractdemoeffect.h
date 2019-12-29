#ifndef ABSTRACTDEMOEFFECT_H
#define ABSTRACTDEMOEFFECT_H

#include <QImage>
#include <QThread>
#include <QMap>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QDebug>
#include "source/LeLib/limage/charsetimage.h"
#include <QElapsedTimer>

class DemoEffectParam {
public:
    QString m_name;
    float m_val;
    DemoEffectParam(QString name, float v) {
        m_val = v;
        m_name = name;
    }
    DemoEffectParam() {

    }
};

class AbstractDemoEffect : public QThread
{
    Q_OBJECT
public:
    QImage m_img;
    QPixmap m_pixmap;
    float m_time=0;
    bool m_ready = true;
    QString m_infoText;
    bool m_toggleAnim = true;
    int m_outputType = 0;
    QGridLayout* m_gl = nullptr;
    AbstractDemoEffect(QGridLayout* gl);
    bool m_abort = false;
    QMap<QString,DemoEffectParam> m_params;
    //CharsetImage* m_mc;
    LImage* m_mc = nullptr;
    QVector<int> m_cols;
//    int m_elapsedTime=0;
    QElapsedTimer m_timer;
    int m_elapsedTime;


    virtual void ToggleAnim() {m_toggleAnim=!m_toggleAnim;}

    virtual void Initialize() {}

    virtual void Render(QImage& img) {}

    virtual void SetParameters(int preset) {}
    virtual void Save(QString file);

    virtual void Init() {}
//    virtual void

    void ConvertToC64(int dither, bool isMulticolor, QVector3D ditherStrength)
    {
        if (m_outputType!=1)
            return;
        m_mc->setMultiColor(isMulticolor);
        if (m_cols.count()>=4) {
            m_mc->SetColor(m_cols[0],0);
            m_mc->SetColor(m_cols[1],1);
            m_mc->SetColor(m_cols[3],2);
            m_mc->SetColor(m_cols[2],3);
        }
        m_mc->m_colorList.EnableColors(m_cols);
        if (dither==1)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true);
        if (dither==2)
            m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength);
        if (dither==0)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false);

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

    }

    void ConvertToP8(int dither, QVector3D ditherStrength)
    {
/*        if (m_outputType!=1)
            return;*/
        m_mc->m_colorList.EnableColors(m_cols);
        if (dither==1)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true);
        if (dither==2)
            m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength);
        if (dither==0)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false);

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

    }


    void FillToGUI();
signals:
    void SignalImageUpdate();
public slots:
    void FillFromGUI();

};

#endif // ABSTRACTDEMOEFFECT_H
