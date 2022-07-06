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

#include "toolboxitem.h"
#include <QDebug>
#include <QPixmap>
#include "source/LeLib/limage/limagefactory.h"
ToolboxItem::ToolboxItem()
{

}

void ToolboxItem::setSize(float f)
{
    m_size = f;
}


ToolboxItem::ToolboxItem(QString name, QString imagefile, QString tooltip)
{
    m_name= name;
    m_imageFilename = imagefile;
    m_tooltip = tooltip;
    if (m_imageFilename!="") {
        m_image.load(m_imageFilename);
        m_icon.addPixmap( QPixmap::fromImage(m_image) );
    }


}

void ShapeBox::Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button)
{
    float m= m_size;
    float ll = 0.66;
    if (button==-1)
        return;
    MultiColorImage* mci = dynamic_cast<MultiColorImage*>(img);
    if (mci!=nullptr) {
        if (mci->isMultiColor())
            ll = 2.4;
    }
    float lw = m_options["lwidth"]->val/10.0;

/*    int sx = fmax(img->m_footer.get(LImageFooter::POS_CURRENT_STAMP_X),1);
    int sy = fmax(img->m_footer.get(LImageFooter::POS_CURRENT_STAMP_X),1);
    int w = fmax(img->m_footer.get(LImageFooter::LImageFooter::POS_CHARSET_WIDTH),1);

    int currentChar = img->m_currentChar;
*/
    img->setBasePixel(x,y);
    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*ll + yy*yy);
    //        img->m_currentChar =currentChar+((i/8)%sx) + ((j/8)%sy)*w;

            bool ok = l<m/2.5;
            if (m_type==1)
                ok = abs(l-m/3)<lw;

            if (ok)
                img->setPixel(x+xx,y+yy,color);
        }

  //  img->m_currentChar = currentChar;
}

void ShapeBoxFilter::Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button)
{
    float m= m_size;
    if (button==-1)
        return;
    img->setBasePixel(x,y);
    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);

            bool ok = l<m/2.5;
            if (m_type==1)
                ok = abs(l-m/3)<0.5;

            if (ok)
            {
                bool regular=true;
    //            if (rand()%100>98)// for (int i=0;i<4;i++)
  //                  qDebug() << img->m_extraCols[2];
/*                if (img->getBackground()!=img->m_extraCols[2]) {
//                    qDebug() << img->m_extraCols[2];
                    regular = false;
                }
*/

                if (regular) {
                    if (img->getPixel(x+xx,y+yy)!=img->getBackground())
                        img->setPixel(x+xx,y+yy,color);
                }
  //              else
    //                if (img->getPixel(x+xx,y+yy)==img->m_extraCols[2])
      //                  img->setPixel(x+xx,y+yy,color);
            }
        }
}




void Spray::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    float m= m_size;
    img->setBasePixel(x,y);
    if (button==-1)
        return;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);
            if (l<m/2.5) {
                if (rand()%100<2)
                    img->setPixel(x+xx,y+yy,color);
            }
        }

}

void Dither::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    float m= m_size;
    int shift = m_type;
    if (button==-1)
        return;

    x = ((int)x/2)*2;
    y = ((int)y/2)*2;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = (i-d);
            float yy = (j-d);
            float l = sqrt(xx*xx*img->m_scaleX + yy*yy);
            if (l<m/2.5) {
                int s = (int)(xx + (int)yy%2)%2;
                if (s == 0 ) {
                    img->setPixel(x+xx+shift,y+yy,color);
                }
            }
        }

}

void Filler::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
//    if (isPreview)
  //      return;
    img->setBasePixel(x,y);
    unsigned char testCol = img->getPixel(x,y);
    if (color == testCol)
        return;
    Fill(x,y,color, testCol, img);
}

void Filler::Fill(int i, int j, unsigned char col, unsigned char testCol, LImage *img)
{
    if (i<0 || j<0 || j>=img->m_height || i>=img->m_width)
        return;

    QVector<QPoint> queue;
    queue.append(QPoint(i,j));
    int maxx = img->m_width*img->m_height;
    QVector<QPoint> done;
    QByteArray mark;
    mark.resize(maxx);
    mark.fill(0);
    int dx = img->getDx();
    int dy = img->getDy();

    while (queue.count()!=0 && queue.count()<2*maxx) {
        QPoint q = queue.last();
        queue.removeLast();
  //      if (rand()%1000>990)
//        qDebug() << queue.count() << q << img->m_width << img->m_height << QString::number(testCol) << QString::number(img->getPixel(q.x(),q.y()));

//        if (QColor(m_index->getPixel(q.x(),q.y())) == unset) {
  //          m_index->setPixel(q.x(),q.y(),set.rgba());
        int pos = q.x() + q.y()*img->m_width;
//        qDebug() << pos << maxx <<q << img->m_width << img->m_height;

        if (testCol == img->getPixel(q.x(),q.y()) && mark.at(pos)==0) {
            img->setPixel(q.x(),q.y(), col);
            done.append(q);
            mark[pos]=1;

            if (q.x()<img->m_width-dx)
                queue.append(QPoint(q.x()+dx,q.y()));
            if (q.x()>=dx)
                 queue.append(QPoint(q.x()-dx,q.y()));
            if (q.y()<img->m_height-dy)
                 queue.append(QPoint(q.x(),q.y()+dy));
            if (q.y()>=dy)
                 queue.append(QPoint(q.x(),q.y()-dy));
            }
        }

  //  qDebug() << "******************** DONE";

}

/*
 *
 * Fill old
void Filler::Fill(int i, int j, unsigned char col, unsigned char testCol, LImage *img)
{
    if (i<0 || j<0 || j>=img->m_height || i>=img->m_width)
        return;
    if (testCol == img->getPixel(i,j)) {
        img->setPixel(i,j, col);
        if (i<img->m_width-1)
            Fill(i+1,j, col, testCol, img);
        if (i>0)
            Fill(i-1,j, col, testCol, img);
        if (j<img->m_height-1)
            Fill(i,j+1, col, testCol, img);
        if (j>0)
            Fill(i,j-1, col, testCol, img);
    }

}
*/

void Line::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
//    if (button==-1)
  //      return;
    img->setBasePixel(x,y);
    if (button==0) {
        m_start = QPoint(x,y);
        return;
    }
    if (m_type==1) {
        if (abs(m_start.x()-x) > abs(m_start.y()-y)) {
            y = m_start.y();
        }
        else x = m_start.x();
    }

    img->drawLine(m_start.x(), m_start.y(), x,y, color, m_size);


}

void CopyStamp::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    img->setBasePixel(x,y);

    if (button==1 && m_status == Status::Idle) {
        m_status = Status::Selecting;
        m_start = QPoint(x,y);
       // qDebug() << "Start1";
        if (m_copy == nullptr)
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);
        // New from source
        if (m_copy->m_type!=img->m_type) {
            delete m_copy;
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);
        }
     //   qDebug() << "Start2";
        m_copy->CopyFrom(img);
    }

    unsigned int frameCol = 1;

    if (m_status == Status::Selecting && button==1) {
        m_end = QPoint(x,y);
        if (m_type!=1) {
            img->drawLine(m_start.x(), m_start.y(), m_start.x(), m_end.y(), frameCol, 1);
            img->drawLine(m_start.x(), m_start.y(), m_end.x(), m_start.y(), frameCol, 1);
            img->drawLine(m_start.x(), m_end.y(), m_end.x(), m_end.y(), frameCol, 1);
            img->drawLine(m_end.x(), m_start.y(), m_end.x(), m_end.y(), frameCol, 1);
        }
        else // cut
 //           if (isPreview)
            for (int i=m_start.x();i<m_end.x();i++)
                for (int j=m_start.y();j<m_end.y();j++) {
                    img->setPixel(i,j,Data::data.currentColor);
                }
    }

    if (m_status== Status::Stamp && button!=1) {
        StampImage(x,y, img);
    }

    if (button==-1 && m_status==Status::Selecting) {
        m_status = Status::Stamp;
        if (m_type==1)
            for (int i=m_start.x();i<m_end.x();i++)
                for (int j=m_start.y();j<m_end.y();j++) {
                    img->setPixel(i,j,Data::data.currentColor);
                }
    }


}

CopyStamp::CopyStamp() {
    m_status=Idle;
}

CopyStamp::CopyStamp(QString name, QString imagefile, QString tooltip, int type) : ToolboxItem(name, imagefile,tooltip) {
    m_type=type;
}

LImage* CopyStamp::m_copy=nullptr;
QPoint CopyStamp::m_start;
QPoint CopyStamp::m_end;

CopyStamp::Status CopyStamp::m_status;// = Idle;


void CopyStamp::StampImage(int x, int y, LImage* img)
{
    if (m_copy == nullptr)
        return;
    int w = abs(m_end.x()-m_start.x());
    int h = abs(m_end.y()-m_start.y());
    img->setBasePixel(x,y);
    for (int i=0;i<w;i++)
        for (int j=0;j<h;j++) {
            unsigned int col = m_copy->getPixel(m_start.x() + i-0.5, m_start.y()+j-0.5);
            if (m_type==1)
                img->setPixel(i-w/2+x,j-h/2+y, col);
            else
            if (col!=Data::data.currentColor)
                img->setPixel(i-w/2+x,j-h/2+y, col);
//                for (int xd=0;xd<m_copy->m_scale;xd++)
  //                  img->setPixel(i-w/2.0+x + xd,j-h/2.0+y, col);
        }
}

void CopyStamp::Init() {
    m_status = Status::Idle;
    if (m_type==2) // paste only
        m_status = Status::Stamp;
}



void RotateAround::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    img->setBasePixel(x,y);
    if (button==1 && m_status == Status::Idle) {
        m_status = Status::Down;
        m_start = QPoint(x,y);
       // qDebug() << "Start1";
        if (m_copy==nullptr)
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);
        // New from source
        if (m_copy->m_type!=img->m_type) {
            delete m_copy;
            m_copy = LImageFactory::Create(img->m_type, img->m_colorList.m_type);

        }
     //   qDebug() << "Start2";
        m_copy->CopyFrom(img);
    }


    if (m_status == Status::Down && button==1) {
        m_end = QPoint(x,y)-m_start;
        float angle = m_end.x()/10.0f;
        m_copy->Rotate(m_start, angle, 1, img);

    }

    if (m_status == Status::Down && button==0) {
        float angle = m_end.x()/10.0f;
        m_status = Status::Idle;
        m_copy->Rotate(m_start, angle, 1, img);

    }

}

void Circle::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    img->setBasePixel(x,y);
    if (button==0) {
        m_start = QPoint(x,y);
        return;
    }
//    if (m_type==1) {
        int dx = m_start.x() - x;
        int dy = m_start.y() - y;

        int r = sqrt(dx*dx + dy*dy);

  //  }p
    int innerR = -1;
    if (m_type==1)
        innerR = r-4;
    if (r>0)
        img->drawCircle(m_start.x(), m_start.y(), r, innerR,color);


}

void Box::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    img->setBasePixel(x,y);
    if (button==0) {
        m_start = QPoint(x,y);
        return;
    }
    int width = 0;
    if (m_type ==1)
        width = 1;

    img->RBox(m_start.x(), m_start.y(), x,y, color, width);


}

void WetBrush::Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button)
{
    img->setBasePixel(x,y);
    float m= m_size;
    float ll = 0.66;
    MultiColorImage* mci = dynamic_cast<MultiColorImage*>(img);
    if (mci!=nullptr) {
        if (mci->isMultiColor())
            ll = 2.4;
    }

    if (curCol != color) {
        prevCol = curCol;
        curCol = color;
    }

    if (button==0) {
        len = maxLen;
    }

    float r = len / (float)maxLen;
//    qDebug() << "TOOLBOX " <<len << button;

    int val = 100-m_options["length"]->val;

    if (len>0 && !isPreview)
        len-=val/4;

    if (len<0) len = 0;


/*    float matrix[16] = {
    0, 4, 1, 5,
    8, 12, 9, 13,
    2, 6, 3, 7,
    10, 6, 11, 15};
*/
    float matrix[64] = {
     0, 32, 8, 40, 2, 34, 10, 42, /* 8x8 Bayer ordered dithering */
    48, 16, 56, 24, 50, 18, 58, 26, /* pattern. Each input pixel */
    12, 44, 4, 36, 14, 46, 6, 38, /* is scaled to the 0..63 range */
    60, 28, 52, 20, 62, 30, 54, 22, /* before looking in this table */
     3, 35, 11, 43, 1, 33, 9, 41, /* to determine the action. */
    51, 19, 59, 27, 49, 17, 57, 25,
    15, 47, 7, 39, 13, 45, 5, 37,
    63, 31, 55, 23, 61, 29, 53, 21 };


    QVector3D c1 = Util::fromColor(img->m_colorList.get(color).color);
    QVector3D c2 = Util::fromColor(img->m_colorList.get(prevCol).color);


//    r = r/2 + 0.25;

    QVector3D c3 = r*c1 + (1-r)*c2;

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {

            float mval = matrix[(i&7) + (j&7)*8]/4.0;
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*ll + yy*yy);

            bool ok = l<m/2.5;


            if (m_type==1)
                ok = abs(l-m/3)<0.75;

            if (ok)
            {
                QColor cc = Util::toColor(c3);

                cc.setRed(std::min((float)cc.red() + mval,255.0f));
                cc.setGreen(std::min((float)cc.green()+ mval,255.0f));
                cc.setBlue(std::min((float)cc.blue() + mval,255.0f));
                int winner = 0;
                img->m_colorList.getClosestColor(cc, winner);

                img->setPixel(x+xx,y+yy,winner);
            }
        }
}


void Smooth::Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button)
{
    float m= m_size;
    float ll = 0.66;
    MultiColorImage* mci = dynamic_cast<MultiColorImage*>(img);
    if (mci!=nullptr) {
        if (mci->isMultiColor())
            ll = 2.4;
    }
    float str = m_options["strength"]->val/50.0;
    LImageQImage small(img->m_colorList.m_type);
    small.Initialize(m,m);
    small.Clear();

    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            LColor& c = img->m_colorList.get(img->getPixel(i+x-m/2,j+y-m/2));
            QColor col = c.color;
        //    col = Qt::red;
            small.setPixel(i,j,col.rgb());
       }
    small.m_qImage = small.Blur(1);
    str = Util::clamp(str,0,1);
    for (int i=0;i<m;i++)
        for (int j=0;j<m;j++) {
            int d = m/2;
            float xx = i-d;
            float yy = (j-d);
            float l = sqrt(xx*xx*ll + yy*yy);

            bool ok = l<m/2.8;
//            if (m_type==1)
  //              ok = abs(l-m/3)<lw;
            if (ok)
            {
                QColor c1 = small.getPixel(i,j);
                QColor c2 = img->m_colorList.get(img->getPixel(x+xx,y+yy)).color;
                QColor mix = Util::toColor(Util::fromColor(c1)*str + Util::fromColor(c2)*(1-str));
                int c;
                img->m_colorList.getClosestColor(mix,c);
 //               qDebug() << "Setting color : "<<c;
                img->setPixel(x+xx,y+yy, c);
            }
        }
}

void ToolBoxItemOption::Build(QGridLayout* gl, int row) {
    QLabel* l = new QLabel(name);
    QSlider* sl = new QSlider(Qt::Horizontal);
    QLabel* v = new QLabel(QString::number(val));
    sl->setMaximum(tmax);
    sl->setMinimum(tmin);
    sl->setValue(val);
    QObject::connect( sl, &QSlider::sliderMoved,  [=](){
        val = sl->value();
        v->setText(QString::number(val));
    } );
    gl->addWidget(l,row,0);
    gl->addWidget(sl,row+1,0);

    QPushButton* help = new QPushButton("?");
    help->setMaximumWidth(16);
    gl->addWidget(help,row,1);

    gl->addWidget(v,row+1,1);

}

void ToolBoxItemOptionFileList::Build(QGridLayout *gl, int row)
{
    QDir directory(m_path);
    QStringList images = directory.entryList(QStringList() << "*."+m_fileType,QDir::Files);
    QStringList names;
    for (QString s : images) {
        s = s.split(QDir::separator()).last();
        names<< s.split(".").first();

    //do whatever you need to do
    }
    QComboBox* cmb = new QComboBox();
    cmb->addItems(names);
    QObject::connect( cmb, &QComboBox::currentTextChanged,  [=](){
        QString fn =cmb->currentText();
        m_img->load(":resources/images/shapes/"+fn + ".png");
    } );
    for (int i=0;i<names.count();i++) {
        QIcon icon(":resources/images/shapes/"+names[i]+".png");
        cmb->setItemIcon(i,icon);
    }


    gl->addWidget(cmb,row,0);
}

void ShapePNGColor::Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button)
{
    img->setBasePixel(x,y);
    float m= m_size;
//    float str = m_options["strength"]->val/50.0;
    QImage scaled = m_shape.scaled(m_size, m_size,Qt::KeepAspectRatio, Qt::TransformationMode::SmoothTransformation);

    for (int i=0;i<scaled.width();i++)
        for (int j=0;j<scaled.height();j++) {
            QColor c = scaled.pixelColor(i,j);
            if (c.red()<200) {
                int col;
                //img->m_colorList.getClosestColor(c,col);
                img->setPixel(x+i-scaled.width()/2, j+y-scaled.height()/2,color);
            }
       }
}
