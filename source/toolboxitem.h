#ifndef TOOLBOXITEM_H
#define TOOLBOXITEM_H

#include "source/LeLib/limage/limage.h"
#include <QPixmap>
#include <QString>


class ToolboxItem
{
public:
    ToolboxItem();
    ToolboxItem(QString name, QString imagefile);

    float m_size = 4;
    float m_type = 0;

    virtual void IsPreview(int button, bool& isPreview) { }

    virtual void Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button) = 0;
    virtual void Init() {}


    void setSize(float f);
    void setRadius(float f);

    QString m_name;
    QString m_imageFilename;

    QIcon m_icon;
    QImage m_image;
};


class ShapeBox : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    ShapeBox();
    ShapeBox(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};

class Circle : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    Circle() {}
    Circle(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};

class Spray : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    Spray() {}
    Spray(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};

class Dither : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    Dither() {}
    Dither(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

};

class Filler : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    Filler() {}
    Filler(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }

    void Fill(int i, int j, unsigned char col, unsigned char testCol, LImage *img );
};

class Line : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;
    QPoint m_start;
    Line() {}
    Line(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }
    void IsPreview(int button, bool& isPreview) override {
        if (button==1)
        isPreview = true;
    }

};

class CopyStamp : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;
    QPoint m_start;
    QPoint m_end;
    enum Status { Selecting, DoneSelecting, Stamp, Idle };
    Status m_status = Status::Idle;
    LImage* m_copy;

    CopyStamp() {}
    CopyStamp(QString name, QString imagefile) : ToolboxItem(name, imagefile) { }
    void IsPreview(int button, bool& isPreview) override {
        if (button==1)
        isPreview = true;
    }

    void StampImage(int x, int y, LImage* img);
    void Init() override { m_status = Status::Idle;}


};



#endif // TOOLBOXITEM_H
