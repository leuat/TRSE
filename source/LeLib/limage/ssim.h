#ifndef SSIM_H
#define SSIM_H

#include <math.h>
#include <algorithm>

class SSIM
{
public:
    SSIM();


    virtual double getWidth() = 0;
    virtual double getHeight() = 0;
    virtual double getL() = 0;
    virtual double getVal(int x, int y) = 0;

    double CalcSSIM(SSIM *other)
    {
        double d = 1.0-calcLuminosity(other)*calcContrast(other)*calcStructure(other);
        return std::max(d,0.0);
    //    return calcStructure(other);
    }

    void calcMeanSigma(double &mu, double &s)
    {
        mu = 0;
        s = 0;
        //qDebug() << m_height << m_qImage->height();
        for (int y=0;y<getHeight();y++)
            for (int x=0;x<getWidth();x++) {
                mu+= getVal(x,y);
            }
        mu = mu/(double)(getHeight()*getWidth());

        s = 0;
        for (int y=0;y<getHeight();y++)
            for (int x=0;x<getWidth();x++) {
                s+= pow(getVal(x,y)-mu,2);
            }

        s = s/(double)(getHeight()*getWidth());

    //    qDebug() << "mean / sigma " << mu << s;

    }

    double calcLuminosity(SSIM *other)
    {
    /*    if (other->m_width != m_width)
            return -1;
        if (other->m_height != m_height)
            return -1;
    */
        double mu_x=0, s_x=0;
        double mu_y=0, s_y=0;
        calcMeanSigma(mu_x, s_x);
        other->calcMeanSigma(mu_y, s_y);
       // qDebug() << mu_x << mu_y;
        return (2.0*(mu_x*mu_y) + c1())/(mu_x*mu_x + mu_y*mu_y +c1());
    }

    double calcStructure(SSIM *other)
    {
      /*  if (other->m_width != m_width)
            return -1;
        if (other->m_height != m_height)
            return -1;
    */
        double mu_x, s_x;
        double mu_y, s_y;
        calcMeanSigma(mu_x, s_x);
        other->calcMeanSigma(mu_y, s_y);

        double sum = 0;
        for (int y=0;y<getHeight();y++)
            for (int x=0;x<getWidth();x++) {
                int dx = (double)x/getWidth()*(double)other->getWidth();
                int dy = (double)y/getHeight()*(double)other->getHeight();
                sum += (getVal(x,y)-mu_x)*(other->getVal(dx,dy) - mu_y);
            }
        sum = sum / ((double)getHeight()*getWidth());

    //    qDebug() << " Covariance  "<< sum << s_x<<s_y<<s_x*s_y<<" mean" << mu_x << mu_y;

        return (sum + c3())/(sqrt(s_x*s_y) + c3());

    }

    double calcContrast(SSIM *other)
    {
    /*    if (other->m_width != m_width)
            return -1;
        if (other->m_height != m_height)
            return -1;
    */
        double mu_x, s_x;
        double mu_y, s_y;
        calcMeanSigma(mu_x, s_x);
        other->calcMeanSigma(mu_y, s_y);

        return (2*s_x*s_y + c2())/(s_x*s_x + s_y*s_y + c2());

    }

    double c1()
    {
        double k1 = 0.01;
        return (k1*getL())*(k1*getL());
    }

    double c2()
    {
        double k2 = 0.03;
        return (k2*getL())*(k2*getL());

    }

    double c3()
    {
        return c2()/2.0;
    }

};

#endif // SSIM_H
