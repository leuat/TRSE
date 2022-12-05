#include "limagelevelsnes.h"
#include "limageio.h"

LImageLevelSNES::LImageLevelSNES(LColorList::Type t) : ImageLevelEditor(t)
{
    m_type = LImage::Type::LevelEditorSNES;
    m_supports.displayBank = false;
    m_charWidthDisplay = 16;
    m_gridWidthDisplay = 16;
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

    m_meta.m_is16bit = true;

    //    m_colorList.InitNES4();
    //    m_colorList.InitNES();

//    m_metaParams.append(new MetaParameter("color_width","Color width",20,2,1000));
  //  m_metaParams.append(new MetaParameter("color_height","Color height",10,2,1000));


    PerformConstrainColours(QVector<int>() <<0<<1<<2<<3<<4<<5<<6<<7<<8);
}

unsigned int LImageLevelSNES::getPixel(int x, int y)
{
    if (m_charset==nullptr)
        return 0;

    m_charset->m_returnActualColor = false;
    int pos;
    if (!PixelToPos(x,y, pos,m_meta.m_width, m_meta.m_height))
        return 0; // out of bounds

    int cx = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_X);
    int cy = m_footer.get(LImageFooter::POS_CURRENT_DISPLAY_Y);

    int shift=0;
    shift += (x%16)/(16/cx);
    shift += ((y%16)/(16/cy))*m_charset->m_charWidthDisplay;

    uchar vl = m_currentLevel->m_CharData[pos];
    uchar vh = m_currentLevel->m_CharDataHi[pos];
    uchar col = m_currentLevel->m_ColorData[pos];

    pos = vl + shift;

    m_charset->m_currentChar = pos;
/*    if (pos>6)
    if (rand()%100>98)
        qDebug() << Util::numToHex(pos);*/
    m_charset->m_footer.set(LImageFooter::POS_DISPLAY_CHAR,1);
    m_charset->m_footer.set(LImageFooter::POS_CURRENT_DISPLAY_REPEAT,0);
//    m_charset->m_colorList.m_curPal = col;
    int lx = 16/cx;
    int ly = 16/cy;
    int xx = (((x%lx))/(float)(lx))*m_charset->m_width;
    int yy = (((y%ly))/(float)(ly))*m_charset->m_height;
    auto ch = dynamic_cast<LImageMetaChunk*>(m_charset);
    if (ch!=nullptr) {
        ch->m_current = pos;
    }
//    m_charset->m_currentChar = pos;
//    qDebug() <<m_charset->m_colorList.m_bpp.x();
    return m_charset->getPixel(xx,yy)+col*pow(2,m_charset->m_colorList.m_bpp.x());

/*    if (rand()%1000>998) {
        for (int i=0;i<12;i++)
                qDebug() << i<<Util::numToHex(m_charset->m_colorList.m_nesPPU[i]);

    }
*/
}

void LImageLevelSNES::LoadCharset(QString file, int skipBttes)
{
    if (!QFile::exists(file))
        return;

    m_charset = nullptr;
    m_charset = LImageIO::Load(file);

    if (m_charset==nullptr)
        return;
    m_charsetFilename = file;
//    m_colorList.m_nesPPU.clear();
   // qDebug() <<m_charset;

//    if (m_charset!=nullptr)
  //      m_colorList.m_nesPPU = m_charset->m_colorList.m_nesPPU;

    m_charWidthDisplay = m_charset->m_charWidthDisplay;
    m_gridWidthDisplay = m_charset->m_gridWidthDisplay;
    m_charHeightDisplay = m_charset->m_charHeightDisplay;;//m_meta.m_height;
//    m_charset->m_colorList.m_nesPPU.clear();
//    m_charset->InitPens();
//    InitPens();
    m_colorList.m_list = m_charset->m_colorList.m_list;
    m_colorList.m_nesPPU = m_charset->m_colorList.m_nesPPU;
    m_colorList.setNoBitplanes(m_charset->m_colorList.m_bpp.x());
//    qDebug() << Util::numToHex(m_charset->m_colorList.m_nesPPU[4*2]);

}

void LImageLevelSNES::InitPens()
{
//    m_colorList.InitSNESPens();



    m_colorList.DefaultPen(LPen::FixedSingleNumbers,8);

}

void LImageLevelSNES::ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center)
{
    if (m_charset==nullptr)
        return;
//#pragma omp parallel for
    for (int i=0;i<m_width;i++)
        for (int j=0;j<m_height;j++) {

            float xp = ((i-center.x())*zoom)+ center.x();
            float yp = ((j-center.y())*zoom) + center.y();

            unsigned int col = getPixel(xp,yp);// % 16;
            col = m_charset->m_colorList.m_nesPPU[col];
//            img->setPixel(i,j,QRgb(col));
            img.setPixel(i,j,m_charset->m_colorList.get(col).color.rgb());
        }
    //return img;

}

void LImageLevelSNES::CopyFrom(LImage *mc)
{
    LImageLevelSNES* c = dynamic_cast<LImageLevelSNES*>(mc);
    if (c!=nullptr) {
        if (c->m_charset==nullptr)
            return;

        m_width = c->m_width;
        m_height = c->m_height;
        CharmapGlobalData d = c->m_meta;
        //d.m_sizex = 1;
        //d.m_sizey = 1;
        m_meta = c->m_meta;
        ReInitialize();

        m_currentChar = c->m_currentChar;

        m_forcePaintColorAndChar = c->m_forcePaintColorAndChar;
        for (int i=0;i<m_meta.m_sizex*m_meta.m_sizey;i++) {
            m_levels[i]->m_CharData = c->m_levels[i]->m_CharData;
            m_levels[i]->m_CharDataHi = c->m_levels[i]->m_CharDataHi;
            m_levels[i]->m_ColorData = c->m_levels[i]->m_ColorData;
            m_levels[i]->m_ExtraData = c->m_levels[i]->m_ExtraData;
        }
        /*for (int i=0;i<m_meta.dataSize();i++) {
                m_currentLevel->m_CharData[i]  = c->m_currentLevel->m_CharData[i];
                m_currentLevel->m_ColorData[i]  = c->m_currentLevel->m_ColorData[i];
            }
            for (int i=0;i<c->m_currentLevel->m_ExtraData.count();i++) {
                m_currentLevel->m_ExtraData[i] = c->m_currentLevel->m_ExtraData[i];
            }
            */
        m_footer = c->m_footer;
        m_charset = c->m_charset;
        m_charWidthDisplay = c->m_charWidthDisplay;
        m_gridWidthDisplay = c->m_gridWidthDisplay;
        m_writeType = c->m_writeType;
        //        for (int i=0;i<4;i++)
        //          m_extraCols[i] = c->m_extraCols[i];
        //SetLevel(QPoint(0,0));
        m_currentLevelPos = c->m_currentLevelPos;
        SetLevel(m_currentLevelPos);
        renderPathGrid = c->renderPathGrid;
        m_scale = c->m_scale;
        m_colorList.CopyFrom(&c->m_colorList);
        m_colorList.m_isLevelEditor = mc->m_colorList.m_isLevelEditor;;
  }

}

LImage *LImageLevelSNES::getCharset() {
    return m_charset;
}

void LImageLevelSNES::ReInitialize()
{
    ImageLevelEditor::ReInitialize();
    m_meta.m_is16bit = true;

}

void LImageLevelSNES::ExportBin(QFile &file)
{
    file.write(m_meta.toHeader());
    if (m_levels.count()==0)
        return;

    for (CharmapLevel* l : m_levels) {


        QByteArray data;
        for (int i=0;i<l->m_CharData.length();i++) {
            data.append(l->m_CharData[i]);
            data.append((l->m_ColorData[i]&7)<<2 | (l->m_CharDataHi[i]&3));
        }
        file.write( data);
        if (l->m_ExtraData.length()!=0)
            file.write( l->m_ExtraData);

    }

}
