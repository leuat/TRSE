#ifndef LIMAGEGENERICSPRITES
#define LIMAGEGENERICSPRITES
#include <QKeyEvent>

#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limagecontainer.h"

class LGenericSprite : public LImageContainerItem {
public:

    static const int HEADER_SIZE = 16;
    static const int HEADER_MULTICOLOR = 0;

    QByteArray m_header;

    LImageQImage m_data;

    LGenericSprite() {
        m_header.resize(HEADER_SIZE);
    }

    QByteArray ToQByteArray(int mask) override;




    void Clear(int val=0) override {
        m_data.m_qImage->fill(val);
    }

    LGenericSprite(QByteArray& a, int index, int mask) {
        Init(1,1);

    }

    void FillColor(int color, int idx) {
    }

    void Init(int w, int h) override {
        m_height = h;
        m_width = w;
        m_data.Initialize(m_width, m_height);
        m_header.resize(HEADER_SIZE);
    }

    void setPixel(double x, double y, uchar color, uchar bitMask);
    uchar getPixel(double x, double y, uchar bitMask);


};


class LImageGenericSprites : public LImageQImage, public LImageContainer
{
public:
    LImageGenericSprites(LColorList::Type t);
//    LImageSprites(LColorList::Type t);

//    LImageSprites2() {}


    LGenericSprite m_copy;

    QPoint getActualPixelWidth() override;

    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;

    virtual QString getMetaInfo() override;

    void CopyFrom(LImage* mc) override;

    QString GetCurrentDataString() override {
        if (m_current<0) return "";
        QString blockSize = " block size (" +QString::number(m_items[m_current]->m_width);
        blockSize += ", " +QString::number(m_items[m_current]->m_height)+")";
        return "  Sprite : " + QString::number(m_current) + "/" +
                QString::number(m_items.count()) + blockSize;
    }


   void Copy();

   void setPixel(int x, int y, unsigned int color) override;

   unsigned int getPixel(int x, int y) override;

   void SaveBin(QFile& file) override;
   void LoadBin(QFile& file) override;
   void SetColor(uchar col, uchar idx) override;
//   void SetColor(uchar col, uchar idx, LSprite& s);

   bool KeyPress(QKeyEvent *e) override;

   void CopyChar() override;
   void PasteChar() override;
   void setMultiColor(bool doSet) override {

   }



   void InitPens() override;
   void ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center) override;


   void ToggleSpriteMulticolor();
   void MegaTransform(int flip, int x, int y);

   void AddNew(int w, int h) override;

   virtual void FlipHorizontal() override;
   virtual void FlipVertical() override;
   virtual void Transform(int x, int y) override;
   virtual int getContainerCount() override {return m_items.count();}

   virtual int getGridWidth() override;
   virtual int getGridHeight()  override;




};

#endif // LIMAGESPRITES2_H
