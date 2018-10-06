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

#ifndef LIMAGEEFFECTS_H
#define LIMAGEEFFECTS_H


#include "source/LeLib/limage/limage.h"
#include "source/LeLib/limage/multicolorimage.h"
#include <QString>
class LImageEffect {
  public:
    QString m_name;
    QMap<QString, float> m_params;

    LImageEffect(QString name) {
        m_name=name;
    }

};


class LImageEffects
{
public:
    LImageEffects();
    LImage* m_image;
    QVector<LImageEffect*> m_effects;

    QStringList getStringList();


    void Init(LImage* image) { m_image=image;}

    void InitEffects();

    void RenderCircles(LImageEffect* le);

    void Render(QString effect);

};

#endif // LIMAGEEFFECTS_H
