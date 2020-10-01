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

#include "dialogmemoryanalyze.h"
#include "ui_dialogmemoryanalyze.h"
#include "source/LeLib/util/util.h"

DialogMemoryAnalyze::DialogMemoryAnalyze(QSharedPointer<CIniFile> ini, AbstractSystem* system,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMemoryAnalyze)
{
    ui->setupUi(this);
    m_system = system;
    m_iniFile = ini;
}


void DialogMemoryAnalyze::RenderSystemLabels(QPainter& p, int xstart, int fs )
{
    int xlstart = 100;
    int xlend = xstart+50;

    for (SystemLabel l:m_system->m_labels) {

        float y0 = (l.m_from/(float)m_system->m_memorySize)*ysize;
        float y1 = (l.m_to/(float)m_system->m_memorySize)*ysize;
        QColor c = AbstractSystem::m_labelColors[l.m_type];

        int height= y1-y0;

        QRect r = Trans(QRect(xlstart, y0,xlend, height));
        QRect r2 = Trans(QRect(xlstart, y0+shift,xlend, height));
        if ((r2).contains(mpos)) {
            curT = l.m_name;
            cur.setX(l.m_from);
            cur.setY(l.m_to);
        }
        p.setPen(c);
        p.setBrush(QBrush(c,Qt::Dense4Pattern));

        p.drawRoundedRect(r,2,2);

        p.setPen(QPen(QColor(32,32,48)));

        p.setFont(QFont("Courier", min(fs,height)));
        p.drawText(r, Qt::AlignTop | Qt::AlignLeft, l.m_name);
    }


}


void DialogMemoryAnalyze::Initialize(QVector<QSharedPointer<MemoryBlock>> &blocks, int fontSize)
{
    m_blocks = blocks;
    m_fontSize = fontSize;
    setMouseTracking(true);
    ui->lblImage->setMouseTracking(true);
    xsize = ui->lblImage->width()-8;
    ysize= ui->lblImage->height()-8;

    fontSize/=2;

    InitColors();




    float xsize = ui->lblImage->width()-8;
    float ysize= ui->lblImage->height()-8;
/*    float xsize=this->width()*2;
    float ysize=this->height()*2;*/
    QImage img(QSize(xsize,ysize), QImage::Format_ARGB32);

    img.fill(m_system->m_systemColor);
    curT="";
    int xstart = xsize/3;
    int ww = xsize/5;
    int xborder = 40;
    mpos = ui->lblImage->mapFromGlobal(QCursor::pos());
    mpos.setY(mpos.y());
    int i=0;
    int round = 5;
    shift = 6;

    QPainter p;
    p.begin(&img);


    RenderSystemLabels(p,xstart,fontSize);

    for (QSharedPointer<MemoryBlock> mb:blocks) {
        float y0 = (mb->m_start/(float)m_system->m_memorySize)*ysize;
        float y1 = (mb->m_end/(float)m_system->m_memorySize)*ysize;
        QColor c = m_colors[mb->Type()];
        //float scale=(0.5 + (rand()%100/100.0));
        float scale = 0.9;
        if (i%2==1) scale=1.2f;
        c.setRed(min(c.red()*scale,255.0f));
        c.setGreen(min(c.green()*scale,255.0f));
        c.setBlue(min(c.blue()*scale,255.0f));
        p.setPen(c);
        p.setBrush(c);


        int x1 = xstart;
        int x2 = xsize;

        if (mb->m_bank>=0) {
            int dx = xsize/m_noBanks;
            x1 = xstart + dx/2.0 * mb->m_bank;
            x2 = dx*1.5+x1;
         //   qDebug() << mb->m_bank <<mb->m_end;
        }


        p.drawRoundedRect(Trans(QRect(x1,y0,x2-xborder-x1-ww,y1-y0)),round,round);

        int box2s = ww;
        float s2 = 0.75f;
        c.setRed(min(c.red()*scale*s2,255.0f));
        c.setGreen(min(c.green()*scale*s2,255.0f));
        c.setBlue(min(c.blue()*scale*s2,255.0f));

        QRect r = Trans(QRect(x1,y0+shift,x2-xborder,y1-y0));
        if (r.contains(mpos)) {
            curT = mb->m_name;
            cur.setX(mb->m_start);
            cur.setY(mb->m_end);
        }



        p.setPen(c);
        p.setBrush(c);
        if (mb->m_bank<0)
            p.drawRoundedRect(Trans(QRect(x2-xborder-box2s,y0,x2-xborder,y1-y0)),round,round);

        int box1 = x2-x1-xborder-box2s;
        int box2 = x2-xborder-box2s;
        int height= y1-y0;
        p.setPen(QPen(QColor(32,32,48)));
        p.setFont(QFont("Courier", min(fontSize,height), QFont::Bold));
        p.drawText(Trans(QRect(x1, y0,box1, height)), Qt::AlignCenter, mb->m_name);

        QString f = "$"+QString::number(mb->m_start,16).rightJustified(4, '0');
        QString t = "$"+QString::number(mb->m_end,16).rightJustified(4, '0');

        p.drawText(Trans(QRect(x1, y0,box1, height)), Qt::AlignLeft|Qt::AlignTop, f + " - " + t);

        // Zeropages
        QString zp = "";
        int cnt=0;
        for (int i: mb->m_zeropages) {
            zp+=Util::numToHex(i) + " ";
            if (cnt++==5) { zp+="\n"; cnt=0; }
        }
        if (mb->m_zeropages.count()!=0)
            zp = "zp :"+zp;
        zp=zp.trimmed();
        p.setFont(QFont("Courier", min(fontSize,height), QFont::Bold));

        p.drawText(Trans(QRect(x2-xborder-box2s+12, y0,box2, y1-y0)), Qt::AlignLeft|Qt::AlignTop, zp);



        i++;

    }
    QColor c(0,0,0);
    p.setPen(QColor(0,0,0,0));
    p.setBrush(QBrush(c,Qt::Dense4Pattern));
    // Render lines
    for (int i=0;i<16;i++) {
        QString v = "$"+QString::number(i*m_system->m_memorySize/16,16).rightJustified(4, '0');
        int y0=(ysize/16)*i;
        int y1=(ysize/16)*(i+1);
        p.setPen(QColor(0,0,0,255));
        p.drawText(Trans(QRect(0, y0,xstart, y1)), Qt::AlignLeft|Qt::AlignTop, v);
        p.setPen(QColor(0,0,0,0));
        int ysize=1;
        if (i%4==0) ysize=4;
        p.drawRect(Trans(QRect(0,y0,xsize,ysize)));

    }




    c = QColor(0,0,0,200);
    p.setPen(QColor(100,255,255));
    p.setBrush(c);
    mpos.setX(mpos.x()+16);
    int width = 800;
    // Render "tooltip" text
    if (curT!="") {
       p.drawRoundedRect(QRect(mpos.x(),mpos.y(),width,60),round,round);
       c = QColor(100,220,255,255);
       p.setPen(c);
//       p.setBrush(c);
       p.setFont(QFont("Courier", 15, QFont::Bold));
       p.drawText(QRect(mpos.x()+180,mpos.y(),width,60), curT);
       p.setFont(QFont("Courier", 12, QFont::Bold));
       c = QColor(80,130,255,255);
       p.setPen(c);

       QString address = "$"+QString::number(cur.x(),16).rightJustified(4, '0');
       address += " - $"+QString::number(cur.y(),16).rightJustified(4, '0');
       p.drawText(QRect(mpos.x(),mpos.y()+4,500,60), address);
    }



    p.end();




    QPixmap pm;
    pm.convertFromImage(img);
    ui->lblImage->setPixmap(pm.scaled(xsize,ysize, Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    ui->lblImage->setPixmap(pm);

    VerifyZPMusic(blocks);
}

void DialogMemoryAnalyze::InitColors()
{
    m_colors.clear();
    m_colors["code"]=QColor(255,127,127);
    m_colors["music"]=QColor(255,255,127);
    m_colors["data"]=QColor(127,127,255);
    m_colors["user"]=QColor(127,255,255);
    m_colors["array"]=QColor(255,127,255);
}


void DialogMemoryAnalyze::resizeEvent(QResizeEvent *e)
{
    Initialize(m_blocks, m_fontSize);
    m_iniFile->setFloat("memory_analyzer_window_width", this->size().width());
    m_iniFile->setFloat("memory_analyzer_window_height", this->size().height());
}

void DialogMemoryAnalyze::VerifyZPMusic(QVector<QSharedPointer<MemoryBlock>> &blocks)
{
    QVector<QSharedPointer<MemoryBlock>> music;
    for (QSharedPointer<MemoryBlock> mb : blocks) {
        if (mb->m_type == MemoryBlock::MUSIC)
            music.append(mb);
    }
    QString infoText="";
    for (QSharedPointer<MemoryBlock> mb: music) {
        bool overlaps=false;
        QString overlapString="";
        for (QSharedPointer<MemoryBlock> o : blocks)
            if (o!=mb) {
                for (int j: o->m_zeropages) {

                    if (mb->m_zeropages.contains(j)) {
                        overlaps = true;
                        overlapString += Util::numToHex(j)+" ";
                    }
                }


            }
        if (overlaps) {
            infoText += "<font color=\"#FF4040\">WARNING</font>:";
            infoText += "Music track '" + mb->m_name +"' has overlapping zero pages with code : " + overlapString;
        }

    }
    ui->leInfoText->setText(infoText);
}


DialogMemoryAnalyze::~DialogMemoryAnalyze()
{
    delete ui;
}

void DialogMemoryAnalyze::mouseMoveEvent(QMouseEvent *event)
{
    Initialize(m_blocks, m_fontSize);
    zoomCenter = this->mapFromGlobal(QCursor::pos());

}

void DialogMemoryAnalyze::wheelEvent(QWheelEvent *event)
{
    zoomVal +=0.002f*event->delta();
    zoomVal = max(1.0f, zoomVal);
}

QPoint DialogMemoryAnalyze::Trans(QPoint p)
{
    return (p-zoomCenter)*zoomVal + zoomCenter;
}

QRect DialogMemoryAnalyze::Trans(QRect r)
{
    QPoint p = Trans(QPoint(r.x(),r.y()));
    return QRect(p.x(),p.y(),r.width()*zoomVal,r.height()*zoomVal);
}

void DialogMemoryAnalyze::on_btnClose_clicked()
{
    m_iniFile->Save();
    close();
}
