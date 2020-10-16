#include "pixelchar.h"
#include "limageqimage.h"

int PixelChar::m_lastBitmask = 1;


PixelChar::PixelChar()
{
    Clear(0);
}

unsigned char PixelChar::get(int x, int y, unsigned char bitMask)
{
    if (x<0 || x>=8 || y<0 || y>=8)
        return 0;

    unsigned char pp = (p[y]>>x) & bitMask;

    return c[pp];

}

void PixelChar::set(int x, int y, unsigned char color, unsigned char bitMask, unsigned char maxCol, unsigned char minCol)
{
    m_lastBitmask = bitMask;
    if (x<0 || x>=8 || y<0 || y>=8) {
        qDebug() << "Trying to set " << x << ", " << y << " out of bounds";
        return;
    }


     unsigned char winner = 254;
    // Does color exist in map?
    for (int i=0;i<maxCol;i++) {
        if (c[i] == color) {
            winner = i;
            break;
        }
    }

    if (winner==254) {// && color!=c[0]) {

        for (int j=minCol;j<maxCol;j++)
            if (c[j]==255) {
                winner = j;
                break;
            }

        // not available slots found
        if (winner==254)
        {
            //winner = 3;
            winner = (p[y]>>x) & bitMask;
            if (winner==0 && minCol!=0)
                winner = maxCol-1;

        }

        if (winner>=minCol)
            c[winner] = color;

    }


    unsigned int f = ~(bitMask << x);
    p[y] &= f;
    // Add

    p[y] |= winner<<x;

}

void PixelChar::set(int x, int y, unsigned char color, unsigned char bitMask)
{
    if (x<0 || x>=8 || y<0 || y>=8) {
        qDebug() << "Trying to set " << x << ", " << y << " out of bounds";
        return;
    }

    // find index
    uchar index = 10;
    if (c[0]==color) index=0;
    else
    if (c[1]==color) index=1;
    else
    if (c[2]==color) index=2;
    else
    if (index==10) {
        index=3;
        c[index] = color;

    }

    // Clear
    unsigned int f = ~(bitMask << x);
    p[y] &= f;
    // Add
    p[y] |= (index&bitMask)<<x;

}

void PixelChar::Clear(unsigned char bg)
{
    for (int i=0;i<8;i++)
        p[i] = 0;
    for (int i=1;i<4;i++)
        c[i] = 255;
    c[0] = bg;

}

QString PixelChar::bitmapToAssembler()
{
    QString s = "   byte ";
    for (int i=0;i<8;i++) {
        s = s + QString::number(reverse(p[i]));
        if (i!=7)
            s = s+", ";
    }
    s=s+"\n";
    return s;
}

QString PixelChar::colorMapToAssembler(int i, int j)
{
    if (c[i]==255) c[i] = 0;
    if (c[j]==255) c[j] = 0;
    return QString(QString::number(c[i] | c[j]<<4));
}

uchar PixelChar::colorMapToNumber(int i, int j)
{
    if (c[i]==255) c[i] = 0;
    if (c[j]==255) c[j] = 0;
    return (c[i] | c[j]<<4);
}

QByteArray PixelChar::data()
{
    QByteArray qb;
    for (int i=0;i<8;i++)
        qb.append(reverse(p[i]));

    return qb;
}

uchar PixelChar::flipSpriteBit(int cnt, int m)
{
    uchar k = p[cnt];
    if (m==0b1)
        return k;

    for (int i=0;i<8;i+=2) {
        uchar mask = 0b11 <<i;
        uchar j = (k >> i)&0b11;
        int n=j;


        if (j==1) n=3;
        if (j==3) n=1;

        k = (k & ~mask) | (n<<i);

    }
    return k;
//    p[cnt] = k;


}



QString PixelChar::colorToAssembler()
{
    if (c[3]==255) c[3] = 0;
    return QString(QString::number(c[3]));

}

QImage PixelChar::toQImage(int size, uchar bmask, LColorList& lst, int scale)
{
    QImage img= QImage(size,size,QImage::Format_RGB32);
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++) {
            int x = i/(float)(size)*8;
            int ix = (x % (8)/scale)*scale;
            int y = j/(float)(size)*8;
            uchar c = get(ix,y, bmask);

           // if (rand()%100==0 && c!=0)
           //     qDebug() << lst.m_list[c].color;
            img.setPixelColor(i,j,lst.get(c).color);
        }

    return img;
}

bool PixelChar::isEmpty()
{
    for (int i=0;i<8;i++)
        if (p[i]!=0)
            return false;

    return true;
}

bool PixelChar::isEqualBytes(PixelChar &o)
{
    for (int i=0;i<8;i++)
        if (p[i]!=o.p[i])
            return false;

    return true;
}

void PixelChar::Reorganize(unsigned char bitMask, unsigned char scale, unsigned char minCol, unsigned char maxCol, unsigned char bgCol)
{
//    if (rand()%1000==0)
  //      qDebug() << bgCol << minCol << maxCol;
    if (c[0]!=bgCol)
        for (int i=1;i<maxCol;i++)
            if (c[i]==bgCol) {
                unsigned char tmp = c[0];
                c[0]=c[i];
                c[i]=tmp;

            }
    if (minCol<1) minCol=1;
    for (int i=minCol;i<maxCol;i++) {
        unsigned int cnt = Count(i, bitMask, scale);
//        qDebug() << cnt;
        if (cnt == 0) {
            c[i] = 255;
           // qDebug() << "REMOVING COLOR";
        }
    }
}

int PixelChar::Count(unsigned int col, unsigned char bitMask, unsigned char scale)
{
    int cnt=0;
    for (int i=0;i<8/scale;i++)
        for (int j=0;j<8;j++)
            if ( ((p[j]>>scale*i) & bitMask)==col)
                cnt++;
    return cnt;
}

double PixelCharSSIM::getVal(int x, int y) {
    if (x<0 || x>=8 || y<0 || y>=8)
        return 0;
    int s = 1;
    if (m_lastBitmask==0b11)
        s = 2;

    double d = (p[y]>>(x*s)) & m_lastBitmask;

//    if (rand()%100>98)
  //      qDebug() << d << m_lastBitmask << x << y << s;

    if (d!=0) d+=1.5; // Always prioritze background color

    return d;
}


void PixelChar::ForceBackgroundColor(int col, int swapcol)
{
    //    if (c[0] == col)
  //      return;

    int idx = -1;
    for (int i=0;i<4;i++) {
        if (c[i]==swapcol) {
            idx=i;
            break;
        }
    }
//    if (idx==-1)
  //      idx=1;
//    idx=1;
    if (idx!=-1) {
//        qDebug() << "swapping "  << rand()%100;
        unsigned char s = c[0];
//        c[0]=c[idx];
  //      c[idx] = s;
//        qDebug() << idx;
        for (int j=0;j<8;j++) {

            p[j] = SwapColor(p[j],0,idx);
/*            int ss = idx*2;
            s = p[j]&0b11;
            unsigned char s2 = (p[j]>>(ss))&0b11;
            p[j] = p[j] & 0b11111100;
            p[j] = p[j] & ~(0b11<<(ss));
            p[j] = p[j] | (s <<(ss)) | s2;*/
        }
    }
//    else c[0]=col;



}

int PixelChar::Compare(PixelChar &other) {
    int l = 0;
    for (int i=0;i<8;i++)
        l+=other.p[i] != p[i];

    /*        for (int i=0;i<4;i++)
            l+=other.c[i]!=c[i];
*/
    return l;

}

int PixelChar::CompareLength(PixelChar &other) {
    int l = 0;
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++) {
            char a = (p[i]>>(j))&0b1;
            char b = (other.p[i]>>(j))&0b1;
            if ( a != b )
                l++;

        }
    return l;

}

int PixelChar::CompareLength2(PixelChar &other) {
    int l = 0;
    for (int i=0;i<8;i++)
        for (int j=0;j<8;j++) {
            char a = (p[i]>>(j))&0b1;
            char b = (other.p[i]>>(j))&0b1;
            if ( a != b )
                l++;
            if (a==0 && b!=0) l++;
            if (b==0 && a!=0) l++;

        }
    //            if (other.p[i]!=p[i])
    //          l+=other.p[i] != p[i];

    return l;

}

int PixelChar::CompareLength3(PixelChar &other)
{
    // C = sum(Pij*Qij)^2/(sum(Pij^2)*sum(Qij^2)).
    int ap = 0;
    int bp = 0;
    int s = 0;
    for (int i=0;i<8;i++) {
        for (int j=0;j<8;j++) {
            char a = (p[i]>>(j))&0b1;
            char b = (other.p[i]>>(j))&0b1;
//            s+=pow(a-b,2);
            s+=a!=b;//abs(a-b);
        }
    }



    return s;
}

double PixelChar::CompareLength4(PixelChar &other,LColorList& lst, int bmask)
{
    other.m_lastBitmask = bmask;
    m_lastBitmask = bmask;
    PixelCharSSIM p1,p2;
    for (int i=0;i<8;i++) {
        p1.p[i] = p[i];
        p2.p[i] = other.p[i];
    }
    p1.m_lastBitmask = 1;//bmask;
    p2.m_lastBitmask = 1;//bmask;
    return p1.CalcSSIM(&p2);
}

uchar PixelChar::SwapColor(uchar data, uchar c1, uchar c2)
{
    for (int i=0;i<4;i++) {
        int j= i*2;
        uchar k1 = (data>>j)&0b11;
        uchar k2 = k1;
        if (k1==c1) { k2=c2;}
        //        else

        if (k1==c2) { k2=c1;}
        uchar mask = ~(0b11<<j);
        data = data & mask;
        data = data | (k2<<j);

    }
    return data;
}


uchar PixelChar::Swap(int a, int b, uchar c)
{
        // damn
    uchar n1 = (c>>a) & 0b00000011;
    uchar n2 = (c>>b) & 0b00000011;

    uchar mask =  ((0b11)<<a) | ((0b11) <<b);

    c = c & ~mask;
    c = c | (n1<<b) | (n2<<a);


    return c;
}

uchar PixelChar::VIC20Swap(uchar c)
{
    uchar ret = 0;
    for (int i=0;i<4;i++) {
        uchar v = c>>(i*2) & 0b11;
/*        if (v==0b10) v=0b11;
        else
            if (v==0b11) v=0b10;
*/
        if (v==0b01) v=0b11;
        else
            if (v==0b11) v=0b01;


        ret |= (v<<(i*2));

    }
    return ret;
}


void PixelChar::Invert() {
    for (int i=0;i<8;i++) {
        p[i] = ~p[i];
    }
}
