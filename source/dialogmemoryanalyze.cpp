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

DialogMemoryAnalyze::DialogMemoryAnalyze(CIniFile* ini, AbstractSystem* system,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMemoryAnalyze)
{
    ui->setupUi(this);
    m_system = system;
    m_iniFile = ini;
}

void DialogMemoryAnalyze::Initialize(QVector<MemoryBlock*> &blocks, int fontSize)
{
    m_blocks = blocks;
    m_fontSize = fontSize;
    setMouseTracking(true);
    ui->lblImage->setMouseTracking(true);

    fontSize/=2;

    InitColors();
    float xsize = ui->lblImage->width()-2;
    float ysize= ui->lblImage->height()-8;
/*    float xsize=this->width()*2;
    float ysize=this->height()*2;*/
    QImage img(QSize(xsize,ysize), QImage::Format_ARGB32);

    if (Syntax::s.m_currentSystem==AbstractSystem::C64)
        img.fill(QColor(80,110,80));

    if (Syntax::s.m_currentSystem==AbstractSystem::VIC20)
        img.fill(QColor(80,110,200));

    int xstart = xsize/3;
    int ww = xsize/5;
    int xborder = 40;
    QPoint mpos = ui->lblImage->mapFromGlobal(QCursor::pos());
    mpos.setY(mpos.y());
    QPainter p;
    p.begin(&img);
    int i=0;
    int round = 5;
    int shift = 6;


    int xlstart = 100;
    int xlend = xstart+50;
    QString curT = "";
    QPoint cur;
    for (SystemLabel l:m_system->m_labels) {

        float y0 = (l.m_from/65535.0)*ysize;
        float y1 = (l.m_to/65535.0)*ysize;
        QColor c = AbstractSystem::m_labelColors[l.m_type];

        int height= y1-y0;

        QRect r(xlstart, y0,xlend, height);
        QRect r2(xlstart, y0+shift,xlend, height);
        if ((r2).contains(mpos)) {
            curT = l.m_name;
            cur.setX(l.m_from);
            cur.setY(l.m_to);
        }
        p.setPen(c);
        p.setBrush(QBrush(c,Qt::Dense4Pattern));

        p.drawRoundedRect(r,2,2);

        p.setPen(QPen(QColor(32,32,48)));

        p.setFont(QFont("Courier", min(fontSize,height), QFont::Bold));
        p.drawText(r, Qt::AlignTop | Qt::AlignLeft, l.m_name);



//        QToolTip::showText(QCursor::pos(), l.m_name + " ", this, QRect(xlstart, y0,xlend-xlstart, height), 5000);
//        QToolTip::showText(this->mapToGlobal( QPoint( 0, 0 ) ), l.m_name + " ", this, QRect(xlstart, y0,xlend-xlstart, height), 5000);
    }

//    QToolTip::showText(QCursor::pos(),  "BLAH ", this, QRect(0,0,800,600), 5000);


    for (MemoryBlock* mb:blocks) {
        float y0 = (mb->m_start/65535.0)*ysize;
        float y1 = (mb->m_end/65535.0)*ysize;
        QColor c = m_colors[mb->Type()];
        //float scale=(0.5 + (rand()%100/100.0));
        float scale = 0.9;
        if (i%2==1) scale=1.2f;
        c.setRed(min(c.red()*scale,255.0f));
        c.setGreen(min(c.green()*scale,255.0f));
        c.setBlue(min(c.blue()*scale,255.0f));
        p.setPen(c);
        p.setBrush(c);
        p.drawRoundedRect(QRect(xstart,y0,xsize-xborder-xstart-ww,y1-y0),round,round);

        int box2s = ww;
        float s2 = 0.75f;
        c.setRed(min(c.red()*scale*s2,255.0f));
        c.setGreen(min(c.green()*scale*s2,255.0f));
        c.setBlue(min(c.blue()*scale*s2,255.0f));

        QRect r = QRect(xstart,y0+shift,xsize-xborder,y1-y0);
        if (r.contains(mpos)) {
            curT = mb->m_name;
            cur.setX(mb->m_start);
            cur.setY(mb->m_end);
        }



        p.setPen(c);
        p.setBrush(c);
        p.drawRoundedRect(QRect(xsize-xborder-box2s,y0,xsize-xborder,y1-y0),round,round);

        int box1 = xsize-xstart-xborder-box2s;
        int box2 = xsize-xborder-box2s;
        int height= y1-y0;
        p.setPen(QPen(QColor(32,32,48)));
        p.setFont(QFont("Courier", min(fontSize,height), QFont::Bold));
        p.drawText(QRect(xstart, y0,box1, height), Qt::AlignCenter, mb->m_name);

        QString f = "$"+QString::number(mb->m_start,16).rightJustified(4, '0');
        QString t = "$"+QString::number(mb->m_end,16).rightJustified(4, '0');

        p.drawText(QRect(xstart, y0,box1, height), Qt::AlignLeft|Qt::AlignTop, f + " - " + t);





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

        p.drawText(QRect(xsize-xborder-box2s+12, y0,box2, y1-y0), Qt::AlignLeft|Qt::AlignTop, zp);



        i++;

    }
    QColor c(0,0,0);
    p.setPen(c);
    p.setBrush(c);
    for (int i=0;i<16;i++) {
        QString v = "$"+QString::number(i*4096,16).rightJustified(4, '0');
        int y0=(ysize/16)*i;
        int y1=(ysize/16)*(i+1);
        p.drawText(QRect(0, y0,xstart, y1), Qt::AlignLeft|Qt::AlignTop, v);
        int ysize=1;
        if (i%4==0) ysize=4;
        p.drawRect(QRect(0,y0,xsize,ysize));

    }


    c = QColor(0,0,0,200);
    p.setPen(QColor(100,255,255));
    p.setBrush(c);
    if (curT!="") {
       p.drawRoundedRect(QRect(mpos.x(),mpos.y(),500,70),round,round);
       c = QColor(100,220,255,255);
       p.setPen(c);
//       p.setBrush(c);
       p.setFont(QFont("Courier", 16, QFont::Bold));
       p.drawText(QRect(mpos.x(),mpos.y(),500,60), curT);
       p.setFont(QFont("Courier", 12, QFont::Bold));
       c = QColor(80,130,255,255);
       p.setPen(c);

       QString address = "$"+QString::number(cur.x(),16).rightJustified(4, '0');
       address += " - $"+QString::number(cur.y(),16).rightJustified(4, '0');
       p.drawText(QRect(mpos.x(),mpos.y()+40,500,60), address);
    }



    p.end();




    QPixmap pm;
    pm.convertFromImage(img);
    ui->lblImage->setPixmap(pm.scaled(xsize,ysize, Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    ui->lblImage->setPixmap(pm);
//label->setPixmap(p.scaled(w,h,Qt::KeepAspectRatio));

/*    for (MemoryBlock& mb:blocks) {
        QPushButton *b = new QPushButton();
        //b->setGeometry(0,0,40,40);
        QPalette p;
        QColor c=m_colors[mb.Type()];
        p.setColor(QPalette::Button, c);
        p.setColor(QPalette::Window, c);
        QString txtCol = QString::number(c.red()) + ", ";
        txtCol += QString::number(c.green()) + ", ";
        txtCol += QString::number(c.blue());

        b->setStyleSheet("background-color: rgb("+txtCol + "); color: rgb(0, 0, 0)");
        b->setPalette(p);
        //b->setMaximumWidth(40);
        //b->setMinimumWidth(40);
        b->setAutoFillBackground( true );
        //QObject::connect( b, &QPushButton::clicked,  colorValueChanged );


        ui->vLayout->addWidget(b);
    }
*/




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
 /*   qDebug() << "oldize: " <<e->oldSize();
    qDebug() << "new: " <<e->size();
    if (e->oldSize()!=e->size())*/
        Initialize(m_blocks, m_fontSize);
        m_iniFile->setFloat("memory_analyzer_window_width", this->size().width());
        m_iniFile->setFloat("memory_analyzer_window_height", this->size().height());
}

void DialogMemoryAnalyze::VerifyZPMusic(QVector<MemoryBlock*> &blocks)
{
    QVector<MemoryBlock*> music;
    for (MemoryBlock* mb : blocks) {
        if (mb->m_type == MemoryBlock::MUSIC)
            music.append(mb);
    }
    QString infoText="";
    for (MemoryBlock* mb: music) {
        bool overlaps=false;
        QString overlapString="";
        for (MemoryBlock* o : blocks)
            if (o!=mb) {
                for (int j: o->m_zeropages)
                    if (mb->m_zeropages.contains(j)) {
                        overlaps = true;
                        overlapString += Util::numToHex(j)+" ";
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

}

void DialogMemoryAnalyze::on_btnClose_clicked()
{
    m_iniFile->Save();
    close();
}
