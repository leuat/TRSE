/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef TOOLBOXITEM_H
#define TOOLBOXITEM_H

#include "source/LeLib/limage/limage.h"
#include <QPixmap>
#include <QString>


class ToolboxItem
{
public:
    ToolboxItem();
    ToolboxItem(QString name, QString imagefile, QString tooltip);

    float m_size = 4;
    float m_type = 0;

    virtual void IsPreview(int button, bool& isPreview) { }

    virtual void Perform(int x, int y, unsigned char color, LImage* img, bool isPreview, int button) = 0;
    virtual void Init() {}


    void setSize(float f);
    void setRadius(float f);

    QString m_name, m_tooltip;
    QString m_imageFilename;

    QIcon m_icon;
    QImage m_image;
};


class ShapeBox : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    ShapeBox();
    ShapeBox(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile, tooltip) { }

};




class ShapeBoxFilter : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    ShapeBoxFilter();
    ShapeBoxFilter(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }

};

class Spray : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    Spray() {}
    Spray(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }

};

class Dither : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    Dither() {}
    Dither(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile, tooltip) { }

};

class Filler : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;

    Filler() {}
    Filler(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }

    void Fill(int i, int j, unsigned char col, unsigned char testCol, LImage *img );
};

class Line : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;
    QPoint m_start;
    Line() {}
    Line(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }
    void IsPreview(int button, bool& isPreview) override {
        if (button==1)
        isPreview = true;
    }

};


class Circle : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;
    QPoint m_start;
    Circle() {}
    Circle(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }
    void IsPreview(int button, bool& isPreview) override {
        if (button==1)
            isPreview = true;
    }

};

class Box : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;
    QPoint m_start;
    Box() {}
    Box(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }
    void IsPreview(int button, bool& isPreview) override {
        if (button==1)
            isPreview = true;
    }

};

class CopyStamp : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;
    static QPoint m_start;
    static QPoint m_end;
    enum Status { Selecting, DoneSelecting, Stamp, Idle };
    static Status m_status;// = Status::Idle;
    static LImage* m_copy;

    CopyStamp() {m_status=Idle;}
    CopyStamp(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }
    void IsPreview(int button, bool& isPreview) override {
        if (button==1)
        isPreview = true;
    }

    void StampImage(int x, int y, LImage* img);
    void Init() override { m_status = Status::Idle;}


};


class RotateAround : public ToolboxItem {
public:
    void Perform(int x, int y, unsigned char color, LImage *img, bool isPreview, int button) override;
    QPoint m_start;
    QPoint m_end;
    float angele;

    enum Status { Click, Down, Idle };
    Status m_status = Status::Idle;
    LImage* m_copy=nullptr;

    RotateAround() {}
    RotateAround(QString name, QString imagefile, QString tooltip) : ToolboxItem(name, imagefile,tooltip) { }
    void IsPreview(int button, bool& isPreview) override {
        //if (button==1)
        //isPreview = true;
    }

    //void RotateImage(int x, int y, LImage* img);
    void Init() override { m_status = Status::Idle;}


};



#endif // TOOLBOXITEM_H
