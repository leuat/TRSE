/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

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
    ie1->m_params["tiltampx"] = 0.0;
    ie1->m_params["tiltampy"] = 0.0;
    ie1->m_params["tiltscale"] = 0.0;

    m_effects.append(ie1);

    LImageEffect* ie2 = new LImageEffect("Waves");
    ie2->m_params["height"] = 80;
    ie2->m_params["width"] = 40;
    ie2->m_params["spread"] = 2;
    ie2->m_params["amplitude"] = 0.045;
    ie2->m_params["threshold"] = 0.0;

    m_effects.append(ie2);

    LImageEffect* ie3 = new LImageEffect("SpriteCircles");
    ie3->m_params["count"] = 16;
    ie3->m_params["amplitude"] = 0.045;
    ie3->m_params["threshold"] = 0.0;

    m_effects.append(ie3);

}

void LImageEffects::RenderCircles(LImageEffect *le)
{
    float density = le->m_params["density"];
    int continuous = le->m_params["continuous"];
    float width = m_image->m_width;
    float height = m_image->m_height;
    float tiltampx = le->m_params["tiltampx"];
    float tiltampy = le->m_params["tiltampy"];
    float tiltscale = le->m_params["tiltscale"];
    QVector3D center(0.5, 0.5,0);
    for (int x=0;x<width;x++)
        for (int y=0;y<height;y++) {

            QVector3D p(x/width, y/height,0);
            p=p-center;
            //float angle = atan2(y,x);


            float l = pow(p.length(), le->m_params["lpow"]);

            QVector3D shift(tiltampx*cos(l*tiltscale),tiltampy*sin(l*tiltscale),0 );

            // Now redo all!
            p=p+shift;
            l = pow(p.length(), le->m_params["lpow"]);
            float angle = atan2(p.y(),p.x());//+3.14159*2;


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




void LImageEffects::RenderWaves(LImageEffect *le)
{
    float cwidth = le->m_params["width"];
    float cheight = le->m_params["height"];
    float spread = le->m_params["spread"];
    float threshold = le->m_params["threshold"];
    float amp = le->m_params["amplitude"];


    float height = m_image->m_height;
    float width = m_image->m_width;
//    return;
    QVector3D center(0.5, 0.5,0);
    for (int x=0;x<width;x++) {
        for (int y=0;y<height;y++) {
            QVector3D p(x/width, y/height,0);
            float yy = sin(p.x()*cwidth)*amp;
            float l = sin((p.y()+yy)*cheight);
//            while (l>1) l-=1;
            int k=0;
            if (l>threshold)
                k=2;


//            m_image->m_currencChar = k;
            m_image->setPixel(x,y,k);

        }
    }
}

void LImageEffects::RenderSpriteCircles(LImageEffect *le)
{
    LImageSprites2 *img = dynamic_cast<LImageSprites2*>(m_image);
    if (img==nullptr)
        return;
    float cnt = le->m_params["count"];


    float w = img->m_width;
    float h = img->m_height;
    for (int i=0;i<cnt;i++) {
        img->AddNew(1,1);
        float a = i/cnt;
        float b = (i+2)/cnt;
        for (int x=0;x<w;x++)
            for (int y=0;y<h;y++) {
                int col = 2;
                float xx = (x-w/2.0)/w;
                float yy = (y-h/2.0)/h;
                float s = sqrt(xx*xx+yy*yy)/0.45f;
                if (s>a && s<b)
                    col=1;
                img->setPixel(x,y,col);

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
    if (winner->m_name=="Waves")
        RenderWaves(winner);
    if (winner->m_name=="SpriteCircles")
        RenderSpriteCircles(winner);


}

