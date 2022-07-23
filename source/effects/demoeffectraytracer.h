#ifndef DEMOEFFECTRAYTRACER_H
#define DEMOEFFECTRAYTRACER_H

#include "abstractdemoeffect.h"
#include "source/LeLib/util/cinifile.h"
#include "source/Raytracer/raytracer.h"
#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/luascript.h"
#include "source/LeLib/limage/limagegamboy.h"
#include "source/LeLib/limage/limageamstradgeneric.h"
#include "source/LeLib/limage/c64fullscreenchar.h"
#include "source/LeLib/limage/limagecga.h"
#include "source/LeLib/limage/limageamiga4.h"
#include "source/LeLib/limage/limagespectrum.h"
#include <QMatrix3x3>

class DemoEffectRaytracer : public AbstractDemoEffect
{
public:
    DemoEffectRaytracer();

//    CIniFile m_file;


    QByteArray data;

    RayTracer* m_rt = nullptr;
    int m_noFrames = 32;
    int m_curFrame = 0;
    int m_noChars = 28;
    int m_char = 1;
/*    int m_frameWidth = 4;
    int m_frameHeight = 4;
*/
  //  int m_frameWidth = 40;
    //int m_frameHeight = 25;

//    int m_frameWidth = 40;
  //  int m_frameHeight = 25;

/*    int m_frameWidth = 4;
    int m_frameHeight = 3;
*/

    int m_frameWidth = 40;
    int m_frameHeight = 25;


    LuaScript* m_script;

//    int m_frameWidth = 40;
  //  int m_frameHeight = 25;

    int m_count = 0;
    int m_nextFrameSave = 1;
    QByteArray m_charData;
    QByteArray m_screenData;

    int m_currentscene = 7;
//    int m_compression = 9; for moviestuff
    int m_compression = 1;
    float m_dither = true;
    int m_N = 6;

    void Initialize();



    void run() override;
    void Render(QImage& img) override;
    void Save(QString f) override;
    void Init() override;
    void AppendData();
    void ToggleAnim() override;

};

#endif // DEMOEFFECTRAYTRACER_H
