#ifndef DEMOEFFECTTWISTER_H
#define DEMOEFFECTTWISTER_H

#include <qmath.h>
#include "source/LeLib/util/util.h"
#include "abstractdemoeffect.h"
#include "source/LeLib/limage/multicolorimage.h"
#include "source/LeLib/limage/charsetimage.h"

class DemoEffectTwister : public AbstractDemoEffect
{
    Q_OBJECT

private:

    QVector<float> m_angles;
    int m_width = 18;
    int m_height = 8;
    int m_N = 4;
    int m_char = 1;
    //QVector<int> m_colors;

public:
    DemoEffectTwister(QGridLayout* gl) ;

    CharsetImage* m_charset ;
    void RenderBar(int, float, QVector3D, float maxy, float tt, float sscale, QVector<float> mask);
    void Render(QImage& img) override;

    void SetParameters(int preset) override;
    void SaveTwister(QString filename, int w, int h);
    void run() override;
    void Save(QString f) override;
    void Init() override;

};

#endif // DEMOEFFECTTWISTER_H
