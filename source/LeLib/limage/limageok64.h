#ifndef LIMAGEOK64_H
#define LIMAGEOK64_H
#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limage.h"

class LImageOK64 : public LImage
{
public:
    LImageOK64();
    LImageOK64(LColorList::Type t);
    QByteArray m_data;
    void Initialize(int width, int height);

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void SaveBin(QFile &f) override;
    void LoadBin(QFile &f) override;

    void LoadQImage(QString filename);

    virtual void ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center) override;
    void fromQImage(QImage* img, LColorList& lst) override;


    void Release() override;

    void ApplyToLabel(QLabel* l) override {

    }
    void Clear() override {
    }

    void ExportBin(QFile &file) override;
    void ImportBin(QFile &file) override;



};

#endif // LIMAGEOK64_H
