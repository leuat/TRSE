#include "abstractdemoeffect.h"

AbstractDemoEffect::AbstractDemoEffect(QGridLayout* gl)
{
    m_img = QImage(320,200,QImage::Format_ARGB32);
    m_gl = gl;
}

void AbstractDemoEffect::Save(QString file)
{

}

void AbstractDemoEffect::SaveCharset(QString filename, int w, int h)
{
    QFile f(filename);
    f.open(QFile::WriteOnly);
    QByteArray data;
    for (int y=0;y<h;y++)
        for (int x=0;x<w;x++) {
            char c = QColor(m_img.pixel(x,y)).red()/16;
            data.append(c);
            //qDebug() << yy;
            //PixelChar& pc = m_mc->m_data[40*y+x];
            //for (int i=0;i<8;i++)
            //    data.append(PixelChar::reverse(pc.p[i]));
        }



    f.write(data);
    f.close();

}

void AbstractDemoEffect::AddScreen(QByteArray &data, int w, int h, char base, int div, char ca1, char ca2)
{

    for (int y=0;y<h;y+=1)
        for (int x=0;x<2*w;x+=2) {
            char c1,c2;
//            if (x<m_img.width() && y<m_img.height())
            c1 = QColor(m_img.pixel(x,y)).red()/div;
  //          if (x+1<m_img.width() && y<m_img.height())
            c2 = QColor(m_img.pixel(x+1,y)).red()/div;
            char add = 0;
            char c = max(c1,c2);
            //if (c>7) c=7;
            if (c1<1) add=ca1;
            if (c2<1) add=ca2;
//            add=0;
            data.append(c+base+add);
        }

}

void AbstractDemoEffect::AddToDataX(QByteArray &data, int xp, int yp, int w, int h)
{
    for (int y=0;y<h;y+=1)
        for (int x=0;x<w;x+=1) {
            int xx = xp+x;
            int yy = yp+y;

            PixelChar& pc = m_mc->m_data[40*(yy/8)+xx];
            data.append(PixelChar::reverse(pc.p[yy&7]));
        }

}

int AbstractDemoEffect::Compare(QByteArray &a, QByteArray &b, int p1, int p2, int length)
{
    int l = 0;
    for (int i=0;i<length;i++) {
        if (a[p1+i]!=b[p2+i]) l++;
    }
    return l;
}

void AbstractDemoEffect::OptimizeAndPackCharsetData(QByteArray &dataIn, QByteArray &out, QByteArray &table, int width, int compression)
{
    out.clear();
    table.clear();

    int cnt = dataIn.count()/width;
    qDebug() << "Width : " << width;
    qDebug() << "total rows: " << cnt;
    qDebug() << "total chars x frames: " << (cnt/width);
    qDebug() << "total chars: " << (cnt/32)/width;

    qDebug() << "cnt SHOULD be " << (32*width*26);


    for (int i=0;i<cnt;i++) {
        unsigned short currentPointer = 0;
        bool isNew = true;
        for (int j=0;j<out.count()/width;j++) {
            int metric = Compare(dataIn, out, i*width, j*width, width);
            if (metric<compression) {
                currentPointer =j*width;
                isNew=false;
                //break;
            }
        }
        if (isNew) {
            currentPointer = out.count();
            for (int j=0;j<width;j++) {
                out.append(dataIn[i*width+j]);
            }
        }
//        qDebug() << isNew << currentPointer;
        char lo = currentPointer&0xff;
        char hi = (currentPointer>>8)&0xff;
//        qDebug() << QString::number(lo) << QString::number(hi);
        table.append(lo);
        table.append(hi);

    }


}


void AbstractDemoEffect::ConvertToC64(bool dither)
{
    if (m_toggleC64) {
        m_mc->m_colorList.EnableColors(m_cols);
        m_mc->SetColor(m_cols[0],0);
        m_mc->SetColor(m_cols[1],1);
        m_mc->SetColor(m_cols[3],2);
        m_mc->SetColor(m_cols[2],3);
        m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, dither);

        m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));
        }

}

void AbstractDemoEffect::FillToGUI()
{
    while (m_gl->count()) {
        QLayoutItem * cur_item = m_gl->takeAt(0);
              if(cur_item->widget())
                  delete cur_item->widget();
              delete cur_item;
    }
    int i=0;
    for (QString s: m_params.keys()) {
        QLabel* l= new QLabel(s);
        m_gl->addWidget(l,i,0);
        QLineEdit* le= new QLineEdit(QString::number(m_params[s].m_val));
        connect(le, SIGNAL(textChanged(const QString &)), this, SLOT(FillFromGUI()));
        m_gl->addWidget(le,i,1);
        i++;
    }
//    exit(1);
}

void AbstractDemoEffect::FillFromGUI()
{
    for (int i=0;i<m_gl->rowCount();i++) {
        QLabel* lbl = dynamic_cast<QLabel*>(m_gl->itemAtPosition(i,0)->widget());
        QString s = lbl->text();
        bool ok=false;
        float val = ((QLineEdit*)m_gl->itemAtPosition(i,1)->widget())->text().toFloat(&ok);
        if (ok)
            m_params[s].m_val = val;

    }

}

