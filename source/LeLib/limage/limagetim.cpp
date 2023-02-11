#include "limagetim.h"


LImageTIM::LImageTIM(LColorList::Type t)  : LImageQImage(t)
{
    Initialize(512,256);
    m_scale = 1;
    m_type = LImage::TIM;

    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_currentChar = 0;

    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "Char";
    m_supports.displayCharOperations = true;


    m_GUIParams[col1] = "Background";
    m_GUIParams[col2] = "Color 1";
    m_GUIParams[col3] = "Color 2";
    m_GUIParams[col4] = "Color 3";

    m_GUIParams[tabCharset] = "1";
    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;

    m_charWidth=64;
    m_charHeight=32;
    m_updateCharsetPosition = true;

    m_supports.displayCharOperations = true;

}

void LImageTIM::ExportBin(QFile &file)
{
    QByteArray data;
    if (m_exportParams.contains("export1"))
        if (m_exportParams["export1"]==1) {
            data.append(m_width);
            data.append(m_height);
        }
    for (int x=0;x<m_width;x+=4) {
        for (int y=0;y<m_height;y++) {
            uchar c = 0;
            for (int i=0;i<4;i++) {
                uchar v = getPixel(x+i,y);
                c=c|(v<<2*i);//'(6-2*i));
            }
            data.append(c);
        }
    }
    file.write(data);




}

void LImageTIM::ExportSubregion(QString outfile, int x, int y, int w, int h, int type) {

    QByteArray data;
    //    qDebug() << x<<type;
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
    if (type==1) {
            data.append(w);
            data.append(h);
        }

    for (int i=0;i<w/4;i++)
        for (int j=0;j<h;j++)
        {
            uchar c = 0;
            for (int p=0;p<4;p++) {
                uchar v = getPixel(x+i*4+p,y+j);
                c=c|(v<<2*p);//'(6-2*i));
            }
            data.append(c);
//            qDebug() << c << x+i*4 << y+j;
        }
    QByteArray existing;
    if (QFile::exists(outfile))
        existing = Util::loadBinaryFile(outfile);

    existing.append(data);
    Util::SaveByteArray(existing,outfile);



}

/*QStringList LImageTIM::SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int xp, int yp, int w, int h)
{
    QString fname = currentDir + "/"+name+".inc";
    if (QFile::exists(fname)) {
        QFile::remove(fname);
    }
    m_footer.set(LImageFooter::POS_DISPLAY_CHAR,0);
    QFile file(fname);
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream f(&file);
    for (int i=0;i<4;i++) {

//    return QStringList();
    f<<"procedure "+name+"_"+QString::number(i)+"();\n";
    f<<"begin\n";
    f<<"asm (\"\n";
    f<<"  ld de, "+Util::numToHex(32-w/4)+"\n";

    for (int y=0;y<h;y++) {
        for (int x=0;x<w;x+=4) {
            uchar c = 0;
            int xx = x + xp-i;
            int yy = y + yp;
            for (int j=0;j<4;j++) {
                uchar v = getPixel(xx+j,yy);
                c=c|(v<<(6-2*j));
            }
            f<< " ld a,"+ Util::numToHex(c) + "\n";
            f<< " ld [hl],a\n";
            f<< " inc hl \n";
            //data.append(c);
        }
        f<<"  add hl,de\n";
    }
    f<<"\");\n";
    f<<"end;\n";
    }



    f<<"var "+name+"_pi : integer;\n";
    f<<"@donotremove "+name+"_dst \n";
    f<<"procedure "+name+"("+name+"_src,"+name+"_dst : pointer; "+name+"px,"+name+"py : byte);\n";
    f<<"begin\n";
    f<<"  "+name+"_pi:="+name+"_dst + "+name+"py<<5 + "+name+"px>>2;\n";
    f<<  "asm(\"";
    f<<  "  ld hl, ["+name+"_src] \n";
    f<<  "  ex de, hl\n";
    f<<  "  ld hl, ["+name+"_pi]\n";
    f<<  "\");\n";
    f<< " "+name+"px:="+name+"px&3;\n";
    f<< " if ("+name+"px=0) then "+name+"_0();\n";
    f<< " if ("+name+"px=1) then "+name+"_1();\n";
    f<< " if ("+name+"px=2) then "+name+"_2();\n";
    f<< " if ("+name+"px=3) then "+name+"_3();\n";
    f<<"end;\n";


    file.close();





    return QStringList();
}
*/
