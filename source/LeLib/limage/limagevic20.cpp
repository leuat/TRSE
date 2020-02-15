#include "limagevic20.h"


LImageVIC20::LImageVIC20(LColorList::Type t)  : CharsetImage (t)
{
    m_width = 88;
    m_height = 184;
    Clear();
    m_type = LImage::Type::VIC20_MultiColorbitmap;
    m_supports.asmExport = false;
    m_supports.binaryLoad = true;
    m_supports.binarySave = true;
    m_supports.koalaImport = false;
    m_supports.koalaExport = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;

    m_colorOrderType = 1;
    m_GUIParams[btnLoadCharset] ="";
    m_GUIParams[btn1x1] = "";
    m_GUIParams[btn2x2] = "";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "";
    m_GUIParams[btnPaste] = "";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";

    m_GUIParams[tabCharset] = "";
    m_GUIParams[tabData] = "";
    m_GUIParams[tabLevels] = "";

    //m_currentMode = FULL_IMAGE;

    m_GUIParams[btnEditFullCharset] = "Full image/charset";

    m_exportParams["StartX"] = 0;
    m_exportParams["EndX"] = 22;
    m_exportParams["StartY"] = 0;
    m_exportParams["EndY"] = 23;

    m_charHeight = 23;
    m_charWidth = 22;

    SetColor(1,1);
    SetColor(2,2);
    SetColor(5,3);

    m_exportParams["Start"] = 0;
    m_exportParams["End"] = std::min(m_charWidth*m_charHeight,192*2);
    m_exportParams["IncludeColors"] = 1;


    m_metaParams.append(new MetaParameter("screen_width","Screen width",20,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",19,2,1000));



}


void LImageVIC20::setMultiColor(bool doSet)
{
    m_charWidthDisplay = m_charWidth;
    m_gridWidthDisplay  = m_charWidth;
    m_charHeightDisplay = m_charHeight;
    if (doSet) {
        m_width = m_charWidth*4;
        m_bitMask = 0b11;
        m_noColors = 4;
        m_scale = 2;
        m_minCol = 0;
    }
    else {
        m_width = m_charWidth*8;
        m_bitMask = 0b1;
        m_noColors = 2;
        m_scale = 1;
        m_minCol = 0;

    }
}
void LImageVIC20::SaveBin(QFile& file)
{
    file.write( ( char * )( &m_background ),  1 );
    file.write( ( char * )( &m_border ), 1 );
    file.write( ( char * )( &m_extraCols[1] ), 1 );
    file.write( ( char * )( &m_extraCols[2] ), 1 );
    file.write( ( char * )( &m_extraCols[3] ), 1 );
    file.write( (char* )(&m_charWidth),1);
    file.write( (char* )(&m_charHeight),1);
    file.write( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );


}

void LImageVIC20::LoadBin(QFile& file)
{
    file.read( ( char * )( &m_background ),1 );
    file.read( ( char * )( &m_border ), 1);
    file.read( ( char * )( &m_extraCols[1] ), 1 );
    file.read( ( char * )( &m_extraCols[2] ), 1 );
    file.read( ( char * )( &m_extraCols[3] ), 1 );
    file.read( ( char * )( &m_charWidth ), 1 );
    file.read( ( char * )( &m_charHeight ), 1 );
    file.read( ( char * )( &m_data ),  m_charWidth*m_charHeight*12 );


//    qDebug() << "CW" << QString::number(m_charWidth);;
  //  qDebug() << "Ch" << QString::number(m_charHeight);;

    m_width = 8*m_charWidth;
    m_height = 8*m_charHeight;

    m_exportParams["End"] = std::min(m_charWidth*m_charHeight,192*2);

    m_exportParams["StartX"] = 0;
    m_exportParams["EndX"] = m_charWidth;
    m_exportParams["StartY"] = 0;
    m_exportParams["EndY"] = m_charHeight;



}

void LImageVIC20::setPixel(int x, int y, unsigned int color)
{
    int dx = x/(8/m_scale);
    int dy = y/8;
    int i = dx + m_charWidth*dy;
    if (i>m_charHeight*m_charWidth || i>=192*2 || i<0)
        return;

    CharsetImage::setPixel(x,y,color);
}


void LImageVIC20::ToRaw(QByteArray &arr)
{
    int start = m_exportParams["Start"];
    int end = m_exportParams["End"];
    int size = start-end;
//    qDebug() << start << " " << end;

    arr.resize(0);
    for (int i=0;i<m_charHeight;i+=2) {
        for (int j=0;j<m_charWidth;j++) {
        PixelChar& pc = m_data[j+i*m_charWidth];
        for (int k=0;k<8;k++)
            arr.append(PixelChar::reverse(PixelChar::VIC20Swap(pc.p[k])));


        PixelChar& pc2 = m_data[j+(i+1)*m_charWidth];
            for (int k=0;k<8;k++)
                arr.append(PixelChar::reverse(PixelChar::VIC20Swap(pc2.p[k])));
        }
    }
    if (m_exportParams["IncludeColors"] != 0) {
        QByteArray cols;
        cols.resize(size);

        m_extraCols[0]=m_background;


        for (int i=start;i<end;i++) {
            PixelChar& pc = m_data[i];
            uchar v = getVariableColor(&pc);
            if (v==255)v=0;
            cols[(i-start)] = (v & 7) +8;
        }
        arr.append(cols);
    }
}

QString LImageVIC20::getMetaInfo()
{
    QString txt="A VIC-20 Bitmap is made up of NxM chars. Colours are severely restricted, with a selection of 3 global colors (background, multicolor 1 and multicolor 2).\n";
    txt+="In addition to this, you also have 1 available color (0-7) for each 8x8 cell.\n\n";
    m_charWidth = getMetaParameter("screen_width")->value;
    m_charHeight = getMetaParameter("screen_height")->value;
    int chars = m_charWidth*m_charHeight/2;
    txt+= "Chars (8x16) used: " + QString::number(chars) +"\n";
    if (chars>192)
        txt+= "WARNING more than 192 chars! Will be truncated. \n";

    txt+= "Data size: " + QString::number(m_charWidth*m_charHeight*8) + " bytes\n";
    txt+= "Color size: " + QString::number(m_charWidth*m_charHeight) + " bytes\n";
    txt+= "Total size: " + QString::number(m_charWidth*m_charHeight*9) + " bytes\n";
    txt+= "Pixel dimensions " + QString::number(m_charWidth*4) + "x" +QString::number(m_charHeight*8);


    return txt;
}

