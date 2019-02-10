#include "demoeffectraytracer.h"

DemoEffectRaytracer::DemoEffectRaytracer() : AbstractDemoEffect (nullptr)
{


}


void DemoEffectRaytracer::Initialize()
{
    SetParameters(0);
    if (m_rt == nullptr)
        m_rt = new RayTracer();

   m_mc = new CharsetImage(LColorList::C64);

   m_mc->m_currentMode = CharsetImage::FULL_IMAGE;
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
    m_time+=1;
    // m_mc->Clear();
    Init();
    m_timer = QElapsedTimer();
    m_timer.start();

    int w = m_rt->m_globals.m_width;
    int h = m_rt->m_globals.m_height;
//        m_rt->Raymarch(m_img, 2*w,1.6*h);
//        m_img.fill(QColor(0,0,0,0));
//        m_rt->Raymarch(m_img, 2*w,h);
    m_rt->Raymarch(m_img, w,h);

    m_elapsedTime = m_timer.elapsed();
//        ConvertToC64(m_params["dither"].m_val == 1);
    m_toggleC64 = m_rt->m_globals.m_c64Output==1;
     ConvertToC64(m_rt->m_globals.m_dither==1);

     m_ready = false;

     //    QPixmap p;
    m_pixmap.convertFromImage(m_img);
    if (m_img.width()<321)
        m_pixmap = m_pixmap.scaled(320, 200, Qt::IgnoreAspectRatio, Qt::FastTransformation);

//    sleep(100);

    emit SignalImageUpdate();
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
    for (int y=0;y<m_frameHeight;y++) {
        for (int x=0;x<m_frameWidth;x++) {
            PixelChar& pc = m_mc->m_data[x+y*40];
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
