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


#include "limageqimage.h"
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QPainter>
#include "source/LeLib/util/util.h"
#include <QKeyEvent>

LImageQImage::LImageQImage(LColorList::Type t)  : LImage(t)
{
    Initialize(320,200);
    m_scale = 1;
    m_type = LImage::Type::QImageBitmap;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_currentChar = 0;
}

void LImageQImage::LoadBin(QFile& file)
{
    //    m_width = 320;
    //    m_height = 200;
    m_qImage = new QImage(m_width, m_height, QImage::Format_ARGB32);
    unsigned char *data = new unsigned char[m_width*m_height];
    file.read((char*)data, m_width*m_height);
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {
            setPixel(i,j, data[i+j*m_width]);
        }
    delete[] data;
}

void LImageQImage::SaveBin(QFile& file)
{
    if (m_qImage->width()!=m_width || m_qImage->height()!=m_height) {
        qDebug() << "LImageQImage::SaveBin error... width/height are not equal!";
        qDebug() << m_qImage->width()<<" vs " << m_width;
        qDebug() << m_qImage->height()<<" vs " << m_height;
        return;
    }
    auto keep = m_footer.get(LImageFooter::POS_DISPLAY_CHAR);
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
    unsigned char *data = new unsigned char[m_width*m_height];
    for (int i=0;i<m_qImage->width();i++)
        for (int j=0;j<m_qImage->height();j++) {
            unsigned char val = getPixel(i,j);
            data[(i+j*m_qImage->width())] = val;
        }
    file.write((char*)data, m_width*m_height);
    delete[] data;
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,keep);

}

void LImageQImage::ExportBlackWhite(QFile &file, int xx, int yy, int w, int h,int type) {

    char c = 0;
    int cnt = 0;
    QByteArray data;

    if (type==1) {

        for (int x=0;x<w;x++)
            for (int y=0;y<h;y++) {
                int p = getPixel(x+xx,y+yy);
                if (p!=0)
                    c |= 1<<(cnt);

                cnt++;
                if (cnt==8) {
                    data.append(c);
                    c = 0;
                    cnt = 0;
                }

            }

    }
    // Pokemon mini
    if (type==2) {

        for (int y=0;y<h;y+=8) {
            for (int x=0;x<w;x++){
                for (int yp=0;yp<8;yp++) {
                    int p = getPixel(x+xx,y+yy+yp);
                    if (p!=0)
                        c |= 1<<(cnt);

                    cnt++;
                    if (cnt==8) {
                        data.append(c);
                        c = 0;
                        cnt = 0;
                    }

                }
            }
        }


    }
    if (type==0)
        for (int y=0;y<h;y++)
            for (int x=0;x<w;x++) {
                int p = getPixel(x+xx,y+yy);
                if (p!=0)
                    c |= 1<<(cnt);

                cnt++;
                if (cnt==8) {
                    data.append(c);
                    c = 0;
                    cnt = 0;
                }

            }
    file.write(data);
}

void LImageQImage::ExportBin(QFile &file)
{

}

void LImageQImage::RemapCharset(QImage *other, int border_w, int border_h, int blockwidth, int blockheight,int dw,int dh, int allowance)
{
    if (blockwidth==0)
        return;
    if (blockheight==0)
        return;
    if (dw==0)
        return;
    if (dh==0)
        return;
    QVector<QSharedPointer<QImage>> charset, temp;

    int cw = (other->width()-2*border_w)/blockwidth;
    int ch = (other->height()-2*border_h)/blockheight;
    for (int by=0;by<blockheight;by++) {
        for (int bx=0;bx<blockwidth;bx++) {
            QSharedPointer<QImage> img = QSharedPointer<QImage>(new QImage(dw,dh, m_qImage->format()));
            for (int cy=0;cy<dh;cy++) {
                for (int cx=0;cx<dw;cx++) {
                    int ox = cx/(float)dw*(float)cw;
                    int oy = cy/(float)dh*(float)ch;
                    QColor c = other->pixelColor(border_w+bx*cw+ox, border_h+by*ch+oy);
                    img->setPixel(cx,cy,c.rgb());
                }

            }
            charset.append(img);
        }
    }

    for (auto img: charset) {
        bool ok = true;
        for (auto img2 : temp) {
            int unequals = img2->height()*img2->height();
            for (int y=0;y<img2->height();y++)
                for (int x=0;x<img2->width();x++) {
                    if (img->pixel(x,y)!=img2->pixel(x,y))
                        unequals--;
                }
            if (unequals<=allowance)
                ok = false;

        }
        if (ok)
            temp.append(img);
    }
    charset = temp;


    // Now, write charset to image
    Clear();
    int dst_curx = 0;
    int dst_cury = 0;
    for (auto img: charset) {
        for (int y=0;y<dh;y++)
            for (int x=0;x<dw;x++) {
                setPixel(dst_curx+x, dst_cury+y,img->pixel(x,y));
            }
        dst_curx = dst_curx + dw;
        if (dst_curx>=m_width) {
            dst_curx = 0;
            dst_cury = dst_cury + dh;
        }
    }

}

void LImageQImage::LoadQImage(QString filename)
{
    Release();
    m_qImage = new QImage();

    if (!m_qImage->load(filename))
        qDebug() << "Could not find file " << filename;

    m_width = m_qImage->width();
    m_height = m_qImage->height();
}

double LImageQImage::getVal(int x, int y) {
    //  unsigned int col = getPixel(x,y);// % 16;

    //            img->setPixel(i,j,QRgb(col));
    QVector3D c = Util::fromColor(m_qImage->pixelColor(x,y));

    //    if (rand()%100>98) qDebug() << c << m_qImage->pixelColor(x,y);
    return (c.x() + c.y() + c.z())/3.0/255.0;
}

QPoint LImageQImage::getPixelPosition(int x, int y)
{
    int xx = x;
    int yy = y;
    if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {
        bool isRepeat = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)==1;

        double scale = 1;
        if (isRepeat)
            scale = 3;

        int cx = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
        int cy = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y);

        int x1 = (m_currentChar%getCharWidthDisplay())*8;
        int y1 = ((int)(m_currentChar/getCharWidthDisplay()))*8;
        xx = x1 + ((int)((x/(double)m_width)*8*cx*scale))%(8*cx);
        yy = y1 + ((int)((y/(double)m_height)*8*cy*scale))%(8*cy);
        //        xx = x1 + (x/(double)m_width)*8.0*cx;
        //      yy = y1 + (y/(double)m_height)*8.0*cy;
        //        if (rand()%100>98)
        //          qDebug() <<xx<<yy;

    }
    return QPoint(xx,yy);
}

QPixmap LImageQImage::ToQPixMap(int chr)
{
    int cx = 32;
    int cy = 32;

    int keep = m_footer.get(LImageFooter::POS_DISPLAY_CHAR);
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);

    QImage img(cx,cy,QImage::Format_ARGB32);
    for (int j=0;j<cy;j++) {
        for (int i=0;i<cx;i++) {
            int x = i/(double)cx*m_width;
            int y = j/(double)cy*m_height;

            int x1 = (chr%getCharWidthDisplay())*8;
            int y1 = (chr/getCharWidthDisplay())*8;

            int xx = x1 + (x/(double)m_width)*8;
            int yy = y1 + (y/(double)m_height)*8;

            unsigned int col = getPixel(xx,yy);// % 16;

            //            img->setPixel(i,j,QRgb(col));
            img.setPixel(i,j,m_colorList.get(col).color.rgb());
        }
    }
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,keep);


    return QPixmap::fromImage(img);
}

int LImageQImage::getCharWidthDisplay()
{
    /*    if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {
        int i =1;
        if (m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)==1)
            i=3;
        return i*8*m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
    }*/
    return m_charWidthDisplay;
}

int LImageQImage::getCharHeightDisplay()
{
    if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {
        int i =1;
        if (m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)==1)
            i=3;
        return i*8*m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y);
    }
    return m_charHeightDisplay;
}

int LImageQImage::getGridWidth()
{
    if (m_footer.get(LImageFooter::POS_DISPLAY_CHAR)==1) {
        int i =1;
        if (m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_REPEAT)==1)
            i=3;
        return i*8*m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
    }
    return m_gridWidthDisplay;
}

QByteArray LImageQImage::toQByteArray(bool inverted)
{
    QByteArray ba;
    if (!inverted){
        for (int i=0;i<m_qImage->width();i++)
            for (int j=0;j<m_qImage->height();j++) {
                unsigned char val = getPixel(i,j);
                ba.append(val);
            }
    }
    else
    {
        for (int i=0;i<m_qImage->height();i++)
  //      for (int i=0;i<8;i++)
            for (int j=0;j<m_qImage->width();j++) {
                //int yy = m_qImage->height()-8-y*8;
//                int yy = y*8; //m_qImage->height()-8-y*8;
                unsigned char val = getPixel(m_qImage->width()-1-j,m_qImage->height()-1-i);
                ba.append(val);
            }

    }
    //    file.write((char*)data, m_width*m_height);
    return ba;
}

void LImageQImage::fromQByteArray(QByteArray &ba)
{
    int idx = 0;
    for (int i=0;i<m_qImage->width();i++)
        for (int j=0;j<m_qImage->height();j++) {
            setPixel(i,j,(uchar)ba[idx++]);
        }

}



void LImageQImage::Initialize(int width, int height)
{
    if (m_width==width && m_height==height && m_qImage!=nullptr && width==m_qImage->width() && height == m_qImage->height())
        return;
    if (m_qImage != nullptr)
        delete m_qImage;

    m_width = width;
    m_height = height;

    m_qImage = new QImage(width, height, QImage::Format_ARGB32);
    //    qDebug() << "LimageImage:: "<<width << height;
    m_qImage->fill(QColor(0,0,0));

}


QImage* LImageQImage::ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect)
{
    //    if(src.isNull()) return QImage(); //No need to do anything else!
    if (effect==nullptr)
        return &src; //No need to do anything else!

    QGraphicsScene scene;
    QGraphicsPixmapItem item;
    item.setPixmap(QPixmap::fromImage(src));
    item.setGraphicsEffect(effect);
    scene.addItem(&item);
    // QSize(extent2, extent2
    QImage* res = new QImage(src.width(), src.height(), QImage::Format_ARGB32);
    res->fill(Qt::transparent);
    QPainter ptr(res);
    scene.render(&ptr, QRectF(), QRectF( 0, 0, src.width(), src.height() ) );
    return res;
}

void LImageQImage::CreateGrid(int x, int y,  QColor color, int strip, double zoom, QPointF center, double scale)
{

    double width = m_qImage->width();
    double height = m_qImage->height();
    QColor col2=QColor(20,30,40,255);
    QColor c;
    m_qImage->fill(QColor(0,0,0,0));
    center.setX(center.x()/(double)m_width*width/scale);
    center.setY(center.y()/(double)m_height*height);

    //    double xp = (((i-center.x())*(double)zoom)+ center.x());
    //  double yp = (((j-center.y())*(double)zoom)+ center.y());

    for (float i=1;i<x;i++)
        for (float j = 0;j<height;j++) {
            double xp = (width/((double)(x)))*(i);

            xp = (xp - scale*center.x())/zoom + scale*center.x();
            double yp = j;

            c = color;
            if ((int)j%strip>=strip/2)
                c= col2;
            if (xp>=0 && xp<width && yp>=0 && yp<height)
                m_qImage->setPixel(xp, yp, c.rgba());
        }

    // width lines
    for (float i=1;i<y;i++)
        for (float j = 0;j<width;j++) {
            float yp = (height/(float)(y))*(i);

            yp = (yp - center.y())/zoom + center.y();
            float xp = j;

            c = color;
            if ((int)j%strip>=strip/2)
                c= col2;
            if (xp>=0 && xp<width && yp>=0 && yp<height)
                m_qImage->setPixel(xp, yp, c.rgba());
        }



}

void LImageQImage::CopyFrom(LImage *img) {

    if (m_width!=img->m_width || m_height!=img->m_height) {
        m_width = img->m_width;
        m_height = img->m_height;

    }

    m_colorList.CopyFrom(&img->m_colorList);
    m_footer = img->m_footer;
    m_scaleX = img->m_scaleX;
    m_currentChar = img->m_currentChar;
    m_charWidthDisplay = img->m_charWidthDisplay;
    m_charWidth = img->m_charWidth;
    LImageQImage* mc = dynamic_cast<LImageQImage*>(img);
    if (mc!=nullptr) {
        *m_qImage = *mc->m_qImage;
    }
    //  m_banks = mc->m_banks;
    //    m_qImage = mc->m_qImage;

}



void LImageQImage::ApplyToLabel(QLabel *l)
{
    QPixmap p;
    if (m_qImage!=nullptr)
        p.convertFromImage(*m_qImage);
    l->setPixmap(p);

}

bool LImageQImage::KeyPress(QKeyEvent *e)
{
    QPoint dir(0,0);



    int sx = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
    int sy = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y);
    //    if (m_currentMode==CHARSET2x2 || m_currentMode==CHARSET2x2_REPEAT)
    //      s=2;

    if (e->key()==Qt::Key_W)
        m_currentChar-=m_charWidthDisplay*sy;
    if (e->key()==Qt::Key_A)
        m_currentChar-=sx;
    if (e->key()==Qt::Key_S)
        m_currentChar+=m_charWidthDisplay*sy;
    if (e->key()==Qt::Key_D)
        m_currentChar+=sx;

    if (m_currentChar>=m_charHeight*m_charWidth)
        m_currentChar=0;

    m_currentChar = Util::clamp(m_currentChar,0,m_charWidth*m_charHeight-1);


    return true;
}

void LImageQImage::setPixel(int x, int y, unsigned int color)
{
    if (m_qImage==nullptr)
        return;

    QPoint p = getPixelPosition(x,y);

    //    if (rand()%1000>900)
    //      qDebug() <<p;

    if (p.x()>=0 && p.x()<m_qImage->width() && p.y()>=0 && p.y()<m_qImage->height()) {
        if (!usePens)
            m_qImage->setPixel(p.x(),p.y(),QRgb(color));
        else
            m_qImage->setPixel(p.x(),p.y(), QRgb(m_colorList.getPenIndex(color)));
    }
}



unsigned int LImageQImage::getPixel(int x, int y)
{
    if (m_qImage==nullptr)
        return 0;


    QPoint p = getPixelPosition(x,y);

    if (p.x()>=0 && p.x()<m_qImage->width() && p.y()>=0 && p.y()<m_qImage->height())
        return m_qImage->pixel(p.x(),p.y());


    return 0;
}

void LImageQImage::Release()
{
    if (m_qImage!=nullptr) {
        delete m_qImage;
    }
    m_qImage = nullptr;
}



QImage* LImageQImage::Resize(int x, int y, LColorList& lst, float gamma, float shift, float hsvShift, float sat, QPointF scale, bool useDither)
{
    if (m_qImage==nullptr)
        return nullptr;
    QImage* other = new QImage(x,y,QImage::Format_ARGB32);
    float aspect = m_qImage->width()/(float)m_qImage->height();
    float m = std::max(m_qImage->width(), m_qImage->height());
    float sx = (float)x/m;
    float sy = (float)y/m;//*aspect;
    hsvShift-=0.5;
    sat-=0.5;
    int addy=0;// -100/sx/2;
    int addx = 0;
    if (m_qImage->width()>m_qImage->height()) {
        int d = m_qImage->width()-m_qImage->height();
        addy=-d/2;
    }
    if (m_qImage->width()<m_qImage->height()) {
        int d = m_qImage->height()-m_qImage->width();
        addx=-d/2;
    }

    float aspectX = 1;
    float aspectY = 1.4*0.5;//1/aspect;
    if (aspect<1) {
        aspectX = 1*(1.4);
        aspectY = 1;
    }
    aspectY = 1.0/aspect;
    QColor black(0,0,0);
    //    qDebug() << sx << sy << aspectX << aspectY;
    //#pragma omp parallel for
    for (int i=0;i<x;i++)
        for (int j=0;j<y;j++) {
            QColor color = black;


            int xx = ((i-x/2.0)*aspectX + x/2.0)/sx + addx;
            int yy = ((j-y/2.0)*aspectY + y/2.0)/sy + addy;

            //            int xx = ((i-x/4)*1.4 + x/4)/sx + addx;
            //            int yy = j/sy + addy;

            if (xx>=0 && xx<m_qImage->width() && yy>=0 && yy<m_qImage->height())
                color = QColor(m_qImage->pixel(xx,yy));

            QColor org = color;
            //color = color.toHsv();


            QColor r = color.toHsl();
            float hsv = Util::clamp(r.hslHue()+(hsvShift*360),0,359);
            float saturation = Util::clamp(r.hslSaturation()+(sat*255),0,255);

            color = QColor::fromHsl(hsv, saturation, r.lightness());
            QVector3D v = Util::fromColor(color);

            v.setX( pow(fmax(color.red() + shift,0), gamma));
            v.setY( pow(fmax(color.green() + shift,0), gamma));
            v.setZ( pow(fmax(color.blue()+ shift,0), gamma));

            v = Util::clamp(v,0,255);
            int winner = 0;
            QColor newCol;
            if (useDither)
                newCol = Util::toColor(v);
            else
                newCol = lst.getClosestColor(Util::toColor(v),winner);
            //           QColor newCol = lst.getClosestColor(org);

            other->setPixel(i,j,newCol.rgb());

        }


    return other;
}

QImage *LImageQImage::Blur(float blurRadius)
{
    if (blurRadius==0) {
        QImage* q = new QImage();
        *q = m_qImage->copy();
        return q;
    }
    QGraphicsBlurEffect* blur = new QGraphicsBlurEffect();
    blur->setBlurRadius((int)((m_qImage->width())*blurRadius/10.0));
    return ApplyEffectToImage(*m_qImage, blur);

}

void LImageQImage::ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center)
{
    //#pragma omp parallel for
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {

            float xp = ((i-center.x())*zoom)+ center.x();
            float yp = ((j-center.y())*zoom) + center.y();

            unsigned int col = getPixel(xp,yp);// % 16;

            //            img->setPixel(i,j,QRgb(col));
            img.setPixel(i,j,lst.get(col).color.rgb());
        }
    //return img;
}

void LImageQImage::fromQImage(QImage *img, LColorList &lst)
{
    //#pragma omp parallel for
    for (int i=0;i<m_qImage->width();i++)
        for (int j=0;j<m_qImage->height();j++) {
            unsigned char col = lst.getIndex(QColor(img->pixel(i, j)));
            m_qImage->setPixel(i,j,col);
        }
}
