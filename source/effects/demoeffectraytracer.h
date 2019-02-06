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
    CharsetImage* m_charset;
    void Scene1();
    void Scene2();
    void Scene3();
    void Scene4();
    void Scene5();
    void Scene6();
    void UpdateScene1();
    void UpdateScene2();
    void UpdateScene3(); // Used in demo
    void UpdateScene4(); // Block
    void UpdateScene5(); // ??
    void UpdateScene6(); // Chars

    void Initialize();

    void CompressAndSaveCharset();
    void CompressAndSaveScreen();

    void run() override;
    void Save(QString f) override;
    void Init() override;
    void SetParameters(int preset) override;
    void AppendData();
    void ToggleAnim() override;

};

#endif // DEMOEFFECTRAYTRACER_H
