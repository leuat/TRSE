#ifndef LIMAGEQIMAGE_H
#define LIMAGEQIMAGE_H
#include "source/LeLib/limage/lcolorlist.h"
#include "source/LeLib/limage/limage.h"
#include <QImage>


class LImageQImage : public LImage {
public:
    LImageQImage() {}
    LImageQImage(LColorList::Type t);
    QImage* m_qImage = nullptr;
    ~LImageQImage() {
        Release();
    }

    void Initialize(int width, int height) override;

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void SaveBin(QFile &f) override;
    void LoadBin(QFile &f) override;

    void LoadQImage(QString filename);

    virtual void ToQImage(LColorList& lst, QImage* img, float zoom, QPointF center) override;
    void fromQImage(QImage* img, LColorList& lst) override;


    void Release() override;

    void ApplyToLabel(QLabel* l) override;
    void Clear() override {
        if (m_qImage)
            m_qImage->fill(QColor(0,0,0,255));
    }



    // Specific stuff

    QImage* Resize(int x, int y, LColorList& lst, float contrast, float shift, float hsv, float sat, QPointF scale);
    QImage* Blur(float rad);
    QImage* ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect);

    void CreateGrid(int x, int y, QColor color, int strip,float zoom, QPoint center);

};

#endif // LIMAGEQIMAGE_H
