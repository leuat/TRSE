#include "limagecga.h"

LImageCGA::LImageCGA(LColorList::Type t)  : LImageQImage(t)
{
    Initialize(320,200);
    m_scale = 1;
    m_type = LImage::Type::CGA;
    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = true;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

}

void LImageCGA::ExportBin(QFile &file)
{
    QByteArray even, odd,m1,m2;
    toCGA(even,odd,m1,m2,0,0,320,200);
    for (int i=0;i<192;i++) even.append((char)0);
    file.write(even);
    file.write(odd);
}

QStringList LImageCGA::SpriteCompiler(QString name, QString source, QString dst, int xp, int yp, int w, int h) {
    QStringList src;

    src<<";Sprite Compiler appendix CGA for : "+name;
    w+=1;
    for (int i=0;i<2;i++)
    {
        src<<"drawsprite_cga_"+name+"_sprite" + QString::number(i) + ":";

//        src<<"mov ds,ax";
  //      src<<"mov di,bx";
        // Build bytes
        QByteArray even,odd, evenMask,oddMask;
        toCGA(even,odd,evenMask, oddMask, xp*4+i*2,yp*8,w*8,h*8);
        QByteArray* data = &even;
        QByteArray* mask = &evenMask;

        for (int k=0;k<2;k++) {
            int cur = 0;
            src << " mov bx, di";
            src << " mov dx, si";
            int siAdd= 0;
            for (int y=0;y<h*4;y++) {
                for (int x=0;x<w*2;x++) {
//                    src << " mov al,"+Util::numToHex(~((uchar)(*data)[cur++])).replace("$","0x");
  //                  src << " and al, [ds:si+"+QString::number(siAdd++)+"]";

                    if (((*mask)[cur])==(char)0x0) {
                        src<<"mov al, [ds:si+"+QString::number(siAdd++)+"]";
                        src << "stosb";
                    }
                    else {
                    src << " mov al,"+Util::numToHex(((uchar)(*mask)[cur])^0xFF).replace("$","0x");
                    src << " and al, [ds:si+"+QString::number(siAdd++)+"]";
                    src << " or al,"+Util::numToHex(((uchar)(*data)[cur])).replace("$","0x");
                    src << " stosb";
                    }
                    cur++;

//                    src << " inc si";
                }
                src << " add di, " + Util::numToHex(80-w*2).replace("$","0x");
//                src << " add si, " + Util::numToHex(80-w*2).replace("$","0x");
                siAdd+=80-w*2;
            }
            data = &odd;
            mask = &oddMask;
            cur = 0;
            src << "mov di,bx";
            src << " add di, "+QString::number(0x2000);
            src << "mov si,dx";
            src << " add si, "+QString::number(0x2000);
        }
        src << "ret";
    }
    src << "drawsprite_cga_"+name+":";
    src<< "mov ax, [ds_xx]";
    src<< "shr ax,1";
    src<< "and ax,1";
    src<< "mov [ds_ddx],ax";

    src<< "mov ax, [ds_xx]";
    src<< "shr ax,1";
    src<< "shr ax,1";
    src<< "mov [ds_xx],ax";

    src<< "mov ax,[ds_yy]";
    src<< "mov bh, 80";
    src<< "mul bh";
    src<< "mov [ds_yy],ax";




//    src<< "mov ax, 0xB800";
//    src<< "mov es, ax";
//    src<< "xor di,di";

    src<< "add di, [ds_xx]";
    src<< "add di,[ds_yy]";

//    src<< "lea si, [data]";

//    src<< "xor si,si";

    src<< "add si, [ds_xx]";
    src<< "add si,[ds_yy]";

    src<< "mov ax,1";
    src<< "cmp [ds_ddx],ax";
    src<< "jne "+name+"_cont";

    src<< "call drawsprite_cga_"+name+"_sprite0";
    src<< "jmp "+name+"_endd";
src<< name+"_cont:	";
    src<< "call drawsprite_cga_"+name+"_sprite1";
src<< name+"_endd:";
    src<<"ret";



    return src;
}

void LImageCGA::toCGA(QByteArray& even, QByteArray& odd, QByteArray& evenMask, QByteArray& oddMask,int x1, int y1, int w, int h)
{
    int cur=0;
    char curC=0;
    char curM=0;
    for (int y=0;y<h;y++) {
        for (int x=0;x<w;x++) {
           // QColor c = QColor(m_qImage->pixel(x,y));
//            qDebug() << m_qImage->pixel(x,y);
            char idx = m_qImage->pixel(x+x1,y+y1);//m_colorList.getIndex(c);
  //          qDebug() << QString::number(cur) << c;

            char midx = 0;
            if (idx!=0) midx=0b11;
            curM = curM | (midx<<(6-(curC*2)));


            cur = cur | (idx<<(6-(curC*2)));
            curC++;
            if (curC==4) {
                if ((y&1)==0)
                    even.append(cur);
                else odd.append(cur);
                if ((y&1)==0)
                    evenMask.append(curM);
                else oddMask.append(curM);
                curC=0;
                cur=0;
                curM = 0;
            }

        }
    }

}

