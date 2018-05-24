#include "limageeffects.h"
#include "c64fullscreenchar.h"


LImageEffects::LImageEffects()
{
    InitEffects();
}

QStringList LImageEffects::getStringList()
{
    QStringList lst;
    for (LImageEffect* le: m_effects)
        lst<< le->m_name;

    return lst;
}

void LImageEffects::InitEffects()
{
    LImageEffect* ie1 = new LImageEffect("Circles");
    ie1->m_params["density"] = 60;
    ie1->m_params["angleStretch"] = 3.23;
    ie1->m_params["noColors"] = 4;
    ie1->m_params["continuous"] = 1;
    ie1->m_params["lpow"] = 0.5;

    m_effects.append(ie1);

}

void LImageEffects::RenderCircles(LImageEffect *le)
{
    float density = le->m_params["density"];
    int continuous = le->m_params["continuous"];
    float width = m_image->m_width;
    float height = m_image->m_height;
    QVector3D center(0.5, 0.5,0);
    for (int x=0;x<width;x++)
        for (int y=0;y<height;y++) {

            QVector3D p(x/width, y/height,0);
            p=p-center;
            //float angle = atan2(y,x);
            float angle = atan2(p.y(),p.x());//+3.14159*2;


            float l = pow(p.length(), le->m_params["lpow"]);
            float as = ((angle*le->m_params["angleStretch"]));
            if (continuous==0) as=(((((int)(angle*8.0))*le->m_params["angleStretch"])));
            //if ((int)as%2==0)  as=0;
            int k = ((int)(l*density + as));

            if (dynamic_cast<C64FullScreenChar*>(m_image)!=nullptr) {

                m_image->m_writeType=LImage::Character;

                m_image->m_currencChar = k;
                m_image->setPixel(x,y,k);

            }
            else {

                while (k<0) k+=le->m_params["noColors"];
                k=k%(int)le->m_params["noColors"];
                m_image->setPixel(x,y,k);
            }

        }

}

void LImageEffects::Render(QString effect)
{
    LImageEffect* winner = nullptr;
    for (LImageEffect* le: m_effects)
        if (le->m_name==effect)
            winner=le;

    if (winner==nullptr)
        return;


    if (winner->m_name=="Circles")
        RenderCircles(winner);


}

