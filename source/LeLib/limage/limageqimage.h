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
#include "ssim.h"
#include <QImage>


class LImageQImage : public LImage, public SSIM {
public:
    LImageQImage() {}
    LImageQImage(LColorList::Type t);
    QImage* m_qImage = nullptr;
    QVector<QSharedPointer<QImage>> m_banks;
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

    // SSIM stuff
    double getVal(int x, int y) override;
    double getWidth() override {return m_width;}
    double getHeight() override {return m_height;}
    double getL() override {return 8;}

    QPoint getPixelPosition(int x, int y);

    virtual QPixmap ToQPixMap(int chr) override;

    int getCharWidthDisplay() override;

    int getCharHeightDisplay() override;

    int getGridWidth() override;

    virtual void ToQImage(LColorList& lst, QImage& img, double zoom, QPointF center) override;
    void fromQImage(QImage* img, LColorList& lst) override;

    void ExportBlackWhite(QFile &file, int p1, int p2, int p3, int p4) override;

    void RemapCharset(QImage* other, int cw, int ch, int bw, int bh, int dw, int dh, int allowance);

    void Release() override;

    void ApplyToLabel(QLabel* l) override;
    void Clear(int val=0) override {
        if (m_qImage)
            m_qImage->fill(QColor(val,val,val,255));
    }

    bool KeyPress(QKeyEvent *e) override;


    // Specific stuff

    QImage* Resize(int x, int y, LColorList& lst, float contrast, float shift, float hsv, float sat, QPointF scale, bool useDither);
    QImage* Blur(float rad);
    QImage* ApplyEffectToImage(QImage& src, QGraphicsBlurEffect *effect);

    void CreateGrid(int x, int y, QColor color, int strip,double zoom, QPointF center, double scale);


    void CopyFrom(LImage *img) override;

    double SSIM(LImageQImage* other);
    void calcMeanSigma(double& mu, double& s);
    double calcLuminosity(LImageQImage* other);
    double calcStructure(LImageQImage* other);
    double calcContrast(LImageQImage* other);
    double c1();
    double c2();
    double c3();

};
#endif // LIMAGEQIMAGE_H
