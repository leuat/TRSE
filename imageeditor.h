#ifndef IMAGEEDITOR_H
#define IMAGEEDITOR_H

#include <QString>
#include <QVector>
#include "source/LeLib/limage/limagefactory.h"
#include "source/LeLib/limage/lcolorlist.h"

class ImageType {
public:
    QString name;
    LImage::Type type;
    LColorList::Type colorType;
    ImageType() {

    }

    ImageType(QString n,LImage::Type t, LColorList::Type colType) {
        type = t;
        name = n;
        colorType = colType;
    }
};



class ImageEdit {
public:
    ImageEdit(ImageType* t, QString name);
    ImageEdit(LImage* image, ImageType* it,QString name);


    ImageType* m_imageType;
    QString m_name;
    QString m_fileName;

    //LColorList* m_colorList = nullptr;
    void Initialize();

    LImage* m_image;
    LImage* m_temp;
    QVector<LImage*> m_undo;
    void Undo();
    void AddUndo();
    void setName();
    const int m_undoMax = 10;


};


#endif // IMAGEEDITOR_H
