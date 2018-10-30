#ifndef LIMAGESPRITES2_H
#define LIMAGESPRITES2_H
#include <QKeyEvent>
#include "source/LeLib/limage/multicolorimage.h"

#include "source/LeLib/limage/charsetimage.h"
#include "source/LeLib/util/util.h"


class LSprite {
public:
    int m_blocksWidth= 1;
    int m_blocksHeight = 1;

    int m_pcWidth = 3;
    int m_pcHeight = 3;

    QVector<PixelChar> m_data;

    LSprite() {

    }

    QByteArray toQByteArray(int mask);

    LSprite(QByteArray& a, int index, int mask) {
        int c = index;
        Initialize(1,1);
        for (int y=0;y<3;y++) {
            for (int x=0;x<3;x++) {
                for (int j=0;j<8;j++) {
                    int d = c+3*j;
                    m_data[y*3+x].p[j] = PixelChar::reverse(a[d]);
                    m_data[y*3+x].p[j] = m_data[y*3+x].flipSpriteBit(j, mask);
                }
                c++;
            }
            c+=7*3;
        }

    }

    void Initialize(int bw, int bh) {
        m_blocksHeight = bh;
        m_blocksWidth = bw;
        m_data.resize(bh*bw*m_pcWidth*m_pcHeight);
    }
    PixelChar* GetSetData(float x, float y, float& ix, float& iy, uchar bitMask);

    void setPixel(float x, float y, uchar color, uchar bitMask);
    uchar getPixel(float x, float y, uchar bitMask);


};


class LImageSprites2 : public CharsetImage
{
public:
    LImageSprites2(LColorList::Type t);
//    LImageSprites(LColorList::Type t);

//    LImageSprites2() {}


    QVector<LSprite> m_sprites;
    LSprite m_copy;


    void AddSprite(int x, int y);

    void ImportBin(QFile& f) override;
    void ExportBin(QFile& f) override;


    void CopyFrom(LImage* mc) override;

    QString GetCurrentDataString() override {
        if (m_currencChar<0) return "";
        QString blockSize = " block size (" +QString::number(m_sprites[m_currencChar].m_blocksWidth);
        blockSize += ", " +QString::number(m_sprites[m_currencChar].m_blocksHeight)+")";
        return "  Sprite : " + QString::number(m_currencChar) + "/" +
                QString::number(m_sprites.count()) + blockSize;
    }


   void Copy();

   void setPixel(int x, int y, unsigned int color) override;

   unsigned int getPixel(int x, int y) override;

   void SaveBin(QFile& file) override;
   void LoadBin(QFile& file) override;
   void SetColor(uchar col, uchar idx) override;
   void SetColor(uchar col, uchar idx, LSprite& s);

   bool KeyPress(QKeyEvent *e) override;

   void CopyChar() override;
   void PasteChar() override;

   void MegaTransform(int flip, int x, int y);

   virtual void FlipHorizontal() override;
   virtual void FlipVertical() override;
   virtual void Transform(int x, int y) override;


};

#endif // LIMAGESPRITES2_H
