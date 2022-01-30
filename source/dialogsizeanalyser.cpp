#include "dialogsizeanalyser.h"
#include "ui_dialogsizeanalyser.h"
#include <QPainter>
#include <algorithm>
DialogSizeAnalyser::DialogSizeAnalyser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSizeAnalyser)
{
    ui->setupUi(this);
}

DialogSizeAnalyser::~DialogSizeAnalyser()
{
    delete ui;
}

void DialogSizeAnalyser::Initialize(SourceBuilder *sb, CIniFile *ini, int fontSize)
{
    m_sb = sb;
    m_ini = ini;
    m_fontSize = fontSize;
    setMouseTracking(true);
    ui->lblImage->setMouseTracking(true);
    fontSize/=2;

    Build();

    ReInitialise();
    repaint();

}

void DialogSizeAnalyser::Build()
{
    auto orgAsm = m_sb->m_system->m_orgAsm.get();
    maxVal = 0;
    int variableSize = 0;
    for (auto p : m_sb->compiler->m_parser.m_proceduresOnly) {
        QString proc = qSharedPointerDynamicCast<NodeProcedureDecl>(p)->m_procName;
        if (!orgAsm->m_symbols.contains("end_procedure_"+proc))
            continue;
        int start = orgAsm->m_symbols[proc];
        int end = orgAsm->m_symbols["end_procedure_"+proc];
        float size = end-start;
//        qDebug() << proc<<Util::numToHex(start)<<Util::numToHex(end);
        m_data.append(SizeData(proc, size, 0));
    }

    // arrays
    auto st = m_sb->compiler->m_parser.m_symTab;
    for (auto s: st->m_symbols) {
        if (s->m_type.toLower()=="byte")
            variableSize+=1;
        if (s->m_type.toLower()=="integer")
            variableSize+=2;
        if (s->m_type.toLower()=="long")
            variableSize+=4;
        if (s->m_type.toLower()=="array") {
            int len = s->getCountingLength()*s->getLength();
            m_data.append(SizeData(s->m_name,len,1));
        }
        if (s->m_type.toLower()=="incbin"){
        //    qDebug() << s->
            if (!orgAsm->m_symbols.contains("end_incbin_"+s->m_name))
                continue;

            int start = orgAsm->m_symbols[s->m_name];
            int end = orgAsm->m_symbols["end_incbin_"+s->m_name];
            float size = end-start;
    //        qDebug() << proc<<Util::numToHex(start)<<Util::numToHex(end);
            m_data.append(SizeData(s->m_name, (size), 2));

        }
    }
    if (orgAsm->m_symbols.contains("main_block_begin_")) {
        int start = orgAsm->m_symbols["main_block_begin_"];
        int end = orgAsm->m_symbols["main_block_end_"];
        m_data.append(SizeData("Main block",end-start,4));
    }

    m_data.append(SizeData("variables",variableSize,3));

    // Normalise
    for (auto& s:m_data) {
        if (maxVal<s.val)
            maxVal = s.val;
    }
    QVector<SizeData> nd;
    for (auto& s:m_data) {
        s.val/=(float)maxVal;
        if (s.val>0.01) // introduce cutoff
            nd.append(s);
    }
    m_data = nd;
    std::sort(m_data.begin(), m_data.end(),
              [](const auto& a, const auto& b) { return a.org > b.org; });

}

void DialogSizeAnalyser::ReInitialise(){
    xsize = ui->lblImage->width()-8;
    ysize= ui->lblImage->height()-8;
//    qDebug() << xsize << ysize;
/*    float xsize=this->width()*2;
    float ysize=this->height()*2;*/
    QImage img(QSize(xsize,ysize), QImage::Format_ARGB32);
    img.fill(QColor(20,60,30));
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
    int sx = xsize/10;


//    p.drawText(QRect(sx+10,sx/2,xsize,m_fontSize*3), Qt::AlignLeft | Qt::AlignVCenter, "Hurra!");

    int cnt = m_data.count();
    int border = 10;
    float dw = (xsize-border)/cnt;
    float dh = (ysize-border*2);
    int x = border;
    int lowerBorder = 60;
    int y = ysize - border-lowerBorder;

    int rw = 2;

    for (int i=0;i<cnt;i++) {
        int h = m_data[i].val*dh;

        auto c = QColor(200,90,40);
        if ((i&1)==0)
            c = QColor(160,30,60);
        // arrays
        if (m_data[i].type==1) {
            c = QColor(200,190,40);
            if ((i&1)==0)
                c = QColor(160,170,60);

        }
        // incbin
        if (m_data[i].type==2) {
            c = QColor(40,140,190);
            if ((i&1)==0)
                c = QColor(30,110,170);

        }
        // variables
        if (m_data[i].type==3)
            c = QColor(190,190,190);

        // main block
        if (m_data[i].type==4)
            c = QColor(20,190,30);

        QRect r = QRect(x,y-h,dw,h);
        QRect r2 = QRect(x,y-h-20,dw,h+30);
        if (r2.contains(mpos+QPoint(0,0))) {
            curT = m_data[i].name + " ("+Util::numToHex(m_data[i].org)+ " bytes)";
//            cur.setX(mb->m_start);
  //          cur.setY(mb->m_end);
            c = QColor(70,30,160);
        }
        p.setPen(c);
        p.setBrush(c);

        p.drawRect(r);
        x+=dw;
    }


    QColor c = QColor(0,0,0,200);
    p.setPen(QColor(100,255,255));
    p.setBrush(c);
    mpos.setX(mpos.x()+16);
    int width = 800;
    int dy = 10;
    // Render "tooltip" text
    if (curT!="") {
       QRect r = QRect(border,y+dy,xsize-2*border,40);
       p.drawRoundedRect(r,round,round);
       c = QColor(100,220,255,255);



       p.setPen(c);
//       p.setBrush(c);
       p.setFont(QFont("Courier", 15, QFont::Bold));
       p.drawText(QRect(border+10,y+4+dy,width,40), curT);
       }



    QPixmap pm;
    pm.convertFromImage(img);
    ui->lblImage->setPixmap(pm.scaled(xsize,ysize, Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    ui->lblImage->setPixmap(pm);

    repaint();
    update();
}

void DialogSizeAnalyser::InitColors()
{
    m_colors.clear();
    m_colors["code"]=QColor(255,127,127);
    m_colors["music"]=QColor(255,255,127);
    m_colors["data"]=QColor(127,127,255);
    m_colors["user"]=QColor(127,255,255);
    m_colors["array"]=QColor(255,127,255);
}


void DialogSizeAnalyser::resizeEvent(QResizeEvent *e)
{
    ReInitialise();
    if (m_ini==nullptr)
        return;
    m_ini->setFloat("size_analyser_window_width", this->size().width());
    m_ini->setFloat("size_analyser_window_height", this->size().height());
}


void DialogSizeAnalyser::mouseMoveEvent(QMouseEvent *event)
{
    zoomCenter = this->mapFromGlobal(QCursor::pos());
    ReInitialise();

}

void DialogSizeAnalyser::wheelEvent(QWheelEvent *event)
{
    zoomVal *=1+0.001f*event->angleDelta().y();
    zoomVal = std::max(1.0f, zoomVal);
    ReInitialise();
}

QPointF DialogSizeAnalyser::Trans(QPointF p)
{
    return (p-zoomCenter)*zoomVal + zoomCenter;
}

QRect DialogSizeAnalyser::Trans(QRect r,float addy)
{
    QPointF p = Trans(QPointF(r.x(),r.y()));
    return QRect(p.x(),p.y(),r.width()*zoomVal,(r.height())*zoomVal);
}

QRect DialogSizeAnalyser::Trans(float x, float y, float w, float h)
{
    QPointF p = Trans(QPointF(x,y));
    return QRect(p.x(),p.y(),w*zoomVal,h*zoomVal);

}

void DialogSizeAnalyser::on_pushButton_clicked()
{
 close();
}

