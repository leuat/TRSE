#include "abstractdemoeffect.h"

AbstractDemoEffect::AbstractDemoEffect(QGridLayout* gl)
{
    m_img = QImage(320,200,QImage::Format_ARGB32);
    m_post = QImage(320,200,QImage::Format_ARGB32);
    m_prev = QImage(320,200,QImage::Format_ARGB32);
    m_gl = gl;
}

void AbstractDemoEffect::Save(QString file)
{

}

void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502(QString file, QString background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h) {
//    m_prev.save("test.png");
    if (m_prev.width() != m_img.width())
        return;

    QByteArray bg;
//    qDebug() << "bg background image " << background;
    if (background !="" && QFile::exists(background)) {
        bg = Util::loadBinaryFile(background);
    }

    QString out ="";
    bool isFirst = false;
    if (QFile::exists(file)) {
        out = Util::loadTextFile(file);
        out.remove(" \"); end;\n");
    }
    else {
        out = "procedure "+name+"();\n";
        out+="begin\n  asm(\"\n";
        isFirst = true;
        alreadySet.clear();
    }

    QMap<uchar,QVector<int>> lst;
    QVector<int> lst2;
    for (int y=yp;y<h;y++) {
        for (int x=xp;x<w;x++) {
            for (int j=0;j<8;j++) {
                // Build bytes!
                int pos = base + 320*y + x*8 + j;
                uchar c = 0;
                for (int i=0;i<4;i++) {
                    QColor c0 = m_img.pixelColor(x*4+i,y*8+j);
                    QRgb c1 = m_prev.pixel(x*4+i,y*8+j);
//                    if (!(c0.red()==c1.red() && c0.green()==c1.green() && c0.blue()==c1.blue())) {
//                    qDebug() << c0;
                    if (c0!=Qt::black)
                        c |=(0b11<<(6-((i)*2)));


                }
                if (c!=0 && !isFirst) {
/*                    if (prevc!=c) {
                        uchar inv = (char)((~c)&0xFF);
//                        out += " lda #"+ Util::numToHex(inv) + "\n";
  //                      prevc = c;
                    }*/
                    //uchar inv = (char)((~c)&0xFF);
                    if (!alreadySet.contains(pos)) {
                        uchar inv = (char)((~c)&0xFF);
                        if (bg.count()==0)
                            c = inv;
                        else {
                            c = (bg[pos-base]) & inv;
                        }
                        lst[c].append(pos);
           //             lst2.append(pos);
                        if (inv==0x0)
                            alreadySet.append(pos);
                    }
                    //                out += " sta "+Util::numToHex((unsigned short)(pos&0xFFFF)) + "\n";
                }

            }
        }
    }
    // Sort
    for (uchar c: lst.keys()) {
        uchar inv = c;
        out += " lda #"+ Util::numToHex(inv) + "\n";
        for (int pos : lst[c]) {
//            if (!alreadySet.contains(pos))
            out += " sta "+Util::numToHex((unsigned short)(pos&0xFFFF)) + "\n";
        }

    }

/*
    out+=" lda #$0\n";
    for (int pos: lst2) {
        out += " sta "+Util::numToHex((unsigned short)(pos&0xFFFF)) + "\n";

    }*/
    out+=" jsr "+waitFunc + " ; wait \n ";


    out+=" \"); end;\n";
    Util::SaveTextFile(file,out);


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

