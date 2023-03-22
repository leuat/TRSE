#ifndef LIMAGELEVELGENERIC_H
#define LIMAGELEVELGENERIC_H

#include "imageleveleditor.h"


class LImageLevelGeneric : public ImageLevelEditor
{
public:
    LImageLevelGeneric(LColorList::Type t);
    LImage* m_charset = nullptr;

    void InitPens() override;

//    void Initialize(int width, int height) override;

//    virtual void SaveBin(QFile &file) override;
//    virtual void LoadBin(QFile &file) override;

    unsigned int getPixel(int x, int y) override;
    void LoadCharset(QString file, int skipBttes) override;

//    virtual QByteArray getBinaryExportData() override;

    void ToQImage(LColorList &lst, QImage &img, double zoom, QPointF center) override;

//    void setPixel(int x, int y, unsigned int color) override;


    void ExportBin(QFile &file) override;
    LImage* getCharset() override;

    void ReInitialize() override;
//    void SetPalette(int pal) override;
//    QStringList getPaletteNames() override;
//    QString GetCurrentDataString() override;
    void CopyFrom(LImage *mc);

};

#endif // LIMAGELEVELGAMEBOY_H
