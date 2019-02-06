#include "demoeffectraytracer.h"

DemoEffectRaytracer::DemoEffectRaytracer(QGridLayout* gl) : AbstractDemoEffect (gl)
{



//    m_rt->m_camera.m_camera = QVector3D(0,0,-5);
//    float a = 0.05;
  //  m_rt->m_globals.m_ambient = QVector3D(a,a,a);

    /*if (m_currentscene==2)
        Scene2();

    if (m_currentscene==1)
        Scene1();

    if (m_currentscene==3)
        Scene3();

    if (m_currentscene==4)
        Scene4();

    if (m_currentscene==5)
        Scene5();

    if (m_currentscene==6)
        Scene6();

    m_params["camera_x"] = DemoEffectParam("camera_x",0);
    */
}

void DemoEffectRaytracer::Scene1()
{
    for (int i=0;i<10; i++) {
        int rw = 8;
        int rx = 16;
        QVector3D pos = QVector3D(rand()%rx-rx/2,rand()%rw-rw/2+1,rand()%rx-rx/2 );
        QVector3D col = QVector3D(rand()%rw,rand()%rw,rand()%rw)/(float)rw;
        int k= rand()%100;
 //       if (i==0) k=10;
   //     if (i==1) k=70;
        float pn = 0.0;
        float ps =3.2;
//        k=45;

        float ref = rand()%100/100.0;
//        k=45;
       if (k>=0 && k<30)
            m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(1,1,1), Material(col,rand()%100,ref, pn,ps,"")));
        else if
           (k>=30 && k<=40)
            m_rt->m_objects.append(new RayObjectSphere(pos,QVector3D(1,1,1), Material(col,rand()%100,ref, pn,ps,"")));
       else if
          (k>=40 && k<=60)
           m_rt->m_objects.append(new RayObjectCylinder(pos,QVector3D(0.3,0.1,2), Material(col,rand()%100,ref, pn,ps,"")));
        else
            m_rt->m_objects.append(new RayObjectTorus(pos,QVector3D(1,0.3,0), QVector3D(0,1,0),Material(col,rand()%100,ref, pn,ps,"")));

    }

//    m_rt->m_objects.append(new RayObjectTorus(QVector3D(0,0,0),QVector3D(2,1,0), QVector3D(0,1,0), Material(col,rand()%150+100,0.4, 0.0f,19.123,"")));
    m_rt->m_objects.append(new RayObjectPlane(QVector3D(0,-4,0),QVector3D(0,1,0),
                                             Material(QVector3D(0.9, 0.7, 0.5),30,0.5,0.0f,3.0, "")));






}

void DemoEffectRaytracer::Scene2()
{
    QVector3D pos =  QVector3D(0,0,0);
    QVector3D col = QVector3D(0.6,0.6,0.8);
    float ref = 0.2;
    float pn = 0;
    float ps = 2;
    float spec = 120;
//    m_rt->m_objects.append(new RayObjectCylinder(pos,QVector3D(0.3,0.1,2), Material(col,spec,ref, pn,ps,"")));


    QVector3D p(6,0,0);
    m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(1,1,1), Material(col,spec,ref, pn,ps,"")));

    m_rt->m_objects.append(new RayObjectBox(pos-p,QVector3D(1,1,1), Material(col,spec,ref, pn,ps,"")));

    m_rt->m_objects.append(new RayObjectBox(pos+p,QVector3D(1,1,1), Material(col,spec,ref, pn,ps,"")));

    m_rt->m_objects.append(new RayObjectBox(pos-p*2,QVector3D(1,1,1), Material(col,spec,ref, pn,ps,"")));

    m_rt->m_objects.append(new RayObjectBox(pos+p*2,QVector3D(1,1,1), Material(col,spec,ref, pn,ps,"")));

/*    m_rt->m_objects.append(new RayObjectTorus(pos,
                                             QVector3D(1,0.6,0),
                                             QVector3D(0,1,0),
                                             Material(col,spec,ref, pn,ps,"")));
*/
}

void DemoEffectRaytracer::Scene3()
{
    float N = m_N;
    float radius = 10;
    QVector3D col = QVector3D(0.6,0.6,0.8);
    float pn = 0;
    float ps = 1;
    float ref = 0.2;

    for (int i=0;i<N; i++) {

        float x= sin(i*3.1415965*2.0/N);
        float y= cos(i*3.1415965*2.0/N);
        QVector3D pos = QVector3D(x,0,y)*radius;
  //      m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0.2,1,0.0), Material(col,0,ref, pn,ps,"")));
//        m_rt->m_objects.last()->SetRotation(QVector3D(90,0,90));
        m_rt->m_objects.append(new RayObjectTorus(pos,QVector3D(2.25,0.7,0), QVector3D(0,1,0),Material(col,rand()%100,ref, pn,ps,"")));
        m_rt->m_objects.append(new RayObjectCylinder(pos,QVector3D(0.3,1,1),Material(col,rand()%100,ref, pn,ps,"")));
    }

}

void DemoEffectRaytracer::UpdateScene3()
{
    float t = (2*3.14165/m_N)*m_time/m_noFrames;
    float x = sin(t);
    float y = cos(t);
    m_rt->m_camera.m_camera = QVector3D(x,0.3,y)*24;
    m_rt->m_camera.m_target = QVector3D(0,1.1,0);
//    m_rt->m_camera.m_camera = QVector3D(x,0.35,y)*22;
  //  m_rt->m_camera.m_target = QVector3D(0,0.4,0);
    t=t+1.5;
    x = sin(t);
    y = cos(t);
    m_rt->m_globals.m_lights[0]->m_direction = QVector3D(x,0.9,y)*1;
    m_rt->m_globals.m_skyScale = 0;

    for (int i=1;i<m_rt->m_objects.count();i+=2) {
        float j = 3.14159*((i/1)&1);///(m_rt->m_objects.count()/2.0)*3.14159;
        float y = cos(t*6 + 3.14/2)*0.6;
        m_rt->m_objects[i]->m_position.setY(y);
    }
}


void DemoEffectRaytracer::Scene4()
{
    float N = 6;
    float radius = 10.0;
    QVector3D col = QVector3D(0.6,0.6,0.8);
    float pn = 0;
    float ps = 1;
    float ref = 0.2;

    QVector3D pos = QVector3D(0,0,0);
    m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0,1,0), QVector3D(7,3,7), Material(col,rand()%100,ref, pn,ps,"")));

}

void DemoEffectRaytracer::Scene5()
{
    float N = 6;
    float radius = 10.0;
    QVector3D col = QVector3D(0.6,0.6,0.8);
    float pn = 0;
    float ps = 1;
    float ref = 0.2;

    QVector3D pos = QVector3D(0,0,0);
    m_rt->m_objects.append(new RayObjectCylinder(pos,QVector3D(0.5,1,4), Material(col,rand()%100,ref, pn,ps,"")));

}

void DemoEffectRaytracer::UpdateScene5()
{
    float t = (2*3.14165/4.0)*m_time/18;
    float x = sin(t);
    float y = cos(t);

    m_rt->m_objects[0]->SetRotation(QVector3D(0,0,90*cos(t)));


    m_rt->m_camera.m_camera = QVector3D(x,0.3,y)*24;
    m_rt->m_camera.m_target = QVector3D(0,1.1,0);
//    m_rt->m_camera.m_camera = QVector3D(x,0.35,y)*22;
  //  m_rt->m_camera.m_target = QVector3D(0,0.4,0);
    t=t+1.0;
    x = sin(t);
    y = cos(t);
    m_rt->m_globals.m_lights[0]->m_direction = QVector3D(x,0.4,y)*1;
    m_rt->m_globals.m_skyScale = 0;


}



void DemoEffectRaytracer::Scene6()
{
    float N = 6;
    float radius = 10.0;
    QVector3D col = QVector3D(0.6,0.6,0.8);
    float pn = 0;
    float ps = 1;
    float ref = 0.2;

    m_time=0;
    QVector3D pos = QVector3D(0,-1.5,0);
    m_rt->m_objects.append(new RayObjectCylinder(pos,QVector3D(1.5,1,8), Material(col*0.7,1000000,ref*0.1, pn*0.1,ps*0.1,"")));
//    m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0,1,0), QVector3D(5,10,5),Material(col*0.7,100000,ref*0.1, pn*0.1,ps*0.1,"")));
//    m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0,1,0), QVector3D(5,10,5),Material(col*0.7,100000,ref*0.1, pn*0.1,ps*0.1,"")));

    // p=3 regular
    float p = 3;
    // 5.5 for outside
    m_charset = new CharsetImage(LColorList::C64);

    m_charset->LoadCharset(":resources/character.rom");

    PixelChar& pc = m_charset->m_data[m_char];
    for (int y=0;y<8;y++)
        for (int x=0;x<8;x++) {
//            qDebug() << x << y;
            if (((pc.p[y]>>x) & 1)==1) {
                float xx = ((x-4)/1 + 0.5)*2;
                float yy = ((y-4)/1)*1.4;
                float s = 0.5;
                QVector3D pos = QVector3D(-p,yy,xx);
     /*           m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0,1,0),
                                                       QVector3D(s,s*1.4,s*1.5), Material(col,100,ref, pn,ps,"")));

                pos = QVector3D(p,yy,-xx);
                m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0,1,0),
                                                       QVector3D(s,s*1.4,s*1.5), Material(col,100,ref, pn,ps,"")));
*/

                m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0,1,0),
                                                       QVector3D(s,s*1.4,s*2), Material(col,100,ref, pn,ps,"")));

/*                pos = QVector3D(1.5,yy,-xx);
                m_rt->m_objects.append(new RayObjectBox(pos,QVector3D(0,1,0),
                                                       QVector3D(s,s,s), Material(col,100,ref, pn,ps,"")));
*/
            }
        }



}

void DemoEffectRaytracer::UpdateScene6()
{


    float t = (2*3.14165/1)*m_time/(m_noFrames+1);
    float x = sin(t);
    float y = cos(t);
    m_rt->m_camera.m_camera = QVector3D(x,0.0,y)*13; // 11
    m_rt->m_camera.m_target = QVector3D(0,0,0);
//    m_rt->m_camera.m_camera = QVector3D(x,0.35,y)*22;
  //  m_rt->m_camera.m_target = QVector3D(0,0.4,0);
//    t=t+0.75;
    t=t+0.75;
    x = sin(t);
    y = cos(t);
    m_rt->m_globals.m_lights[0]->m_direction = QVector3D(x,0.0,y)*1;
    m_rt->m_globals.m_skyScale = 0;


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

void DemoEffectRaytracer::CompressAndSaveCharset()
{
    int noChars = 0;
    if (m_curFrame<m_noFrames) {
        m_mc->CompressAndSave(m_charData, m_screenData, 0,40, 13,25,noChars, m_compression, 128);
        m_infoText+="Compressing frame "+QString::number(m_curFrame) + " with chars : " + QString::number(noChars)+"\n";

    }
    m_curFrame++;

    if ((m_curFrame==6*m_nextFrameSave && m_curFrame<=m_noFrames) || m_curFrame==m_noFrames || m_curFrame==20) {
        QString kk = QString::number(m_nextFrameSave);
        QFile f("chardata"+kk+".bin");
        f.open(QFile::WriteOnly);
        f.write(m_charData);
        f.close();
        m_charData.clear();

        QFile f2("screendata"+kk+".bin");
        f2.open(QFile::WriteOnly);
        f2.write(m_screenData);
        f2.close();
        m_screenData.clear();
        m_nextFrameSave++;

    }

}

void DemoEffectRaytracer::CompressAndSaveScreen()
{
    int ww = 16;
//    qDebug() << m_frameWidth*4 << m_frameHeight*8;
    if (m_curFrame<m_noFrames) {
        AddScreen(m_screenData, 16,16);
        m_count++;
//        qDebug() <<m_count*16 << " but is " <<m_screenData.count()/ww << "Should be: " << m_screenData.count()/m_count/16;
//        qDebug() << m_screenData.count()/16;
  //      exit(1);
//        qDebug() << m_curFrame;
//        exit(1);
       }

    m_curFrame++;

    if (m_curFrame==m_noFrames) {
        m_char++;

        qDebug() << "Current char:" <<QString::number(m_char);
        if (m_char == m_noChars-1) {
            qDebug() <<m_count*ww*16 << " but is " <<m_screenData.count();
            qDebug() << 26*ww*16*32 << " mcount" <<m_count ;
            QByteArray packedData, table;
            table.clear();
            qDebug() <<m_count*16 << " but is " <<m_screenData.count()/ww;
            OptimizeAndPackCharsetData(m_screenData, packedData, table, ww, m_compression);
            qDebug() << "Table should be : " << (m_noChars-1)*1024;
            qDebug() << "Table is : " << table.count();


            QFile f("screen.bin");
            f.open(QFile::WriteOnly);
            f.write(packedData);
            f.close();

            QFile f2("table.bin");
            f2.open(QFile::WriteOnly);
            f2.write(table);
            f2.close();

            m_toggleAnim=false;

        }
        else {
            m_curFrame = 0;

            m_rt->m_objects.clear();
            Scene6();
        }

    }


}






void DemoEffectRaytracer::UpdateScene4()
{
    float t = (2*3.14165/4.0)*m_time/18;
    float x = sin(t);
    float y = cos(t);
    m_rt->m_camera.m_camera = QVector3D(x,0.3,y)*24;
    m_rt->m_camera.m_target = QVector3D(0,1.1,0);
//    m_rt->m_camera.m_camera = QVector3D(x,0.35,y)*22;
  //  m_rt->m_camera.m_target = QVector3D(0,0.4,0);
    t=t+1.0;
    x = sin(t);
    y = cos(t);
    m_rt->m_globals.m_lights[0]->m_direction = QVector3D(x,0.4,y)*1;
    m_rt->m_globals.m_skyScale = 0;

}



void DemoEffectRaytracer::UpdateScene2() {
    float t = -(360/(float)(m_noFrames))*((int)m_time%(int)(m_noFrames));
//    float t2 = (11/(float)(m_noFrames-1))*((int)m_time%(int)(m_noFrames));
    float ww = 5.3;
    float t2 = (ww/(float)(m_noFrames-1))*((int)m_time%(int)(m_noFrames));

    m_rt->m_camera.m_camera.setX(t2-ww/2);
    m_rt->m_camera.m_target.setX(t2-ww/2);
    m_rt->m_camera.m_target.setY(-1.3);

    m_rt->m_globals.m_lights[0]->m_direction = QVector3D(0,0,-1);

   for (AbstractRayObject* ro : m_rt->m_objects) {
        ro->m_bbRadius=30;
        ro->SetRotation(QVector3D(0,0, t));
   }
    m_rt->m_globals.m_skyScale = 0;
    m_rt->m_camera.m_camera.setZ(-100);
    m_rt->m_globals.m_shadowScale = 1;
    m_rt->m_camera.m_fov = 3.0;
}



void DemoEffectRaytracer::UpdateScene1()
{
    float t = m_time/30;
     static_cast<DirectionalLight*>(m_rt->m_globals.m_lights[0])->m_direction =
             QVector3D(sin(t),1,-cos(t*1.23123)).normalized();

     t = m_time/41.32;
     float r= 11;
     m_rt->m_camera.m_camera =
             QVector3D(sin(t)*r,0.2,cos(t)*r);

     int i=0;
     t = m_time*3.32;

 /*    m_rt->m_objects[0]->SetRotation(QVector3D(i+t,-2*t+2*i, 3.12*i+t));
     m_rt->m_objects[1]->SetRotation(QVector3D(i+t,2*t+2*i, 3.12*i+t*2));*/
     for (AbstractRayObject* a: m_rt->m_objects) {
         if (i!=m_rt->m_objects.count()-1)
         a->SetRotation(QVector3D(i*63.1345+t,-2.13*t+2.134*i, 3.12*i+t));
         i++;
     }

     m_rt->m_globals.m_skyScale = sin(m_time/18.12)/3.0 + 0.5;
     m_rt->m_camera.m_target.setY(sin(m_time/21.34)*4-2);


}

void DemoEffectRaytracer::run()
{
    while (!m_abort) {
        m_time+=1;
        // m_mc->Clear();
        m_img.fill(QColor(0,0,0,255));
        Init();
        m_timer = QElapsedTimer();
        m_timer.start();
//        m_timer.
//        m_rt->Raytrace(m_img);
//        int w = m_img.width();
  //      int h = m_img.height();
//        w = 4*(m_frameWidth);
  //      h = 8*m_frameHeight;
        int w = m_rt->m_globals.m_width;
        int h = m_rt->m_globals.m_height;
//        m_rt->Raymarch(m_img, 2*w,1.6*h);
//        m_img.fill(QColor(0,0,0,0));
//        m_rt->Raymarch(m_img, 2*w,h);
        m_rt->Raymarch(m_img, w,h);
        for (int y=0;y<h;y++) {
        //for (int x=0;x<1;x++)
        {
            for (int i=0;i<w;i++)
//                m_img.setPixel(i+x*w,y,m_img.pixel(i,y));
            m_img.setPixel(i,y,m_img.pixel(i,y));
        }
        }



        m_elapsedTime = m_timer.elapsed();
//        ConvertToC64(m_params["dither"].m_val == 1);
        m_toggleC64 = m_rt->m_globals.m_c64Output==1;
         ConvertToC64(m_rt->m_globals.m_dither==1);
//        AppendData();
//        CompressAndSaveCharset();
  //      CompressAndSaveScreen();
//        qDebug() << m_curFrame;

        emit SignalImageUpdate();

        this->msleep(6);
    }

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
    if (m_toggleAnim) {
        if (m_currentscene == 2)
            UpdateScene2();
        if (m_currentscene == 1)
            UpdateScene1();
        if (m_currentscene == 3)
            UpdateScene3();
        if (m_currentscene == 4)
            UpdateScene4();
        if (m_currentscene == 5)
            UpdateScene5();
        if (m_currentscene == 6)
            UpdateScene6();
    }
    m_rt->m_camera.m_rotMatrix.setToIdentity();
    m_rt->m_camera.setupViewmatrix();


    m_cols.clear();

    m_cols.append(m_params["color_bg"].m_val);
    m_cols.append(m_params["color_mc1"].m_val);
    m_cols.append(m_params["color_main"].m_val); // This one generic
    m_cols.append(m_params["color_mc2"].m_val);

    m_mc->SetColor(m_cols[0],0);
    m_mc->SetColor(m_cols[1],1);
    m_mc->SetColor(m_cols[3],2);
    m_mc->SetColor(m_cols[2],3);

//    m_rt->m_camera.m_fov = 90;

}

void DemoEffectRaytracer::SetParameters(int preset)
{
    if (preset==0) {
        m_params["color_bg"] = DemoEffectParam("color_bg",0);
        m_params["color_mc1"] = DemoEffectParam("color_mc1",4);
        m_params["color_mc2"] = DemoEffectParam("color_mc2",6);
        m_params["color_main"] = DemoEffectParam("color_main",1);
        m_params["char_width"] = DemoEffectParam("char_width",40);
        m_params["char_height"] = DemoEffectParam("char_height",25);
        m_params["dither"] = DemoEffectParam("dither",1);

    }

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
