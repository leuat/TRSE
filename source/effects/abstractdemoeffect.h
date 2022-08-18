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
    QImage m_img, m_post, m_prev;
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
    QVector<int> alreadySet;
    QByteArray m_background, m_backgroundUsed, m_backgroundTarget;
;
    bool m_backgroundSet = false;

    char FadeColor(char c, int speed);
    char FadeColorTo(char c, char tar, int speed, int start = 0);

    virtual void ToggleAnim() {m_toggleAnim=!m_toggleAnim;}

    virtual void Initialize() {}

    virtual void Render(QImage& img) {}

    virtual void SetParameters(int preset) {}
    virtual void Save(QString file);


    void UpdateScreenDataFrame(QVector<int>& screen, int xp, int yp, int w, int h, int frame);

    void SaveImageBin(QString fname);
    void ExportDiffAsUnrolledBitmap6502ColorOut(QString file, QString background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h, int speed, int startCol);
    void ExportDiffAsUnrolledBitmap6502ColorIn(QString file, QString background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h, int speed, int startCol);
    void ExportDiffAsUnrolledBitmap6502ColorInAddress(QString file, int background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h, int speed);
    void ExportDiffAsUnrolledBitmap6502(QString file, QString background, QString name, QString wf, int pos, int x, int y, int w, int h);
    void ExportDiffAsUnrolledBitmap6502In(QString file, QString background, QString name, QString wf, int pos, int x, int y, int w, int h);
    virtual void Init() {}
//    virtual void

    void ConvertToC64(int dither, bool isMulticolor, QVector3D ditherStrength);
    void ConvertToCharset(int dither, bool isMulticolor, QVector3D ditherStrength);




    void ConvertToP8(int dither, QVector3D ditherStrength)
    {
/*        if (m_outputType!=1)
            return;*/
        int size = 2;
        m_mc->m_colorList.EnableColors(m_cols);
//        qDebug() <<"COLS IN USE " <<m_cols;
        if (dither==1)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true,ditherStrength.x());
        if (dither==2)
            m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength,QPoint(1,size),1);
        if (dither==0)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false,ditherStrength.x());

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

    }

    void ConvertToCPC(int dither, QVector3D ditherStrength)
    {
/*        if (m_outputType!=1)
            return;*/
        int size = 2;
/*        for (int i=0;i<16;i++) {
            int pen = 0;
            if (i<m_cols.size())
                pen = m_cols[i];
            m_mc->m_colorList.setPen(i,pen);
        }*/
        m_mc->m_colorList.m_selectClosestFromPen = true;

        m_mc->m_colorList.EnableColors(m_cols);
//        qDebug() <<"COLS IN USE " <<m_cols;
        if (dither==1)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true,ditherStrength.x());
        if (dither==2)
            m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength,QPoint(1,size),1);
        if (dither==0)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false,ditherStrength.x());

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

    }

    void ConvertToBIN(int dither, QVector3D ditherStrength)
    {
/*        if (m_outputType!=1)
            return;*/
        int size = 2;
//        m_mc->m_colorList.m_selectClosestFromPen = false;

        m_mc->m_colorList.EnableColors(m_cols);
        if (dither==1)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true,ditherStrength.x());
        if (dither==2)
            m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength,QPoint(size,1),1.0);
        if (dither==0)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false,ditherStrength.x());

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

    }

    void ConvertToStandard(int dither, QVector3D ditherStrength)
    {
/*        if (m_outputType!=1)
            return;*/
//        m_mc->m_colorList.EnableColors(m_cols);
        if (dither==1)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true,ditherStrength.x());
        if (dither==2)
            m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength,QPoint(1,2),1);
        if (dither==0)
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false,ditherStrength.x());

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

    }


    void FillToGUI();
signals:
    void SignalImageUpdate();
public slots:
    void FillFromGUI();

};

#endif // ABSTRACTDEMOEFFECT_H
