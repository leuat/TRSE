#ifndef LIMAGEMETACHUNK_H
#define LIMAGEMETACHUNK_H

#include "source/LeLib/limage/limagecontainer.h"
#include "source/LeLib/limage/limagenes.h"
#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/limage/charsetimage.h"

class LMetaChunkItem : public LImageContainerItem {
public:

//    QByteArray m_header;
    QByteArray m_data;
    QByteArray m_attributes;

    LImageNES* m_charImage;


    LMetaChunkItem() {
  //      m_header.resize(HEADER_SIZE);
    }

    QByteArray ToQByteArray(int mask) override;

    void Clear(int val=0) override {
        m_data.fill(val);
    }

    LMetaChunkItem(QByteArray& a, int index) {
        Init(2,2);
    }




    void FillColor(int color, int idx) {
        m_data.fill(color);
    }

    void Init(int w, int h) override {
        m_height = h;
        m_width = w;
        m_data.resize(m_height*m_width);
        m_data.fill(0);
        m_attributes.resize(m_height*m_width);
        m_attributes.fill(0);
    }

    void setPixel(float x, float y, uchar color, uchar bitMask);
    void setPixelAttrib(float x, float y, uchar color, uchar bitMask);
    uchar getPixel(float x, float y, uchar bitMask);
    uchar getPixelAttrib(float x, float y, uchar bitMask);



};


//class LImageMetaChunk : public LImageQImage, public LImageContainer
//class LImageMetaChunk : public LImageNES, public LImageContainer
class LImageMetaChunk : public CharsetImage, public LImageContainer
{
public:
    LImageMetaChunk(LColorList::Type t);
//    LImageSprites(LColorList::Type t);
/*    LImageMetaChunk() : CharsetImage() {

    }*/
//    LImageSprites2() {}


    LMetaChunkItem m_copy;
    uchar m_currentAttribute = 0;


    LImage* m_img = nullptr, *m_charset = nullptr;
    QPoint getPos(int x, int y);
/*    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;

*/
    void CopyFrom(LImage* mc) override;
    int m_pixelWidth = 8, m_pixelHeight = 8;


    QString GetCurrentDataString() override {
        if (m_current<0) return "";
        QString blockSize = " char: " + Util::numToHex(m_currentChar);
//        blockSize += ", " +QString::number(m_items[m_current]->m_height)+")";
        return "  Char : " + QString::number(m_current) + "/" +
                QString::number(m_items.count()) + blockSize;
    }


   void Copy();

   void setPixel(int x, int y, unsigned int color) override;
   void LoadCharset(QString file, int skipBytes) override;

   void InitPens() override;

   void SetPalette(int pal) override;

   QStringList getPaletteNames() override;



   unsigned int getPixel(int x, int y) override;

   virtual bool isNes() override {
       if (m_charset!=nullptr)
           return m_charset->isNes();
       return false;
   }
   virtual bool isSnes() override {
       if (m_charset!=nullptr)
           return m_charset->isSnes();
       return false;
   }

   void SaveBin(QFile& file) override;
   void LoadBin(QFile& file) override;
   void SetColor(uchar col, uchar idx) override;
   void SetColor(uchar col, uchar idx, LImageMetaChunk& s);

   LMetaChunkItem* getCur();
   LColorList::Type getColorType() override {
       if (m_charset==nullptr)
           return LColorList::C64;

       return m_charset->m_colorList.m_type;
   }

   bool KeyPress(QKeyEvent *e) override;

   void CopyChar() override;
   void PasteChar() override;
   void setMultiColor(bool doSet) override {
       if (m_charset!=nullptr)
           m_charset->setMultiColor(doSet);
   }



//   void ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center) override;



   void AddNew(int w, int h) override;

   virtual void FlipHorizontal() override;
   virtual void FlipVertical() override;
   virtual void Transform(int x, int y) override;
   virtual int getContainerCount() override {return m_items.count();}

   CharsetImage* getCharset() override { return (CharsetImage*)m_charset; }



    unsigned int getCharPixel(int pos,  int pal,int x, int y) override;


    QPixmap ToQPixMap(int chr) override;

    void ExportBin(QFile &file) override;
//    virtual void ImportBin(QFile &file) {}


    virtual QString getMetaInfo() override;

};

#endif // LIMAGEMETACHUNK_H
