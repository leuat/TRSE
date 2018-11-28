#include "demoeffecttwister.h"

DemoEffectTwister::DemoEffectTwister(QGridLayout* gl) : AbstractDemoEffect (gl)
{
//    m_mc = new MultiColorImage(LColorList::C64);
    m_mc = new CharsetImage(LColorList::C64);

    m_mc->m_currentMode = CharsetImage::FULL_IMAGE;
     //m_mc->Initialize(160,200);
     m_mc->setMultiColor(true);
     m_img = QImage(160,200,QImage::Format_ARGB32);



     SetParameters(0);
}

void DemoEffectTwister::RenderBar(int y, float width, QVector3D init, float maxy, float tt, float sscale )
{
    if (m_angles.count()==0)
        return;
    QVector<float> ang = m_angles;
    float t=0;//m_time/50.0;
    if (m_toggleAnim)
        t=m_time*5.212;
    float k=0;

    for (int i=0;i<ang.count();i++) {
        ang[i]+=y/maxy*(2*M_PI/m_params["no_points"].m_val);
        if (m_toggleAnim) {
            ang[i]+=+sin((y+t)/120.0);
            ang[i]-=cos((1.3*y-t*1.23)/172.0);
        }
    }
/*    for (int i=0;i<ang.count();i++) {
        ang[i] = (int)((int)(1000*ang[i])%6283)/1000.0;
    }
    qSort(ang.begin(),ang.end());
*/

    QVector3D light(m_params["light_x"].m_val,m_params["light_y"].m_val,m_params["light_z"].m_val);
    light= light.normalized();

    for (int i=0;i<ang.count();i++) {
        int x1 = cos(ang[i])*width + width;
        int j=(i+1)%ang.count();
        int x2 = cos(ang[j])*width + width;

        QVector3D p1(cos(ang[i]),0,sin(ang[i]));
        QVector3D p2(cos(ang[j]),0,sin(ang[j]));


        QVector3D dpx = (p2-p1);
        QVector3D dir = (p2-p1).normalized();

/*        float l =QVector3D::dotProduct(dir,light);
        l = l *m_cols.count();
        if (l<0) l=0;
        if (l>=m_cols.count()) l=m_cols.count()-1;
*/

        QVector3D l;
        //int c =m_cols[l];

//        int c=i+1;



        if (x2>x1) {

            QVector3D p = p1;
            QVector3D ddir = dir /(x2-x1);
            int k=0;
            float ss = 0;
            float sx = 3.14159/(x2-x1);
            float step = 0.1;
            for (int j=x1;j<x2;j++) {

                //int col=c;

                l=init;


                float scale = tt+(1-tt)*cos(ss);
//                float scale = 1+sin(ss);
                p=(p1+k*ddir)*scale;

                // Raytrace for shadows
                for (int l2=0;l2<30;l2++) {
                    QVector3D pp = p+light*step*l2;

                    QVector3D perp(-dpx.z(), 0,dpx.x());
                        float d = QVector3D::dotProduct(pp + p1, perp);

//                    if (QVector3D::dotProduct(pp-p1,dpx)>0)
                       if (d<0)
                        l-=QVector3D(10,10,10)*sscale;
                      // l-=QVector3D(7,8,10);
//                       l-=QVector3D(7,8,10);

                }
//                col=c;
                if (l.x()<0) l.setX(0);
                if (l.y()<0) l.setY(0);
                if (l.z()<0) l.setZ(0);
//                if (l<0) l=0;
               // col = m_cols[5-(int)(l/256*5)];
         //       m_mc->setPixel(j,y,col);
//                QColor c(l*0.5,l*0.5,l);
                m_img.setPixel(j,y,Util::toColor(l).rgba());

                k++;
                ss+=sx;
            }
        }
    }



}

void DemoEffectTwister::Render(QImage &img)
{

}

void DemoEffectTwister::SetParameters(int preset)
{
    if (preset==0) {
        m_params["no_points"] = DemoEffectParam("no_points",4);
        m_params["width"] = DemoEffectParam("width",20);
        m_params["init_red"] = DemoEffectParam("init_red",255);
        m_params["init_green"] = DemoEffectParam("init_green",255);
        m_params["init_blue"] = DemoEffectParam("init_blue",255);
        m_params["concave"] = DemoEffectParam("concave",0);
        m_params["shadow"] = DemoEffectParam("shadow",1.3);
        m_params["light_x"] = DemoEffectParam("light_x",1);
        m_params["light_y"] = DemoEffectParam("light_y",0);
        m_params["light_z"] = DemoEffectParam("light_z",3.0);
        m_params["color_bg"] = DemoEffectParam("color_bg",0);
        m_params["color_mc1"] = DemoEffectParam("color_mc1",7);
        m_params["color_mc2"] = DemoEffectParam("color_mc2",10);
        m_params["color_main"] = DemoEffectParam("color_main",2);
//        m_params["colorList"] = DemoEffectParam("colorList",3.0);


    }

}

void DemoEffectTwister::SaveTwister(QString filename, int w, int h)
{
    QFile f(filename);
    f.open(QFile::WriteOnly);
    QByteArray data;
/*    for (int y=0;y<h*8;y++)
        for (int x=0;x<w;x++) {
            int yy=y/8;
            //qDebug() << yy;
            PixelChar& pc = m_mc->m_data[40*yy+x];
            data.append(PixelChar::reverse(pc.p[y&7]));
        }

*/
    for (int y=0;y<h;y++)
        for (int x=0;x<w;x++) {
            //qDebug() << yy;
            PixelChar& pc = m_mc->m_data[40*y+x];
            for (int i=0;i<8;i++)
                data.append(PixelChar::reverse(pc.p[i]));
        }



    f.write(data);
    f.close();
}

void DemoEffectTwister::run()
{

    while (!m_abort) {



        m_time+=1;
        m_mc->Clear();
        m_img.fill(QColor(0,0,0,255));
        Init();
        for (int y=0; y<m_img.height();y++) {
                RenderBar(y,m_params["width"].m_val*2,
                QVector3D(m_params["init_red"].m_val,m_params["init_green"].m_val,m_params["init_blue"].m_val),
               m_height*8,
                m_params["concave"].m_val, m_params["shadow"].m_val);


  //          }
        }
/*        for (int y=0; y<m_img.height();y++) {
            for (int x=0;x<m_img.width();x++) {
                int winner = 0;
                QColor c = m_mc->m_colorList.getClosestColor(QColor(m_img.pixel(x,y)),winner);
                int col = m_mc->m_colorList.getIndex(c);
                m_mc->setPixel(x,y,col);
            }
        }
        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));
*/
      //  m_mc->setMultiColor(true);
        if (m_toggleC64) {
            m_mc->m_colorList.EnableColors(m_cols);
            m_mc->SetColor(m_cols[0],0);
            m_mc->SetColor(m_cols[1],1);
            m_mc->SetColor(m_cols[3],2);
            m_mc->SetColor(m_cols[2],3);
            m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList);

            m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));
        }
        emit SignalImageUpdate();
        this->msleep(10);
    }
}

void DemoEffectTwister::Save(QString f)
{
    SaveTwister(f,m_params["width"].m_val,m_height);

}

void DemoEffectTwister::Init()
{
    m_angles.clear();
    m_cols.clear();
    for (int i=0;i<m_params["no_points"].m_val;i++) {
        m_angles.append(i*2.0*M_PI/(float)m_params["no_points"].m_val);
    }

    m_cols.append(m_params["color_bg"].m_val);
    m_cols.append(m_params["color_mc1"].m_val);
    m_cols.append(m_params["color_main"].m_val); // This one generic
    m_cols.append(m_params["color_mc2"].m_val);

    m_mc->SetColor(m_cols[0],0);
    m_mc->SetColor(m_cols[1],1);
    m_mc->SetColor(m_cols[3],2);
    m_mc->SetColor(m_cols[2],3);


}
