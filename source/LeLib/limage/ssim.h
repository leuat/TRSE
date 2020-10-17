#ifndef SSIM_H
#define SSIM_H

#include <math.h>
#include <algorithm>

class SSIM
{
public:
    SSIM();

    static double weight_luminosity;
    static double weight_structure;
    static double weight_contrast;

    virtual double getWidth() = 0;
    virtual double getHeight() = 0;
    virtual double getL() = 0;
    virtual double getVal(int x, int y) = 0;

    double CalcSSIM(SSIM *other);

    void calcMeanSigma(double &mu, double &s);

    double calcLuminosity(SSIM *other);

    double calcStructure(SSIM *other);

    double calcContrast(SSIM *other);

    double c1();

    double c2();

    double c3();

};

#endif // SSIM_H
