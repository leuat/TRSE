/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * |yright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
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

    virtual void ReInitialize() override {}

    void setPixel(int x, int y, unsigned int color) override;
    unsigned int getPixel(int x, int y) override;

    void SaveBin(QFile &f) override;
    void LoadBin(QFile &f) override;

    void LoadQImage(QString filename);

    virtual void ToQImage(LColorList& lst, QImage& img, float zoom, QPointF center) override;
    void fromQImage(QImage* img, LColorList& lst) override;

    void ExportBlackWhite(QFile &file, int p1, int p2, int p3, int p4) override;

    void Release() override;

    void ApplyToLabel(QLabel* l) override;
    void Clear() override {
        if (m_qImage)
            m_qImage->fill(QColor(0,0,0,255));
    }



    // Specific stuff

    QImage* Resize(int x, int y, LColorList& lst, float contrast, float shift, float hsv, float sat, QPointF scale, bool useDither);
    QImage* Blur(float rad);
    QImage* ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect);

    void CreateGrid(int x, int y, QColor color, int strip,float zoom, QPointF center, float scale);


    void CopyFrom(LImage *img) override;
};
#endif // LIMAGEQIMAGE_H
