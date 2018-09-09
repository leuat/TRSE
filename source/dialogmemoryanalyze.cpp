#include "dialogmemoryanalyze.h"
#include "ui_dialogmemoryanalyze.h"

DialogMemoryAnalyze::DialogMemoryAnalyze(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogMemoryAnalyze)
{
    ui->setupUi(this);
}

void DialogMemoryAnalyze::Initialize(QVector<MemoryBlock> &blocks)
{
    InitColors();
    float ysize=1600;
    float xsize = 1600;
    QImage img(QSize(xsize,ysize), QImage::Format_ARGB32);

    img.fill(QColor(80,110,80));
    int xstart = 400;
    int xborder = 40;

    QPainter p;
    p.begin(&img);
    int i=0;
    for (MemoryBlock& mb:blocks) {
        float y0 = (mb.m_start/65535.0)*ysize;
        float y1 = (mb.m_end/65535.0)*ysize;
        QColor c = m_colors[mb.Type()];
        //float scale=(0.5 + (rand()%100/100.0));
        float scale = 0.9;
        if (i%2==1) scale=1.2f;
        c.setRed(min(c.red()*scale,255.0f));
        c.setGreen(min(c.green()*scale,255.0f));
        c.setBlue(min(c.blue()*scale,255.0f));
        for (int y=y0;y<y1;y++)
            for (int x=xstart;x<xsize-xborder;x++)
                img.setPixel(x,y,c.rgba());


        p.setPen(QPen(QColor(32,32,48)));
        p.setFont(QFont("Times", 16, QFont::Bold));
        p.drawText(QRect(xstart, y0,xsize-xstart-xborder, y1-y0), Qt::AlignCenter, mb.m_name);

        QString f = "$"+QString::number(mb.m_start,16).rightJustified(4, '0');
        QString t = "$"+QString::number(mb.m_end,16).rightJustified(4, '0');

        p.drawText(QRect(xstart, y0,xsize-xstart-xborder, y1-y0), Qt::AlignLeft|Qt::AlignTop, f + " - " + t);
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
    ui->lblImage->setPixmap(pm);


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


DialogMemoryAnalyze::~DialogMemoryAnalyze()
{
    delete ui;
}

void DialogMemoryAnalyze::on_btnClose_clicked()
{
    close();
}
