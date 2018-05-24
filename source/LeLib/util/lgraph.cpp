#include "lgraph.h"
#include <fstream>
#include <string>
#include <iostream>
#include <qdebug.h>
#include <QFile>
#include <QTextStream>

void LGraph::RenderGauss(float mean, float std)
{
    for (int i=0;i<m_noBins;i++) {
        float v = m_index[i];
        m_value[i] = exp(-(pow(mean-v,2)/(2*std*std)));
    }
}

void LGraph::normalizeArea()
{
    double integralSum = 0;

    for(int i=0; i<m_value.size()-1; i++) {
        //QPointF &p1 = m_points[i];
        //QPointF &p2 = m_points[i+1];
        double dx = m_delta;
        double dy = m_value[i+1] + m_value[i];
        integralSum += dx*dy;
    }
    integralSum *= 0.5;
    for(int i=0; i<m_value.size(); i++) {
        m_value[i] /= integralSum;
    }

}




void LGraph::getMinMaxY()
{
    m_miny = 1E30;
    m_maxy = -1E30;
    for (int i=0;i<m_noBins;i++) {
        m_miny = min(m_value[i], m_miny);
        m_maxy = max(m_value[i], m_maxy);
    }
}

void LGraph::CheatStdY(float threshold)
{
    getMinMaxY();
    for (int i=0;i<m_noBins;i++) {
        m_stdY = abs(m_meanY - m_index[i]);
        //qDebug() << "max x, val: " <<m_maxy << ",  " << m_value[i];

        if (m_value[i]>m_maxy*threshold)
            return;

    }
}

void LGraph::Normalize()
{
    getMinMaxY();
    for (int i=0;i<m_noBins;i++) {
        m_value[i] = (m_value[i] - m_miny)/(m_maxy - m_miny);
    }

}


/*void LGraph::fitSpline(LGraph& fit, int N) {


    vector<double> x,y;


    LGraph sm;
    sm.Copy(*this);
    //sm.Smooth(50);

    for (int i=0;i<=N;i++) {
        int b = i*m_noBins/((double)N);
        if (b==m_noBins) b--;
        x.push_back(i/(double)N);
        y.push_back(sm.getAveragem_valueueAt(b, 250));
        //cout << Index[b] << " " <<m_value[b] << endl;
    }

    Spline<double, double> sp(x, y);

    fit.initialize(N);
    for (int i=0;i<N;i++)  {
        fit.m_value[i] = sp[(double)i/(double)m_noBins];
        fit.index[i] = x[i];
    }

}
*/
void LGraph::Mean() {
    m_mean = 0;
    for (int i=0;i<m_noBins;i++) {
        m_mean+=m_value[i];
    }
    m_mean/=(double)m_noBins;
    m_meanY = 0;
    float current = -1;
    for (int i=0;i<m_noBins;i++) {
        if (m_value[i]>current) {
            current = m_value[i];
            m_meanY = m_index[i];
        }
    }

}

void LGraph::Std() {
    m_std = 0;
    Mean();

    for (int i=0;i<m_noBins;i++) {
        double v= m_value[i] - m_mean;
        m_std += v*v;
    }
    m_std/=(double)m_noBins;
    m_std = sqrt(m_std);

}


/*void LGraph::Normalize() {
    double m = -1E20;
    for (int i=0;i<m_noBins;i++)
        m = max(m_value[i], m);


    for (int i=0;i<m_noBins;i++)
        m_value[i]/=m;

}

*/

void LGraph::LoadText(QString Filename) {

}



void LGraph::SaveText(QString fileName) {
    if (QFile::exists(fileName))
      QFile::remove(fileName);
        QFile file(fileName);
    qDebug() << "Saving graph " << fileName;
    if (file.open(QIODevice::ReadWrite|QFile::Text)) {
            QTextStream stream(&file);

            for (int i=0;i<m_noBins;i++) {
                stream << m_index[i] << "  " << m_value[i] << endl;
//                qDebug() << m_index[i] << " " << m_value[i];
            }
        }
    file.close();
}


/*void LGraph::LikelihoodFromChisq() {
    // find min
    double m = 1E20;
    for (int i=0;i<m_noBins;i++)
        m = min(m_value[i], m);

    for (int i=0;i<m_noBins;i++) {
        m_value[i] = exp(-(m_value[i]-m));
    }

}
*/

/*double LGraph::ChiSQ(LGraph& temp, LGraph& two, double temperature) {
    // temp is data, two is model
    double chisq = 0;
    if (temp.m_noBins!=two.m_noBins)
        return 0;


    for (int i=0;i<temp.m_noBins;i++) {
        float t = two.m_value[i];
        if (t!=0)
            chisq += pow(temperature*(temp.m_value[i] - two.m_value[i]), 2.0)/(temperature*t);
    }
    return chisq;
}
*/


/*void LGraph::CalculateStatistics() {
    double top = -1E12;
    int idx;

    cout << "eh " << m_noBins << endl;
    for (int i=0;i<m_noBins;i++) {
        if (m_value[i]>top) {
            top = m_value[i];
            idx = i;
        }
    }
    cout << "føkk dis" << endl;
    int winner = idx;
    mean = index[idx];

    // double m_valueue = m_value[idx];
    bool ok = true;

    while (ok && m_value[idx]>0.5 && ok) {
        idx--;
        if (idx<0) {
            break;
            ok = false;
        }

    }
    //  cout << idx << endl;
    if (!ok) {
        idx = winner;
        while (m_value[idx]>0.5) {
            idx++;
            if (idx>=m_noBins) {
                cout << "ERROR Likelihood width larger than chisq m_noBins - bad sample" << endl;
            }

        }

    }

    double pos = index[idx];
    m_std = abs(mean - pos);
}
*/
