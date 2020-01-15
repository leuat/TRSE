/*c
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "imageleveleditor.h"
#include "source/LeLib/util/util.h"
#include "source/Compiler/syntax.h"

void ImageLevelEditor::SetLevel(QPoint f)
{
    // Clamp
    f.setX(Util::clamp(f.x(), 0, m_meta.m_sizex-1));
    f.setY(Util::clamp(f.y(), 0, m_meta.m_sizey-1));

    m_currentLevelPos = f;
    if (f.x()<0 || f.x()>=m_meta.m_sizex || f.y()<0 || f.y()>=m_meta.m_sizey)
        return;
    m_currentLevel = m_levels[f.x() + f.y()*m_meta.m_sizex];

//    qDebug() << "Current colors:";

    for (int i=0;i<3;i++)
        m_extraCols[i] = m_currentLevel->m_ExtraData[i];



    m_background = m_currentLevel->m_ExtraData[0];
//        qDebug() << QString::number(m_currentLevel->m_ExtraData[i]);
    if (m_charset==nullptr)
        return;


    if (m_type!=LImage::LevelEditorNES) {
        m_charset->SetColor(m_currentLevel->m_ExtraData[0], 0);
        m_charset->SetColor(m_currentLevel->m_ExtraData[1], 1);
        m_charset->SetColor(m_currentLevel->m_ExtraData[2], 2);
    }
    else {
        SetColor(m_currentLevel->m_ExtraData[0], 0);
        SetColor(m_currentLevel->m_ExtraData[1], 1);
        SetColor(m_currentLevel->m_ExtraData[2], 2);
    }


/*    for (int i=0;i<3;i++) {
        if (m_extraCols[i]!=0)
            qDebug() << "Extra data" << QString::number(m_extraCols[i]);
    }
*/
}

ImageLevelEditor::ImageLevelEditor(LColorList::Type t)  : MultiColorImage(t)
{
    m_width = 320;
    m_height = 200;
    m_scaleX = 1.0f;
    m_bitMask = 0b11;
    m_noColors = 4;
    m_scale = 1;
    m_minCol = 0;
    m_type = LImage::Type::LevelEditor;
    m_charWidth = 40;
    m_charHeight = 25;

    m_supports.asmExport = false;
    m_supports.binaryLoad = false;
    m_supports.binarySave = false;
    m_supports.flfSave = true;
    m_supports.flfLoad = true;
    m_supports.asmExport = false;
    m_supports.koalaExport = false;
    m_supports.koalaImport = false;

    m_GUIParams[btnLoadCharset] ="Load Charset";
    m_GUIParams[btn1x1] = "";
    m_GUIParams[btn2x2] = "";
    m_GUIParams[btn2x2repeat] = "";
    m_GUIParams[btnCopy] = "";
    m_GUIParams[btnPaste] = "";
    m_GUIParams[btnFlipH] = "";
    m_GUIParams[btnFlipV] = "";
    m_GUIParams[btnEditFullCharset] = "";

    m_GUIParams[tabCharset] = "1";
    m_GUIParams[tabData] = "1";
    m_GUIParams[tabLevels] = "1";
    m_GUIParams[col3] = "Multicolor 1";
    m_GUIParams[col4] = "Multicolor 2";
    m_supports.displayMC1 = true;
    m_supports.displayMC2 = true;

    m_metaParams.append(new MetaParameter("screen_width","Screen width",20,2,1000));
    m_metaParams.append(new MetaParameter("screen_height","Screen height",10,2,1000));
    m_metaParams.append(new MetaParameter("levels_x","Levels x",4,1,100));
    m_metaParams.append(new MetaParameter("levels_y","Levels y",4,1,100));
    m_metaParams.append(new MetaParameter("data_width","Data width",8,0,100));
    m_metaParams.append(new MetaParameter("data_count","Data count",8,0,100));
    m_metaParams.append(new MetaParameter("data_extra","Extra data",3,0,1000));
    m_metaParams.append(new MetaParameter("use_colors","Use charset colors",1,1,1));

    EnsureSystemColours();
}

void ImageLevelEditor::Initialize()
{
//    m_meta = meta;
    m_meta.Calculate();
//    qDebug() << meta.m_width;
  //  qDebug() << meta.dataSize();
    m_levels.resize(m_meta.m_sizex*m_meta.m_sizey);
    for (int i=0;i<m_meta.m_sizex;i++)
        for (int j=0;j<m_meta.m_sizey;j++)
            m_levels[i + j*m_meta.m_sizex] = new
                    CharmapLevel(m_meta.dataSize(), m_meta.m_extraDataSize);

    //SetLevel(QPoint(0,0));

    m_currentLevel = m_levels[0];
    m_charWidthDisplay = m_meta.m_width;
    m_charHeightDisplay = m_meta.m_height;
    m_width = m_meta.m_width*16;
    m_height = m_meta.m_height*16;
    setMultiColor(m_meta.m_displayMultiColor);

    m_charWidthDisplay = m_meta.m_width;
    m_charHeightDisplay = m_meta.m_height;

}

void ImageLevelEditor::SetColor(uchar col, uchar idx)
{
    if (m_charset==nullptr)
        return;

//    qDebug() << "SetColor being set..." << QString::number(col) << " and " << QString::number(idx);

    if (m_currentLevel->m_ExtraData.count()>=3) {
        m_currentLevel->m_ExtraData[idx] = col;
    }
    m_extraCols[idx] = col;
    if (idx==0)
        m_background = col;

    m_colorList.SetMulticolor(idx,col);

//    m_charset->SetColor(col, idx);
}

void ImageLevelEditor::Clear()
{
    if (m_currentLevel!=nullptr)
        m_currentLevel->Clear();
}

void ImageLevelEditor::SaveBin(QFile &file)
{


    file.write(m_meta.toHeader());
    if (m_levels.count()==0)
        return;
    CharmapLevel* ll = m_levels[0];

    for (CharmapLevel* l : m_levels) {
        file.write( l->m_CharData);
        if (m_meta.m_useColors)
           file.write( l->m_ColorData);

//        qDebug() << "LevelEditor::savebin count " <<l->m_ColorData.count();

        if (l->m_ExtraData.count()!=0)
            file.write( l->m_ExtraData,m_meta.m_extraDataSize);
      //  for (int i=0;i<m_meta.m_extraDataSize;i++)
    //        qDebug() << "Saving extracol: " <<QString::number(l->m_ExtraData[i]);

    }
//    qDebug() << "POS : " <<file.pos();
    AppendSaveBinCharsetFilename(file);


}

void ImageLevelEditor::ExportBin(QFile &file)
{
    file.write(m_meta.toHeader());
    if (m_levels.count()==0)
        return;
    CharmapLevel* ll = m_levels[0];

    for (CharmapLevel* l : m_levels) {
        file.write( l->m_CharData);
        if (m_meta.m_useColors)
           file.write( l->m_ColorData);


        if (l->m_ExtraData.count()!=0)
            file.write( l->m_ExtraData,m_meta.m_extraDataSize);

    }

}

void ImageLevelEditor::LoadBin(QFile &file)
{
    QByteArray h1 = file.read(32);
    m_meta.fromHeader(h1);
    m_meta.Calculate();

    Initialize();
    m_meta.m_startx = 0;
    m_meta.m_starty = 0;
    for (CharmapLevel* l : m_levels) {

        l->m_CharData = file.read(m_meta.dataSize());
        if (m_meta.m_useColors)
            l->m_ColorData = file.read(m_meta.dataSize());
        if (m_meta.m_extraDataSize!=0)
            l->m_ExtraData = file.read(m_meta.m_extraDataSize);

//        for (int i=0;i<m_meta.m_extraDataSize;i++)
  //          qDebug() << "Saving extracol: " <<QString::number(l->m_ExtraData[i]);


    }
    LoadBinCharsetFilename(file);

    SetLevel(QPoint(0,0));


}

void ImageLevelEditor::BuildData(QTableWidget *tbl, QStringList header)
{
    int chunks = m_meta.m_dataChunks;
    int size = m_meta.m_dataChunkSize;
    tbl->setColumnCount(chunks);
    tbl->setRowCount(size);
    tbl->setVerticalHeaderLabels(header);
    for (int i=0;i<chunks;i++)
        tbl->setColumnWidth(i,55);
    int i=0;
    int j=0;
    if (m_currentLevel == nullptr)
        return;
    for (int k=3;k<m_currentLevel->m_ExtraData.count();k++) {
        tbl->setItem(i,j,new QTableWidgetItem(QString::number(m_currentLevel->m_ExtraData[k])));
        if (++i>=size) {
            i=0;
            j++;
        }

    }

}

void ImageLevelEditor::StoreData(QTableWidget *tbl)
{
    int i=0;
    int j=0;
    if (m_currentLevel==nullptr)
        return;
    for (int k=3;k<m_currentLevel->m_ExtraData.count();k++) {
        if (tbl->item(i,j)==nullptr)
                return;
        uchar val = tbl->item(i,j)->text().toInt();


/*        if (val!=0)
        {
            qDebug() << " data " << j << ", "<< i << "  : " << val;
        }*/
        m_currentLevel->m_ExtraData[k] = val;
        if (++i>=tbl->rowCount()) {
            i=0;
            j++;
        }

    }
}

bool ImageLevelEditor::KeyPress(QKeyEvent *e)
{
    QPoint dir(0,0);
    if (e->key()==Qt::Key_W)
        dir.setY(-1);
    if (e->key()==Qt::Key_A)
        dir.setX(-1);
    if (e->key()==Qt::Key_S)
        dir.setY(1);
    if (e->key()==Qt::Key_D)
        dir.setX(1);


    if (e->key()==Qt::Key_C) {
        m_currencChar=0x20;
    }

    if (dir.x()!=0 || dir.y()!=0) {
        SetLevel(m_currentLevelPos + dir);
        return true;
    }
    return false;
}



CharmapLevel *ImageLevelEditor::getLevel(int i, int j)
{
    return m_levels[i+j*m_meta.m_sizex];
}


QVector<QPixmap> ImageLevelEditor::CreateIcons()
{
    QVector<QPixmap> lst;
    for (int i=0;i<m_meta.m_sizex;i++)
        for (int j=0;j<m_meta.m_sizey;j++) {
            QImage img = getLevel(i,j)->createImage(64,m_colorList,m_meta.m_width, m_meta.m_height);
            QPixmap pixmap = QPixmap::fromImage(img);
            //QIcon icon(pixmap);
            lst.append(pixmap);
        }
    return lst;
}

QString ImageLevelEditor::getMetaInfo()
{
    QString txt="";

    txt+= "Char Data & color size: " + QString::number(m_meta.dataSize()) + " bytes\n";
    txt+= "Extra data size: " + QString::number(m_meta.m_extraDataSize) + " bytes\n";
    txt+= "Level size: " + QString::number(m_meta.levelSize()) + " bytes\n";
    txt+= "Total no levels: " + QString::number(m_meta.m_sizex*m_meta.m_sizey) + " \n";
    txt+= "Total size: " + QString::number(m_meta.totalSize()) + " bytes\n";

    txt+="\n\nThe TRSE Level Editor comes in two flavors (for now): One for the Commodore computers, and one for NES.\n\n";
    txt+="A level file consists of (levels x*levels y) levels of size (screen width*screen height) bytes. ";
    txt+="These bytes correspond to a specific tile in a charset or metablock charset. The levels can in addition to tile data also contain colour data, ";
    txt+="which can vary depending on the current system (NES/C64) etc. For instance, the NES level editor will export colour attributes directly in the native at. \n\n";
    txt+="In addition to having colour/tile data, you can also specify a user-defined matrix (data_width*data_count + extra_data) of byte data for custom use - monster positions, items etc, for each of your levels. \n";
    m_meta.m_sizex = getMetaParameter("levels_x")->value;
    m_meta.m_sizey = getMetaParameter("levels_y")->value;
    m_meta.m_width = getMetaParameter("screen_width")->value;
    m_meta.m_height = getMetaParameter("screen_height")->value;
    m_meta.m_dataChunks = getMetaParameter("data_count")->value;
    m_meta.m_dataChunkSize = getMetaParameter("data_width")->value;
    m_meta.m_extraDataSize = getMetaParameter("data_extra")->value;
    m_meta.m_useColors = getMetaParameter("use_colors")->value==1.0;
    m_meta.Calculate();
//    m_meta.m_data = getMetaParameter("screen_height")->value;
 //   m_meta.m_height = getMetaParameter("screen_height")->value;


    return txt;
}


bool ImageLevelEditor::PixelToPos(int x, float y, int& pos, int w, int h)
{
    if (x>=m_width || x<0 || y>=m_height || y<0)
        return false;

/*    x/=16.0;
    y/=16.0;
*/

    x=x/(float)m_width*(float)w;
    y=y/(float)m_height*(float)h;

//    x=x-m_meta.m_startx;
  //  y=(y-(m_meta.m_starty*0.5-0.01));
    if (y<0) return false;
    if (x<0) return false;

    if (x>=w)
        return false;

//    if (rand()%100>98)

  //  qDebug() <<m_meta.m_width;

    pos = x + (int)y*w;
    if ((pos<0 || pos>=w*h))
        return false;

    return true;

}

void ImageLevelEditor::Fix()
{
    QByteArray data = m_currentLevel->m_CharData;
    QByteArray cols = m_currentLevel->m_ColorData;

    for (int i=0;i<m_meta.m_width;i++)
        for (int j=0;j<m_meta.m_height;j++) {
            int x = (i-1)%m_meta.m_width;
            int y = (j)%m_meta.m_height;
            m_currentLevel->m_CharData[i+j*m_meta.m_width] = data[x+y*m_meta.m_width];
            m_currentLevel->m_ColorData[i+j*m_meta.m_width] = cols[x+y*m_meta.m_width];

        }


}

void ImageLevelEditor::setMultiColor(bool doSet)
{
    if (doSet) {
        m_scale = 2;
    }
    else {
        m_scale = 1;

    }

    m_meta.m_displayMultiColor = doSet;
    //    for (int i=0;i<1000;i++)
    //      m_data->c[0] = m_extraCols[0];
    if (m_charset!=nullptr)
        m_charset->setMultiColor(doSet);
}

void ImageLevelEditor::setPixel(int x, int y, unsigned int color)
{
    int pos;

    if (!PixelToPos(x,y, pos,m_meta.m_width, m_meta.m_height))
        return; // out of bounds


    if (m_currentLevel==nullptr)
        return;


    if (m_writeType==Character || m_forcePaintColorAndChar)
        m_currentLevel->m_CharData[pos] = m_currencChar;
    if (m_writeType==Color || m_forcePaintColorAndChar)
        m_currentLevel->m_ColorData[pos] = color;

    //BuildImage();
}

unsigned int ImageLevelEditor::getPixel(int x, int y)
{
    if (m_charset==nullptr)
        return 0;
    int pos;
    if (!PixelToPos(x,y, pos,m_meta.m_width, m_meta.m_height))
        return 0; // out of bounds




    int shift=0;
    if (x%16>=8) shift+=1;
    if (y%16>=8) shift+=40;


    uchar v = m_currentLevel->m_CharData[pos];
    uchar col=0;
    if (m_meta.m_useColors)
        col = m_currentLevel->m_ColorData[pos];

    pos = v + shift;


    if (!m_meta.m_useColors)
        col = m_charset->m_data[pos].c[3];

//    return m_charset->m_data[pos].get(v + (2*x)&7, v+ y&7,m_bitMask);
    int ss = m_scale;

    if (m_meta.m_displayMultiColor) {
        if (col>=8)
            m_charset->setMultiColor(true);
        else {
            m_charset->setMultiColor(false);
            ss = 1;
        }
    }


    int ix = ((x) % (8)/ss)*ss;//- (dx*40);
    int iy = (y) % 8;//- (dy*25);

 //   return pc.get(m_scale*ix, iy, m_bitMask);

//    if (ix>m_meta.m_width)
  //      return 0;



    uint val = m_charset->m_data[pos].get(ix, iy,m_charset->m_bitMask);
    if (m_meta.m_displayMultiColor) {
        if (col<8) // Non-multicolor
            if (val!=m_background)
                val = col;
    }
    if (dynamic_cast<LImageMetaChunk*>(m_charset)!=nullptr) {


        val = m_charset->getCharPixel(v, col, x,y);
            if (!m_charset->m_isMultiColor)
                if (val!=m_background)
                    val = col;
    }


    if (m_meta.m_useColors) {
        if (val==m_charset->m_data[pos].c[3]) {
            val = col&0b00000111;
        }
  //      if ((col&0b00001000)==0b00001000)
    //        val+=1000;
    }
    m_charset->setMultiColor(m_meta.m_displayMultiColor);



    return val;

}

void ImageLevelEditor::CopyFrom(LImage *mc)
{
    ImageLevelEditor* c = dynamic_cast<ImageLevelEditor*>(mc);
    if (c!=nullptr) {
        if (c->m_charset==nullptr)
            return;

        m_width = c->m_width;
        m_height = c->m_height;
        CharmapGlobalData d = c->m_meta;
        //d.m_sizex = 1;
        //d.m_sizey = 1;
        m_meta = c->m_meta;
        Initialize();

        m_currencChar = c->m_currencChar;
        m_forcePaintColorAndChar = c->m_forcePaintColorAndChar;
        for (int i=0;i<m_meta.m_sizex*m_meta.m_sizey;i++) {
            m_levels[i]->m_CharData = c->m_levels[i]->m_CharData;
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
        m_writeType = c->m_writeType;
        for (int i=0;i<4;i++)
            m_extraCols[i] = c->m_extraCols[i];
        //SetLevel(QPoint(0,0));
        m_currentLevelPos = c->m_currentLevelPos;
        SetLevel(m_currentLevelPos);
        renderPathGrid = c->renderPathGrid;
        m_scale = c->m_scale;
    }
    else
    LImage::CopyFrom(mc);

}

void ImageLevelEditor::onFocus()
{
    if (m_charsetFilename!="") {
        LoadCharset(m_charsetFilename,0);
        setMultiColor(m_meta.m_displayMultiColor);
    }
}


void ImageLevelEditor::Resize(CharmapGlobalData newMeta)
{
    if (newMeta.m_dataChunks!=m_meta.m_dataChunks || newMeta.m_dataChunkSize!=m_meta.m_dataChunkSize) {
      //  qDebug() << "Resizing datachunks";
        int odc = m_meta.m_dataChunks;
        int odcs = m_meta.m_dataChunkSize;

        m_meta.m_dataChunks = newMeta.m_dataChunks;
        m_meta.m_dataChunkSize = newMeta.m_dataChunkSize;

        m_meta.m_extraDataSize = 3+m_meta.m_dataChunkSize*m_meta.m_dataChunks;
        m_meta.Calculate();
        for (int i=0;i<m_meta.m_sizex*m_meta.m_sizey;i++) {
            int k = m_levels[i]->m_ExtraData.count();
            QByteArray newData;
            newData.resize(m_meta.m_extraDataSize);
            newData.fill(0);
            for (int j=0;j<m_meta.m_dataChunks;j++) {
                for (int k=0;k<m_meta.m_dataChunkSize;k++) {
                    int nidx = 3+j*m_meta.m_dataChunkSize+k;
                    int oidx = 3+j*odcs + k;
                    char val = 0;
                    if (oidx<m_levels[i]->m_ExtraData.count() &&
                       j<odc && k<odcs)
                        val=m_levels[i]->m_ExtraData[oidx];
                    //qDebug() << odcs << " vs "  <<m_meta.m_dataChunkSize << " and k " << k << " :  " << QString::number(val) ;

                    newData[nidx] = val;
                }

            }
            newData[0] = m_levels[i]->m_ExtraData[0];
            newData[1] = m_levels[i]->m_ExtraData[1];
            newData[2] = m_levels[i]->m_ExtraData[2];
            //m_levels[i]->m_ExtraData.resize(m_meta.m_extraDataSize);
            m_levels[i]->m_ExtraData = newData;
            //for (int j=k;j<m_meta.m_extraDataSize; j++)
            //    m_levels[i]->m_ExtraData[j]=0;
        }

    }
    if (newMeta.m_sizex!=m_meta.m_sizex || newMeta.m_sizey!=m_meta.m_sizey) {
      //  qDebug() << "Resizing datachunks";
        int osx = m_meta.m_sizex;
        int osy = m_meta.m_sizey;
        m_meta.m_dataChunks = newMeta.m_dataChunks;
        m_meta.m_dataChunkSize = newMeta.m_dataChunkSize;
        m_meta.m_sizex = newMeta.m_sizex;
        m_meta.m_sizey = newMeta.m_sizey;

        m_meta.Calculate();

        QVector<CharmapLevel*> nl;
        nl.resize(m_meta.m_sizex*m_meta.m_sizey);
        for (int i=0;i<nl.count();i++)
            nl[i] = nullptr;
        for (int i=0;i<osx;i++)
            for (int j=0;j<osy;j++)
                nl[ i + m_meta.m_sizex*j] = m_levels[i + osx*j];


        for (int i=0;i<nl.count();i++)
            if (nl[i] == nullptr)
                nl[i] = new CharmapLevel(m_meta.dataSize(), m_meta.m_extraDataSize);


        m_levels = nl;
    }

}




int CharmapGlobalData::levelSize() const
{
    return m_levelSize;
}

int CharmapGlobalData::totalSize() const
{
    return m_totalSize;
}

int CharmapGlobalData::dataSize() const
{
    return m_dataSize;
}

QImage CharmapLevel::createImage(int size, LColorList& lst, int width, int height)
{
    QImage img = QImage(size, size, QImage::Format_ARGB32);
    for (int i=0;i<size;i++)
        for (int j=0;j<size;j++) {
            QColor c = QColor(0,0,0);
            int x = (i/(float)size)*(float)width;
            int y = (j/(float)size)*(float)height;
            int pos = x+ width*y;
            uchar val = 32;
            uchar colval = 0;
            if (pos>=0 && pos<m_CharData.count()) {
                val = m_CharData[pos];
                colval = m_ColorData[pos];
            }
            if (val==32)
                colval = 0;
            img.setPixel(i,j, lst.get(colval).color.rgba());
        }

    return img;
}
