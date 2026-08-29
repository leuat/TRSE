#include "dialogsizeanalyser.h"
#include "ui_dialogcrash.h"
#include <QPainter>
DialogCrash::DialogCrash(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCrash)
{
    ui->setupUi(this);
}

DialogCrash::~DialogCrash()
{
    delete ui;
}

void DialogCrash::Initialize(SourceBuilder *sb, CIniFile *ini, int fontSize)
{
    //m_sb = sb;
    m_ini = ini;
    m_fontSize = fontSize;
    setMouseTracking(true);
    ui->lblImage->setMouseTracking(true);
    xsize = ui->lblImage->width()-8;
    ysize= ui->lblImage->height()-8;
    fontSize/=2;

    InitColors();


    ReInitialise();

}

void DialogCrash::ReInitialise(){
    float xsize = ui->lblImage->width()-8;
    float ysize= ui->lblImage->height()-8;
/*    float xsize=this->width()*2;
    float ysize=this->height()*2;*/
    QImage img(QSize(xsize,ysize), QImage::Format_ARGB32);

    QString prevT = curT;
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
    time = time +1;

    QPixmap imgError(":resources/images/error.png");
    if (!m_success) {
        int sx = xsize/10;
        p.setFont(QFont("Courier", m_fontSize*2, QFont::Bold));
         p.drawPixmap(10,10,sx,sx, imgError);  // this works
         p.drawText(QRect(sx+10,sx/2,xsize,m_fontSize*3), Qt::AlignLeft | Qt::AlignVCenter, "Compile failed");
    }


    QPixmap pm;
    pm.convertFromImage(img);
    ui->lblImage->setPixmap(pm.scaled(xsize,ysize, Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    ui->lblImage->setPixmap(pm);

    repaint();

}

void DialogCrash::InitColors()
{
    m_colors.clear();
    m_colors["code"]=QColor(255,127,127);
    m_colors["music"]=QColor(255,255,127);
    m_colors["data"]=QColor(127,127,255);
    m_colors["user"]=QColor(127,255,255);
    m_colors["array"]=QColor(255,127,255);
}

/*
void DialogCrash::resizeEvent(QResizeEvent *e)
{
    ReInitialise();
    m_ini->setFloat("size_analyser_window_width", this->size().width());
    m_ini->setFloat("size_analyser_window_height", this->size().height());
}
*/

void DialogCrash::mouseMoveEvent(QMouseEvent *event)
{
    zoomCenter = this->mapFromGlobal(QCursor::pos());
    ReInitialise();

}

void DialogCrash::wheelEvent(QWheelEvent *event)
{
    zoomVal *=1+0.001f*event->angleDelta().y();
    zoomVal = std::max(1.0f, zoomVal);
    ReInitialise();
}

QPointF DialogCrash::Trans(QPointF p)
{
    return (p-zoomCenter)*zoomVal + zoomCenter;
}

QRect DialogCrash::Trans(QRect r,float addy)
{
    QPointF p = Trans(QPointF(r.x(),r.y()));
    return QRect(p.x(),p.y(),r.width()*zoomVal,(r.height())*zoomVal);
}

QRect DialogCrash::Trans(float x, float y, float w, float h)
{
    QPointF p = Trans(QPointF(x,y));
    return QRect(p.x(),p.y(),w*zoomVal,h*zoomVal);

}

void DialogCrash::on_pushButton_clicked()
{
 close();
}

