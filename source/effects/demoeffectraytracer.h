#ifndef DEMOEFFECTRAYTRACER_H
#define DEMOEFFECTRAYTRACER_H

#include "abstractdemoeffect.h"
#include "source/LeLib/util/cinifile.h"
#include "source/Raytracer/raytracer.h"
#include <QMatrix3x3>

class DemoEffectRaytracer : public AbstractDemoEffect
{
public:
    DemoEffectRaytracer(QGridLayout* gl);

//    CIniFile m_file;

    RayTracer m_rt;

    void run() override;
    void Save(QString f) override;
    void Init() override;
    void SetParameters(int preset) override;

};

#endif // DEMOEFFECTRAYTRACER_H
