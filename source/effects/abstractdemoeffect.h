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
    bool m_toggleC64 = false;
    QGridLayout* m_gl = nullptr;
    AbstractDemoEffect(QGridLayout* gl);
    bool m_abort = false;
    QMap<QString,DemoEffectParam> m_params;
    CharsetImage* m_mc;
    QVector<int> m_cols;
//    int m_elapsedTime=0;
    QElapsedTimer m_timer;
    int m_elapsedTime;
    void SaveCharset(QString filename, int w, int h);
    void AddScreen(QByteArray& data, int w, int h, char base, int div, char add1, char add2);

    int Compare(QByteArray& a, QByteArray& b, int p1, int p2, int length);

    void OptimizeAndPackCharsetData(QByteArray& dataIn, QByteArray& out, QByteArray& table, int width, int compression);


    virtual void ToggleAnim() {m_toggleAnim=!m_toggleAnim;}

    virtual void Initialize() {}

    virtual void Render(QImage& img) {}

    virtual void SetParameters(int preset) {}
    virtual void Save(QString file);

    virtual void Init() {}
//    virtual void

    void ConvertToC64(bool dither);

    void FillToGUI();
signals:
    void SignalImageUpdate();
public slots:
    void FillFromGUI();

};

#endif // ABSTRACTDEMOEFFECT_H
