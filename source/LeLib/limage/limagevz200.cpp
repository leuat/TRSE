#include "limagevz200.h"


LImageVZ200::LImageVZ200(LColorList::Type t)  : LImageQImage(t)
{
    Initialize(128,64);
    m_scale = 1;
    m_type = LImage::VZ200;

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


    m_GUIParams[col1] = "Background";
    m_GUIParams[col2] = "Color 1";
    m_GUIParams[col3] = "Color 2";
    m_GUIParams[col4] = "Color 3";

    m_GUIParams[tabCharset] = "1";
    m_updateCharsetPosition = true;
    m_colorList.m_isCharset = true;

    m_charWidth=16;
    m_charHeight=16;

    m_supports.displayCharOperations = true;

}

void LImageVZ200::ExportBin(QFile &file)
{
    QByteArray data;

    for (int y=0;y<m_height;y++) {
        for (int x=0;x<m_width;x+=4) {
            uchar c = 0;
            for (int i=0;i<4;i++) {
                uchar v = getPixel(x+i,y);
                c=c|(v<<(6-2*i));
            }
            data.append(c);
        }
    }
    file.write(data);




}

QStringList LImageVZ200::SpriteCompiler(QString name, QString currentDir, QString src, QString dst, int xp, int yp, int w, int h)
{
    QString fname = currentDir + "/"+name+".inc";
    if (QFile::exists(fname)) {
        QFile::remove(fname);
    }

    QFile file(fname);
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream f(&file);

    f<<"procedure "+name+"();\n";
    f<<"begin\n";


    for (int y=0;y<h;y++) {
        for (int x=0;x<w;x+=4) {
            uchar c = 0;
            int xx = x + xp;
            int yy = y + yp;
            for (int i=0;i<4;i++) {
                uchar v = getPixel(xx+i,yy);
                c=c|(v<<(6-2*i));
            }
            //data.append(c);
        }
    }


    f<<"end\n";


    file.close();




    return QStringList();
}
