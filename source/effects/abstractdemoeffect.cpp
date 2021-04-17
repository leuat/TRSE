#include "abstractdemoeffect.h"

AbstractDemoEffect::AbstractDemoEffect(QGridLayout* gl)
{
    m_img = QImage(320,200,QImage::Format_ARGB32);
    m_post = QImage(320,200,QImage::Format_ARGB32);
    m_prev = QImage(320,200,QImage::Format_ARGB32);
    m_gl = gl;
}

char AbstractDemoEffect::FadeColor(char c, int speed)
{
//    int fromBlack[] = {0x00,0x06,0x09,0x0b,0x02,0x04,0x08,0x0c,0x0e,0x0a,0x05,0x0f,0x03,0x07,0x0d,0x01,0x0d,0x07,0x03,0x0f,0x05,0x0a,0x0e,0x0c,0x08,0x04,0x02,0x0b,0x09,0x06,0x00,0x00};
    int fromBlack[] = {0x00,0x06,0x09,0x0b,0x02,0x04,0x08,0x0c,0x0e,0x0a,0x05,0x0f,0x03,0x07,0x0d,0x01,0x0d,0x07,0x03,0x0f,0x05,0x0a,0x0e,0x0c,0x08,0x04,0x02,0x0b,0x09,0x06,0x00,0x00};
    uchar c1 = c&0b00001111;
    uchar c2 = (c&0b11110000)>>4;
    int i1=0, i2=0;
    if (speed==0) speed=1;
    for (int i=0;i<16;i++) {
        if (fromBlack[i]==c1) i1 = i;
        if (fromBlack[i]==c2) i2 = i;
    }
    i1-=speed;
    i2-=speed;
    if (i1<0) i1=0;
    if (i2<0) i2=0;
    return fromBlack[i1] | fromBlack[i2]<<4;

}

char AbstractDemoEffect::FadeColorTo(char c, char tar, int speed, int start)
{
    if (speed==0) speed=1;
        tar = tar &0xff;
        int fromBlack[] = {0x00,0x06,0x09,0x0b,0x02,0x04,0x08,0x0c,0x0e,0x0a,0x05,0x0f,0x03,0x07,0x0d,0x01,0x0d,0x07,0x03,0x0f,0x05,0x0a,0x0e,0x0c,0x08,0x04,0x02,0x0b,0x09,0x06,0x00,0x00};
        uchar c1 = c&0b00001111;
        uchar c2 = (c&0b11110000)>>4;
        uchar tc1 = tar&0b00001111;
        uchar tc2 = (tar&0b11110000)>>4;
        int i1=0, i2=0, t1=0, t2=0;
        for (int i=start;i<32;i++) {
            if (fromBlack[i]==c1) i1 = i;
            if (fromBlack[i]==c2) i2 = i;
            if (fromBlack[i]==tc1) t1 = i;
            if (fromBlack[i]==tc2) t2 = i;
        }
        i1+=speed;
        i2+=speed;
        if (i1>t1) i1=t1;
        if (i2>t2) i2=t2;
//        if (i1>16) i1=16;
 //       if (i2>16) i2=16;
//        qDebug() << i1 << t1 << " AND " << i2 << t2 <<Util::numToHex((uchar)tar);
//        if (rand()%100>98)
  //          qDebug() << speed;
        return fromBlack[i1] | fromBlack[i2]<<4;

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

void AbstractDemoEffect::UpdateScreenDataFrame(QVector<int> &screen, int xp, int yp, int w, int h, int frame)
{
    if (!m_backgroundSet) {
        m_backgroundUsed.resize((w-xp)*(h-yp));
        m_backgroundUsed.fill(0);
        m_backgroundSet = true;
    }
    if (screen.count()==0) {
        screen.resize((w-xp)*(h-yp));
        screen.fill(0);
    }
    for (int y=yp;y<h;y++) {
        for (int x=xp;x<w;x++) {
            QColor c0 = m_img.pixelColor(x,y);
            QRgb c1 = m_prev.pixel(x,y);
            int pos = y * w +x;
            if (c0 != Qt::black) {
                if (m_backgroundUsed[pos]==(char)0) {
                    screen[pos] = frame;
                    //                    if (m_backgroundUsed[pos]!=(char)0)  {
                    m_backgroundUsed[pos] = 1;
                }


            }

        }

    }

}



void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502ColorOut(QString file, QString background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h, int speed, int endCol) {
//    m_prev.save("test.png");
    if (m_prev.width() != m_img.width())
        return;

    if (!m_backgroundSet) {
        m_background = Util::loadBinaryFile(background);
        m_backgroundUsed.resize(m_background.size());
        m_backgroundUsed.fill(0);
        m_backgroundSet = true;

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
            QColor c0 = m_img.pixelColor(x,y);
            QRgb c1 = m_prev.pixel(x,y);
            int pos = y * w +x + 2;
            if (c0 != Qt::black) {
                if (m_backgroundUsed[pos]==(char)0) {
                    //                    if (m_backgroundUsed[pos]!=(char)0)  {
                    uchar c = FadeColorTo(m_background[pos],endCol,speed);
  //                  c= 0;
                    m_background[pos]=c;
                    if (c==endCol) // Done!
                        m_backgroundUsed[pos] = 1;
                    //                        else
                    lst[c].append(pos + base-2);
                }

                pos+=1000;

                if (m_backgroundUsed[pos]==(char)0) {
//                    if (m_backgroundUsed[pos]!=(char)0)  {
                        uchar c = FadeColorTo(m_background[pos],endCol,speed);
//                        c= 0;
                        m_background[pos]=c;
                        if (c==endCol) // Done!
                            m_backgroundUsed[pos] = 1;
//                        else
                        lst[c].append(pos + 0xD800-1000-2);
                    }

            }

        }
        //          if ()

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

void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502ColorIn(QString file, QString background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h, int speed, int startCol) {
//    m_prev.save("test.png");
    if (m_prev.width() != m_img.width())
        return;

    if (!m_backgroundSet) {
        m_backgroundTarget = Util::loadBinaryFile(background);
        m_background.resize(m_backgroundTarget.size());;
        m_background.fill(startCol);
//        m_background.fill(st);
        m_backgroundUsed.resize(m_background.size());
        m_backgroundUsed.fill(0);
        m_backgroundSet = true;

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
            QColor c0 = m_img.pixelColor(x,y);
            QRgb c1 = m_prev.pixel(x,y);
            int pos = y * w +x +2;
            if (c0 != Qt::black) {

                if (m_backgroundUsed[pos]==(char)0) {
                    //                    if (m_backgroundUsed[pos]!=(char)0)  {
                    char c = FadeColorTo(m_background[pos],m_backgroundTarget[pos],speed,startCol);
//                    c = m_backgroundTarget[pos];
  //                  c= 0;
                    m_background[pos] = c;
                    if (c==m_backgroundTarget[pos]) {// Done!
                        m_backgroundUsed[pos] = 1;
//                        qDebug() << "DONE "<< Util::numToHex((char)c) << Util::numToHex(pos);
                    }
                    //                        else
                    if (pos-2>=0 && pos-2<=1000)
                    lst[c].append(pos + base-2);
                }

                pos+=1000;

                if (m_backgroundUsed[pos]==(char)0) {
//                    if (m_backgroundUsed[pos]!=(char)0)  {
                        uchar c = FadeColorTo(m_background[pos],m_backgroundTarget[pos], speed);
//                        c= 0;
                        c =c&0xF;
  //                      c = m_backgroundTarget[pos];
                        m_background[pos]=c;
                        if (c==m_backgroundTarget[pos]) // Done!
                            m_backgroundUsed[pos] = 1;
//                        else
                        int p = pos + 0xD800-1000-2;
                        if (p>=0xD800 && p<0xD800+1000)
                            lst[c].append(p);
                    }


            }

        }
        //          if ()

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


void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502ColorInAddress(QString file, int background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h, int speed) {
//    m_prev.save("test.png");
    if (m_prev.width() != m_img.width())
        return;

    if (!m_backgroundSet) {
//        m_backgroundTarget = Util::loadBinaryFile(background);

        m_background.resize(2000);;
        m_background.fill(0);
        m_backgroundUsed.resize(m_background.size());
        m_backgroundUsed.fill(0);
        m_backgroundSet = true;

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
            QColor c0 = m_img.pixelColor(x,y);
            QRgb c1 = m_prev.pixel(x,y);
            int pos = y * w +x;
            if (c0 != Qt::black) {

                if (m_backgroundUsed[pos]==(char)0) {

                    int c = background + pos;
                    //                    if (m_backgroundUsed[pos]!=(char)0)  {
                    //char c = FadeColorTo(m_background[pos],m_backgroundTarget[pos],speed);

//                    c = m_backgroundTarget[pos];
  //                  c= 0;
                    m_background[pos] = c;
                    m_backgroundUsed[pos] = 1;
//                        qDebug() << "DONE "<< Util::numToHex((char)c) << Util::numToHex(pos);

                    //                        else
                    lst[c].append(pos + base);
                }

                pos+=1000;

                if (m_backgroundUsed[pos]==(char)0) {
//                    if (m_backgroundUsed[pos]!=(char)0)  {
                    int c = background + pos + 1000;
                        m_background[pos]=c;
                            m_backgroundUsed[pos] = 1;
                        lst[c].append(c);
                    }


            }

        }
        //          if ()

    }
    // Sort
    for (uchar c: lst.keys()) {
        uchar inv = c;
        out += " lda "+ Util::numToHex(inv) + "\n";
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



void AbstractDemoEffect::ExportDiffAsUnrolledBitmap6502In(QString file, QString background, QString name, QString waitFunc, int base, int xp, int yp, int w, int h) {
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
                        uchar inv = (char)((c)&0xFF);
                        if (bg.count()==0)
                            c = inv;
                        else {
                            c = (bg[pos-base]) & inv;
                        }
                        lst[c].append(pos);
           //             lst2.append(pos);
                        if (inv==0xFF)
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

void AbstractDemoEffect::ConvertToC64(int dither, bool isMulticolor, QVector3D ditherStrength)
{
    if (!(m_outputType==1  || m_outputType==5))
        return;
    m_mc->setMultiColor(isMulticolor);
    m_mc->m_colorList.DefaultPen(LPen::Type::FixedSingle,m_cols.count());
    for (int i=0;i<m_cols.count();i++)
        m_mc->m_colorList.setPen(i,m_cols[i]);


    //            m_mc->m_colorList.DefaultPen(LPen::Type::FixedSingle);


    m_mc->setBackground(m_cols[0]);
    m_mc->m_footer.set(LImageFooter::POS_DISPLAY_MULTICOLOR,isMulticolor==1);
    m_mc->m_footer.set(LImageFooter::POS_DISPLAY_HYBRID,0);
    int size = 2;
    //     m_mc->m_forcePaintColorAndChar = false;
    m_mc->m_colorList.EnableColors(m_cols);
    if (dither==1)
        m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true);
    if (dither==2)
        m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength,QPoint(1,size), 1);
    if (dither==0)
        m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false);

    m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

}

void AbstractDemoEffect::ConvertToCharset(int dither, bool isMulticolor, QVector3D ditherStrength)
{
    if (!(m_outputType==7 ))
        return;
    isMulticolor = false;
    m_mc->setMultiColor(isMulticolor);
    m_mc->m_colorList.DefaultPen(LPen::Type::FixedSingle,m_cols.count());
    for (int i=0;i<m_cols.count();i++)
        m_mc->m_colorList.setPen(i,m_cols[i]);



    //            m_mc->m_colorList.DefaultPen(LPen::Type::FixedSingle);


    m_mc->setBackground(m_cols[0]);
    m_mc->m_footer.set(LImageFooter::POS_DISPLAY_MULTICOLOR,isMulticolor==1);
    m_mc->m_footer.set(LImageFooter::POS_DISPLAY_HYBRID,0);
    int size = 2;
    //     m_mc->m_forcePaintColorAndChar = false;
    m_mc->m_colorList.EnableColors(m_cols);
    if (dither==1)
        m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, true);
    if (dither==2)
        m_mc->OrdererdDither(m_img, m_mc->m_colorList, ditherStrength,QPoint(1,size), 1);
    if (dither==0)
        m_mc->FloydSteinbergDither(m_img, m_mc->m_colorList, false);

    m_mc->ToQImage(m_mc->m_colorList,m_img,1,QPointF(160,100));

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

