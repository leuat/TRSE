#ifndef LIMAGECHARSETFIXEDCOLOR_H
#define LIMAGECHARSETFIXEDCOLOR_H

#include "source/LeLib/limage/charsetimage.h"


class LImageCharsetFixedColor : public CharsetImage
{
public:
    LImageCharsetFixedColor(LColorList::Type t);
    QByteArray m_colors;

    void SetColor(uchar col, uchar idx) override;

    void SaveBin(QFile& file) override;
    void LoadBin(QFile& file) override;

//    CharsetImage* getCharset() override { return this; }
//    unsigned int getPixel(int x, int y) override;

 //   void ImportBin(QFile& f) override;
   // void ExportBin(QFile& f) override;
  //  virtual void FromRaw(QByteArray& arr);
    //virtual void ToRaw(QByteArray& arr);
    //void ToQPixMaps(QVector<QPixmap>& map);
  //  virtual void setPixel(int x, int y, unsigned int color) override;


    //void RenderEffect(QMap<QString, float> params) override;

   // void CopyFrom(LImage* mc);
    //bool KeyPress(QKeyEvent *e) override;

    //void setLimitedPixel(int x, int y, unsigned int color);


};

#endif // LIMAGECHARSETFIXEDCOLOR_H
