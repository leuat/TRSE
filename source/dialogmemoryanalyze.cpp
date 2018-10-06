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

DialogMemoryAnalyze::DialogMemoryAnalyze(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMemoryAnalyze)
{
    ui->setupUi(this);
}

void DialogMemoryAnalyze::Initialize(QVector<MemoryBlock*> &blocks, int fontSize)
{
    m_blocks = blocks;
    m_fontSize = fontSize;

    fontSize/=2;

    InitColors();
    float xsize = ui->lblImage->width()-2;
    float ysize= ui->lblImage->height()-8;
/*    float xsize=this->width()*2;
    float ysize=this->height()*2;*/
    QImage img(QSize(xsize,ysize), QImage::Format_ARGB32);

    img.fill(QColor(80,110,80));
    int xstart = xsize/4;
    int ww = xsize/5;
    int xborder = 40;

    QPainter p;
    p.begin(&img);
    int i=0;
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
        for (int y=y0;y<y1;y++)
            for (int x=xstart;x<xsize-xborder-ww;x++)
                img.setPixel(x,y,c.rgba());

        int box2s = ww;
        float s2 = 0.75f;
        c.setRed(min(c.red()*scale*s2,255.0f));
        c.setGreen(min(c.green()*scale*s2,255.0f));
        c.setBlue(min(c.blue()*scale*s2,255.0f));


        for (int y=y0;y<y1;y++)
            for (int x=xsize-xborder-box2s;x<xsize-xborder;x++)
                img.setPixel(x,y,c.rgba());


        int box1 = xsize-xstart-xborder-box2s;
        int box2 = xsize-xborder-box2s;

        p.setPen(QPen(QColor(32,32,48)));
        p.setFont(QFont("Arial", fontSize, QFont::Bold));
        p.drawText(QRect(xstart, y0,box1, y1-y0), Qt::AlignCenter, mb->m_name);

        QString f = "$"+QString::number(mb->m_start,16).rightJustified(4, '0');
        QString t = "$"+QString::number(mb->m_end,16).rightJustified(4, '0');

        p.drawText(QRect(xstart, y0,box1, y1-y0), Qt::AlignLeft|Qt::AlignTop, f + " - " + t);

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
        p.setFont(QFont("Arial", fontSize-1, QFont::Bold));

        p.drawText(QRect(xsize-xborder-box2s+12, y0,box2, y1-y0), Qt::AlignLeft|Qt::AlignTop, zp);



        i++;

    }
    for (int i=0;i<16;i++) {
        QString v = "$"+QString::number(i*4096,16).rightJustified(4, '0');
        int y0=(ysize/16)*i;
        int y1=(ysize/16)*(i+1);
        p.drawText(QRect(0, y0,xstart, y1), Qt::AlignLeft|Qt::AlignTop, v);
        int ysize=1;
        if (i%4==0) ysize=4;
        for (int y=y0;y<y0+ysize;y++)
            for (int x=0;x<xsize;x++)
                img.setPixel(x,y,QColor(0,0,0).rgba());
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

void DialogMemoryAnalyze::on_btnClose_clicked()
{
    close();
}
