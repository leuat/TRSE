#include "limagelevelnes.h"

LImageLevelNES::LImageLevelNES(LColorList::Type t) : ImageLevelEditor(t)
{
    m_type = LImage::Type::LevelEditorNES;
    m_supports.displayBank = true;
    m_charWidthDisplay = 32;
    m_gridWidthDisplay = 32;
    m_supports.asmExport = true;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.nesPalette = false;
    m_supports.koalaImport = false;
    m_supports.koalaExport = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.compressedExport = false;
    m_supports.displayForeground = false;

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
    m_currentChar = 1;


    m_GUIParams[btnCopy] = "Copy";
    m_GUIParams[btnPaste] = "Paste";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";
    m_GUIParams[btnEditFullCharset] = "";

    m_GUIParams[tabCharset] = "1";
    m_GUIParams[tabData] = "1";
    m_GUIParams[tabLevels] = "1";

//    m_colorList.InitNES4();
//    m_colorList.InitNES();

    m_metaParams.append(new MetaParameter("color_width","Color width",20,2,1000));
    m_metaParams.append(new MetaParameter("color_height","Color height",10,2,1000));


    PerformConstrainColours(QVector<int>() <<0<<1<<2<<3);
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
    int pos = 0;
    int posC = 0;


    if (!PixelToPos(x,y, pos, m_meta.m_width, m_meta.m_height))
        return 0; // out of bounds
   // pos = 0;
    if (!PixelToPos(x,y, posC, m_meta.m_colSizex, m_meta.m_colSizey))
        return 0;


    uchar v = m_currentLevel->m_CharData[pos];
    uchar col = m_currentLevel->m_ColorData[posC];

    return m_charset->getCharPixel(v, col, x,y);


}


void LImageLevelNES::setPixel(int x, int y, unsigned int color)
{
    int pos, posC;
    if (!PixelToPos(x,y, pos,m_meta.m_width, m_meta.m_height))
        return; // out of bounds
    if (!PixelToPos(x,y, posC,m_meta.m_colSizex, m_meta.m_colSizey))
        return; // out of bounds

    //    qDebug() << (m_writeType==Color);

    if (m_writeType==Character)
        m_currentLevel->m_CharData[pos] = m_currentChar;
    if (m_writeType==Color)
        m_currentLevel->m_ColorData[posC] = color;

    //BuildImage();
}

QString LImageLevelNES::getMetaInfo()
{
    m_meta.m_colSizex = getMetaParameter("color_width")->value;
    m_meta.m_colSizey = getMetaParameter("color_height")->value;
    m_meta.m_startx = 0;
    m_meta.m_starty = 0;
    return ImageLevelEditor::getMetaInfo();
}

void LImageLevelNES::Initialize()
{

    ImageLevelEditor::Initialize();

    for (int i=0;i<m_meta.m_sizex;i++)
        for (int j=0;j<m_meta.m_sizey;j++) {
            int p = i + j*m_meta.m_sizex;
            m_levels[p]->m_ColorData.resize(m_meta.m_colSizex*m_meta.m_colSizey);
            m_levels[p]->m_CharData.fill(0);
            m_levels[p]->m_ColorData.fill(0);
        }
    SetBank(m_footer.get(LImageFooter::POS_CURRENT_BANK));

}

void LImageLevelNES::LoadBin(QFile &file)
{
    QByteArray h1 = file.read(32);
    m_meta.fromHeader(h1);
    m_meta.Calculate();
    m_meta.m_startx = 0;
    m_meta.m_starty = 0;
    Initialize();
    m_width = m_meta.m_width*16;
    m_height = m_meta.m_height*16;
    for (CharmapLevel* l : m_levels) {

        l->m_CharData = file.read(m_meta.dataSize());
        if (m_meta.m_useColors)
            l->m_ColorData = file.read(m_meta.m_colSizex*m_meta.m_colSizey);

//        qDebug() << "ImageLevelEditor loadbin " << m_meta.m_extraDataSize;
        if (m_meta.m_extraDataSize!=0)
            l->m_ExtraData = file.read(m_meta.m_extraDataSize);

  //      l->m_ColorData.fill(0);
//        l->m_CharData.fill(0);

    }
    LoadBinCharsetFilename(file);

    SetLevel(QPoint(0,0));
}

void LImageLevelNES::SetBank(int bank)
{
    if (m_charset!=nullptr)
        m_charset->SetBank(bank);
}

void LImageLevelNES::LoadCharset(QString file, int skipBttes)
{
    ImageLevelEditor::LoadCharset(file,skipBttes);
    if (m_charset!=nullptr)
        m_colorList.m_nesPPU = m_charset->m_colorList.m_nesPPU;
}

void LImageLevelNES::ExportBin(QFile &file)
{


    file.write(m_meta.toHeader());
    if (m_levels.count()==0)
        return;

    for (CharmapLevel* l : m_levels) {
        file.write( l->m_CharData);

        QByteArray d;
        d.resize(l->m_ColorData.count()/4);
        int j=0;
        int w= m_meta.m_colSizex;
        int h= m_meta.m_colSizey;

        for (int y=0;y<h/2;y++)
            for (int x=0;x<w/2;x++) {
                int p = 2*((y*w) + x);



                d[j++]  = (l->m_ColorData[p]&3) |
                        ((l->m_ColorData[p+1]&3)<<2) |
                        ((l->m_ColorData[p+w]&3)<<4)|
                                   ((l->m_ColorData[p+w+1]&3)<<6);


            }
        //        qDebug() << d.size();
        file.write(d);
        if (l->m_ExtraData.count()!=0)
            file.write( l->m_ExtraData);

    }

}
