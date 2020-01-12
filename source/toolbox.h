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

#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "source/toolboxitem.h"
#include <QVector>
#include <QLayout>
#include <QPushButton>
#include "source/LeLib/util/util.h"

class Toolbox
{
public:
    Toolbox();

    QVector<ToolboxItem*> m_items;
    QVector<QPushButton*> m_buttons;
    int m_windowWidth;
    QGridLayout *m_ly;
    void Initialize(QGridLayout* layout, int windowWidth);

    void BuildGUI(QGridLayout* ly, int windowWidth);
    ToolboxItem* m_current;

    void handleButton(int data);


};

#endif // TOOLBOX_H
