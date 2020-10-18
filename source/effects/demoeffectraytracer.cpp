#include "demoeffectraytracer.h"
#include "source/LeLib/limage/standardcolorimage.h"
DemoEffectRaytracer::DemoEffectRaytracer() : AbstractDemoEffect (nullptr)
{


}
void DemoEffectRaytracer::Initialize()
{
    SetParameters(0);
    if (m_rt == nullptr)
        m_rt = new RayTracer();


   m_mc = new MultiColorImage(LColorList::C64);

    if (!m_rt->m_globals.m_multicolor)
        m_mc = new StandardColorImage(LColorList::C64);

   if (m_rt->m_globals.m_c64ImageType == 0.0) {
       if (m_mc==nullptr)
           delete m_mc;
       m_mc = new CharsetImage(LColorList::C64);
       //((CharsetImage*)(m_mc))->m_currentMode = CharsetImage::FULL_IMAGE;

   }
   if (m_rt->m_globals.m_outputType == RayTracerGlobals::output_type_pico8) {
       m_mc = new LImageQImage(LColorList::PICO8);
       m_mc->Initialize(m_rt->m_globals.m_width,m_rt->m_globals.m_height);
   }
   if (m_rt->m_globals.m_outputType == RayTracerGlobals::output_type_AMSTRAD) {
       m_mc = new LImageAmstradGeneric(LColorList::AMSTRADCPC);
       m_mc->Initialize(m_rt->m_globals.m_width,m_rt->m_globals.m_height);
   }
   if (m_rt->m_globals.m_outputType == RayTracerGlobals::output_type_VGA) {
       m_mc = new LImageQImage(LColorList::PICO8);
       m_mc->Initialize(m_rt->m_globals.m_width,m_rt->m_globals.m_height);
   }
   if (m_rt->m_globals.m_outputType == RayTracerGlobals::output_type_BINARY) {
       m_mc = new LImageQImage(LColorList::PICO8);
       m_mc->Initialize(m_rt->m_globals.m_width,m_rt->m_globals.m_height);
   }
   if (m_rt->m_globals.m_outputType == RayTracerGlobals::output_type_GAMEBOY) {
       m_mc = new LImageGamboy(LColorList::NES);
       m_mc->Initialize(m_rt->m_globals.m_width, m_rt->m_globals.m_height);
       m_mc->SetColor(0xD,0);
       m_mc->SetColor(0xC,1);
       m_mc->SetColor(0x1C,2);
       m_mc->SetColor(0x2C,3);
   }
   m_mc->setMultiColor(true);


   m_rt->m_globals.m_lights[0]->m_color = QVector3D(1,1,0.7);
   m_img = QImage(m_rt->m_globals.m_orgWidth,m_rt->m_globals.m_orgHeight,QImage::Format_ARGB32);
   m_post = QImage(m_rt->m_globals.m_orgWidth,m_rt->m_globals.m_orgHeight,QImage::Format_ARGB32);
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
    int w = m_rt->m_globals.m_orgWidth;
    int h = m_rt->m_globals.m_orgHeight;

    m_rt->Raymarch(m_img, w,h);




    //qDebug() << "FRAME2";

    m_elapsedTime = m_timer.elapsed();
    m_outputType = m_rt->m_globals.m_outputType;
    m_post.fill(0);
    if (m_script->lua_exists("PostProcess"))
        luaL_dostring(m_script->L, "PostProcess()");




    if (m_rt->m_globals.m_smooth!=0) {
        LImageQImage img;
        img.Initialize(m_img.width(), m_img.height());
        img.m_qImage = &m_img;
        m_img = *img.Blur(m_rt->m_globals.m_smooth);
        img.m_qImage = nullptr;
    }

    w = m_rt->m_globals.m_width;
    h = m_rt->m_globals.m_height;
    if (w!=m_rt->m_globals.m_orgWidth || h!=m_rt->m_globals.m_orgHeight) {
/*        m_img.save("testBefore.png");
        m_img  = m_img.scaled(w,h,Qt::IgnoreAspectRatio,
                              Qt::SmoothTransformation);
        m_img.save("testAfter.png");*/
//        qDebug() << "Scaling..";
    }


    if (m_outputType==RayTracerGlobals::output_type_c64)
        ConvertToC64(m_rt->m_globals.m_dither,m_rt->m_globals.m_multicolor==1,m_rt->m_globals.m_ditherStrength);

    if (m_outputType==RayTracerGlobals::output_type_pico8)
        ConvertToP8(m_rt->m_globals.m_dither,m_rt->m_globals.m_ditherStrength);

    if (m_outputType==RayTracerGlobals::output_type_AMSTRAD)
        ConvertToCPC(m_rt->m_globals.m_dither,m_rt->m_globals.m_ditherStrength);

    if (m_outputType==RayTracerGlobals::output_type_VGA)
        ConvertToP8(m_rt->m_globals.m_dither,m_rt->m_globals.m_ditherStrength);

    if (m_outputType==RayTracerGlobals::output_type_BINARY)
        ConvertToBIN(m_rt->m_globals.m_dither,m_rt->m_globals.m_ditherStrength);

    if (m_outputType==RayTracerGlobals::output_type_GAMEBOY)
        ConvertToC64(m_rt->m_globals.m_dither,m_rt->m_globals.m_multicolor==1,m_rt->m_globals.m_ditherStrength);

/*
    if (m_outputType==RayTracerGlobals::output_type_GAMEBOY)
        ConvertToC64(m_rt->m_globals.m_dither,m_rt->m_globals.m_multicolor==1,m_rt->m_globals.m_ditherStrength);
*/

//    if (m_outputType==RayTracerGlobals::output_type_STANDARD)
  //      ConvertToStandard(m_rt->m_globals.m_dither,m_rt->m_globals.m_ditherStrength);



    m_pixmap.convertFromImage(m_img);
    if (m_img.width()<321)
        m_pixmap = m_pixmap.scaled(320, 200, Qt::IgnoreAspectRatio, Qt::FastTransformation);

//    msleep(1000);
    m_ready = false;
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

    if (m_cols.count()>=4) {
        m_mc->SetColor(m_cols[0],0);
        m_mc->SetColor(m_cols[1],1);
        m_mc->SetColor(m_cols[2],3);
        m_mc->SetColor(m_cols[3],3);
    }
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
