#include "demoeffectraytracer.h"

DemoEffectRaytracer::DemoEffectRaytracer() : AbstractDemoEffect (nullptr)
{


}


void DemoEffectRaytracer::Initialize()
{
    SetParameters(0);
    if (m_rt == nullptr)
        m_rt = new RayTracer();


   m_mc = new MultiColorImage(LColorList::C64);

   if (m_rt->m_globals.m_c64ImageType == 0.0) {
       delete m_mc;
       m_mc = new CharsetImage(LColorList::C64);

       ((CharsetImage*)(m_mc))->m_currentMode = CharsetImage::FULL_IMAGE;

   }
   if (m_rt->m_globals.m_outputType == RayTracerGlobals::output_type_pico8) {
       m_mc = new LImageQImage(LColorList::PICO8);
       m_mc->Initialize(m_rt->m_globals.m_width,m_rt->m_globals.m_height);

   }
   m_mc->setMultiColor(true);


   m_rt->m_globals.m_lights[0]->m_color = QVector3D(1,1,0.7);
   m_img = QImage(m_rt->m_globals.m_width,m_rt->m_globals.m_height,QImage::Format_ARGB32);
//   m_rt->m_objects.clear();

}








void DemoEffectRaytracer::run()
{

   Render(m_img);

}

void DemoEffectRaytracer::Render(QImage &img)
{
    while (!m_abort) {


    while (!m_ready) {
        msleep(10);
        if (m_abort)
            return;
    }

    m_ready = false;


    m_time+=1;
    // m_mc->Clear();
    //qDebug() << "FRAME0";

    Init();
    //qDebug() << "FRAME1";
//    m_timer = QElapsedTimer();
  //  m_timer.start();

    //qDebug() << "FRAME" << rand()%100;
//    if (m_rt==nullptr)
  //      qDebug() << "ISNULL";
    //return;
    int w = m_rt->m_globals.m_width;
    int h = m_rt->m_globals.m_height;

    m_rt->Raymarch(m_img, w,h);
    //qDebug() << "FRAME2";

    m_elapsedTime = m_timer.elapsed();
    m_outputType = m_rt->m_globals.m_outputType;
    if (m_outputType==RayTracerGlobals::output_type_c64)
        ConvertToC64(m_rt->m_globals.m_dither,m_rt->m_globals.m_multicolor==1,m_rt->m_globals.m_ditherStrength);
    if (m_outputType==RayTracerGlobals::output_type_pico8)
        ConvertToP8(m_rt->m_globals.m_dither,m_rt->m_globals.m_ditherStrength);

    //qDebug() << "FRAME3";

    m_pixmap.convertFromImage(m_img);
    if (m_img.width()<321)
        m_pixmap = m_pixmap.scaled(320, 200, Qt::IgnoreAspectRatio, Qt::FastTransformation);


    emit SignalImageUpdate();

    }
    //qDebug() << "FRAME5";

}


void DemoEffectRaytracer::Save(QString f)
{
    QFile file(f);
    file.open(QFile::WriteOnly);
    file.write(data);
    file.close();
}

void DemoEffectRaytracer::Init()
{
    m_rt->m_camera.m_rotMatrix.setToIdentity();
    m_rt->m_camera.setupViewmatrix();



    m_cols = m_rt->m_globals.m_c64Colors;

    m_mc->SetColor(m_cols[0],0);
    m_mc->SetColor(m_cols[1],1);
    m_mc->SetColor(m_cols[2],3);
    m_mc->SetColor(m_cols[3],3);


}


void DemoEffectRaytracer::AppendData()
{
    if (m_curFrame>=m_noFrames)
        return;
    MultiColorImage* mc = dynamic_cast<MultiColorImage*>(m_mc);
    if (mc==nullptr)
        return;
    for (int y=0;y<m_frameHeight;y++) {
        for (int x=0;x<m_frameWidth;x++) {
            PixelChar& pc = mc->m_data[x+y*40];
            for (int i=0;i<8;i++)
                data.append(pc.reverse(pc.p[i]));
        }
    }
    m_curFrame++;
}

void DemoEffectRaytracer::ToggleAnim()
{
    AbstractDemoEffect::ToggleAnim();
    m_time = 0;
    data.clear();
    m_curFrame = 0;
}
