#ifndef LIMAGEMETACHUNK_H
#define LIMAGEMETACHUNK_H

#include "source/LeLib/limage/limagecontainer.h"
#include "source/LeLib/limage/limagenes.h"
#include "source/LeLib/limage/limageqimage.h"

class LMetaChunkItem : public LImageContainerItem {
public:

    int HEADER_SIZE = 16;
    int HEADER_MULTICOLOR = 0;

//    QByteArray m_header;

    QByteArray m_data;

    LImageNES* m_charImage;

    LMetaChunkItem() {
  //      m_header.resize(HEADER_SIZE);
    }

    QByteArray ToQByteArray(int mask) override;

    void Clear() override {
        m_data.clear();
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
    }

    void setPixel(float x, float y, uchar color, uchar bitMask);
    uchar getPixel(float x, float y, uchar bitMask);



};


class LImageMetaChunk : public LImageQImage, public LImageContainer
{
public:
    LImageMetaChunk(LColorList::Type t);
//    LImageSprites(LColorList::Type t);

//    LImageSprites2() {}


    LMetaChunkItem m_copy;


    LImage* m_img, *m_charset;

    QPoint getPos(int x, int y);
/*    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;

*/
    void CopyFrom(LImage* mc) override;
    int m_pixelWidth = 8, m_pixelHeight = 8;


    QString GetCurrentDataString() override {
        if (m_current<0) return "";
        QString blockSize = " block size (" +QString::number(m_items[m_current]->m_width);
        blockSize += ", " +QString::number(m_items[m_current]->m_height)+")";
        return "  Sprite : " + QString::number(m_current) + "/" +
                QString::number(m_items.count()) + blockSize;
    }


   void Copy();

   void setPixel(int x, int y, unsigned int color) override;
   void LoadCharset(QString file, int skipBytes) override;

   unsigned int getPixel(int x, int y) override;

   void SaveBin(QFile& file) override;
   void LoadBin(QFile& file) override;
   void SetColor(uchar col, uchar idx) override;
   void SetColor(uchar col, uchar idx, LImageMetaChunk& s);

   bool KeyPress(QKeyEvent *e) override;

   void CopyChar() override;
   void PasteChar() override;
   void setMultiColor(bool doSet) override {

   }



//   void ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center) override;



   void AddNew(int w, int h) override;

   virtual void FlipHorizontal() override;
   virtual void FlipVertical() override;
   virtual void Transform(int x, int y) override;
   virtual int getContainerCount() override {return m_items.count();}




};

#endif // LIMAGEMETACHUNK_H
