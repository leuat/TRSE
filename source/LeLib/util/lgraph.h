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

#pragma once
#include "spline.h"

using namespace std;

#include <QVector>
#include <QPointF>
#include "util.h"
#include <QString>



class LGraph {
public:
    QVector<float> m_index;
    QVector<float> m_value;

    float m_minx, m_maxx;
    float m_miny, m_maxy;
    float m_delta;
    double m_mean, m_std;
    double m_avg = 0;


    float m_meanY, m_stdY;

    QString m_name;

    int m_noBins;

    LGraph() {
    }


    void RenderGauss(float mean, float std);


    void normalizeArea();

    void getMinMaxY();

    void CheatStdY(float threshold);




    int getIndex(float xval) {
        float s = (xval - m_minx)/(m_maxx - m_minx);
        s*=m_noBins;
        if (s>=0 && s<m_noBins)
            return s;
        return -1;
    }

    float get(float xval) {
        int idx = getIndex(xval);
        if (idx!=-1)
            return m_value[idx];
        return -1;
    }
    void set(float xval, float value) {
        int idx = getIndex(xval);
        if (idx!=-1)
            m_value[idx] = value;
    }
    void add(float xval, float val) {
        int idx = getIndex(xval);
        if (idx!=-1)
            m_value[idx] += val;
    }

    void Initialize(int bins, float xmin, float xmax) {
        m_noBins = bins;
        m_minx = xmin;
        m_maxx = xmax;
        m_delta = (xmax - xmin)/bins;
        m_value.resize(bins);
        m_index.resize(bins);
        for (int i=0;i<bins;i++) {
            m_value[i] = 0;
            m_index[i] = xmin + m_delta*i;
        }

    }

    float ChiSq(LGraph& o) {
        if (o.m_noBins!=m_noBins) {
            qDebug() << "Bins not equal in chisq!";
            return 0;
        }

        double chisq = 0;
        for (int i=0;i<m_noBins;i++) {
            float diff = m_value[i] - o.m_value[i];
            if (diff!=0)
                chisq += pow(diff,2.0);
          //  qDebug() << m_value[i] << " vs " << o.m_value[i] << " : " << chisq;
        }
        return chisq;
    }

    int FitGaussStd(float mean, float minstd, float maxstd, float steps) {

        LGraph gauss;
        gauss.CopyFrom(*this);
        float cur = minstd;
        float delta = (maxstd-minstd)/steps;

        float winner = 0;
        float winnerChi = 1E30;
        qDebug() << "Steps: " << steps;
        for (int i=0;i<steps;i++) {
            gauss.RenderGauss(mean, cur);
            float chi = ChiSq(gauss);
            if (chi<winnerChi) {
                //qDebug() << "NEW WINNER: " << chi << " with prev " << winnerChi;

                winnerChi = chi;
                winner = cur;
            }
            cur+=delta;
        }

        return winner;
    }


    void CopyFrom(LGraph& o) {
        Initialize(o.m_noBins, o.m_minx, o.m_maxx);
    }

//    void CalculateStatistics();

    void Normalize();


    void LoadText(QString Filename);
    void SaveText(QString Filename);

    void Mean();
    void Std();


};
