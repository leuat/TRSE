#include "limagelevelnes.h"

LImageLevelNES::LImageLevelNES(LColorList::Type t) : ImageLevelEditor(t)
{
    m_type = LImage::Type::LevelEditorNES;
    m_supports.displayBank = true;

    m_charWidthDisplay = 32;
    m_supports.asmExport = true;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.nesPalette = false;
    m_supports.koalaImport = false;
    m_supports.koalaExport = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.compressedExport = false;
    m_supports.displayForeground = true;


    m_GUIParams[btnLoadCharset] ="Load charset";
    m_GUIParams[btn1x1] = "";
    m_GUIParams[btn2x2] = "";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "";
    m_GUIParams[btnPaste] = "";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";

    m_GUIParams[tabCharset] = "Charset";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";
    m_GUIParams[tabEffects] = "Effects";

    m_GUIParams[btnEditFullCharset] = "";

    m_writeType = Character;

//    m_GUIParams[tabSprites] ="Metachunks";
    m_currencChar = 1;


    m_GUIParams[btnCopy] = "Copy";
    m_GUIParams[btnPaste] = "Paste";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";
    m_GUIParams[btnEditFullCharset] = "";

    m_GUIParams[tabCharset] = "1";
    m_GUIParams[tabData] = "1";
    m_GUIParams[tabLevels] = "1";


}

void LImageLevelNES::SetColor(uchar col, uchar idx)
{
/*    if (m_charset==nullptr)
        return;
    m_charset->SetColor(col, idx);
    */
}

unsigned int LImageLevelNES::getPixel(int x, int y)
{
    if (m_charset==nullptr)
        return 0;
    int pos;
    if (!PixelToPos(x,y, pos))
        return 0; // out of bounds



    uchar v = m_currentLevel->m_CharData[pos];
    uchar col=0;
    if (m_meta.m_useColors)
        col = m_currentLevel->m_ColorData[pos];

//    int ix = (x % (8)/2)*2;//- (dx*40);
  //  int iy = y % 8;//- (dy*25);

//    v = 0;
    return m_charset->getCharPixel(v, col, x,y);


}

void LImageLevelNES::ExportBin(QFile &file)
{


    file.write(m_meta.toHeader());
    int i=0;
    if (m_levels.count()==0)
        return;
    CharmapLevel* ll = m_levels[0];

    for (CharmapLevel* l : m_levels) {
        file.write( l->m_CharData);

        QByteArray d;
        d.resize(l->m_ColorData.count()/4);
        int j=0;
        for (int y=0;y<m_meta.m_height/2;y++)
            for (int x=0;x<m_meta.m_width/2;x++) {
                int p = (y*2*m_meta.m_width) + x*2;

            d[j]  = (l->m_ColorData[p]&3) |
                    ((l->m_ColorData[p+1]&3)<<2) |
                    ((l->m_ColorData[p+m_meta.m_width]&3)<<4)|
                    ((l->m_ColorData[p+m_meta.m_width+1]&3)<<6);
            j++;
        }
        file.write(d);

        file.write( l->m_ExtraData);

    }

}

/*void LImageLevelNES::setPixel(int x, int y, unsigned int color)
{
    int pos;
    if (!PixelToPos(x,y, pos))
        return; // out of bounds

    //    qDebug() << (m_writeType==Color);

    if (m_writeType==Character)
        m_currentLevel->m_CharData[pos] = m_currencChar;
    if (m_writeType==Color)
        m_currentLevel->m_ColorData[pos] = color;

    //BuildImage();
}
*/
