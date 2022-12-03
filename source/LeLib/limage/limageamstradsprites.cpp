#include "limageamstradsprites.h"
#include "source/Compiler/syntax.h"
#include "source/LeLib/limage/limageamstradcpc.h"

LImageAmstradSprites::LImageAmstradSprites(LColorList::Type t) : LImageGenericSprites(t) {
    m_type = LImage::Type::AmstradSprites;
    m_current=0;
    m_width = 320;
    m_height = 200;
    m_GUIParams[tabCharset] = "";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "";
    m_GUIParams[tabSprites] ="Sprites";

    //    m_supports.displayForeground = true;

    m_exportParams.clear();
    m_supports.displayCharOperations = false;
    m_footer.set(LImageFooter::POS_DISPLAY_MULTICOLOR,0);
    m_colorList.m_supportsFooterPen = true;
    usePens = true;
    m_blockSize = 4;
    m_items.clear();
    AddNew(1,1);
    //Data::data.currentColor=0;

}

void LImageAmstradSprites::ExportBin(QFile &ofile)
{

    QString f = ofile.fileName();

    QString filenameBase = Util::getFileWithoutEnding(f);

    QString fColor = filenameBase + "_palette.bin";

    if (QFile::exists(fColor))
        QFile::remove(fColor);

    QByteArray palette,data;

    QVector<int> lst = m_colorList.getPenList();
    for (auto i : lst)
        palette.append(((unsigned char)i));
    //    qDebug() << lst;

    Util::SaveByteArray(palette,fColor);

    for (int i=0;i<m_items.size();i++)
        data.append(ExportFrame(i));


    ofile.write(data);
}


QByteArray LImageAmstradSprites::ExportFrame(int frame)
{
    QByteArray d;
    int y = 0;
    int dy = 0;
    int xw;

    LGenericSprite* s = ((LGenericSprite*)m_items[frame].get());


    for (int i=0;i<s->m_height*m_blockSize;i++) {
        char c = 0;
        int curBit = 0;
        for (int x=0;x<s->m_width*m_blockSize;x++) {
            //int pixel = ((dy+y)/10)&15;
            int pixel = s->m_data.getPixel(x,i);
            //pixel = m_colorList.getPen(pixel);
//            qDebug() << pixel;
            //            qDebug() <<curBit << pixel << x;
            //pixel = 4;
            c|=(LImageAmstradCPC::table160[pixel]<<(1-curBit));


            curBit+=1;

            if (curBit>=2) {
                curBit=0;
                d.append((LImageAmstradCPC::AmstradCrazySwap(c)));
                //                data.append(c);
                c=0;
            }


        }
        y=y+8;

        if (y>=m_height) {
            y=0;
            dy++;
//            for (int i=0;i<s->m_width/2;i++) // Extra data to align to $0, $0800, $1000 etc
  //              d.append((char)0);
        }

    }

    return d;
}

void LImageAmstradSprites::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
    for (int i=0;i<m_items.count();i++) {

        LGenericSprite* s = ((LGenericSprite*)m_items[i].get());
        s->m_data.usePens = true;

    }


    LGenericSprite* s = ((LGenericSprite*)m_items[m_current].get());
    if (s->m_data.m_qImage==nullptr)
        return;
    img = QImage(s->m_data.m_qImage->width(),s->m_data.m_qImage->height(), QImage::Format_ARGB32);
    //    qDebug() << s->m_data.m_qImage->width()<<img.width();
    //    exit(1);

    for (int i=0;i<s->m_width*m_blockSize;i++)
        for (int j=0;j<s->m_height*m_blockSize;j++) {

            float xp = ((i-center.x())*zoom)+ center.x();
            float yp = ((j-center.y())*zoom) + center.y();

            unsigned int pen = s->m_data.getPixel(xp,yp);// % 16;
            unsigned int col = m_colorList.getPen(pen);

            img.setPixel(i,j,lst.get(col).color.rgb());
        }


//    s->m_data.ToQImage(lst,img,zoom,center);
}

