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

#include "toolbox.h"


Toolbox::Toolbox()
{

}


void Toolbox::Initialize(QGridLayout* ly, int windowWidth)
{
    m_items.clear();
    m_ly = ly;
    m_items.append(new ShapeBox("",":/resources/images/brush_circle.png","Brush"));
    m_items.append(new Smooth("",":/resources/images/brush_blur.png","Blur"));
    m_items.append(new ShapePNGColor("",":/resources/images/brush_shape.png","Shape"));
    m_items.append(new Spray("",":/resources/images/brush_spray.png","Spray"));
    m_items.append(new Dither("",":/resources/images/brush_dither.png","Dither"));
    m_items.append(new WetBrush("",":/resources/images/brush_wet.png","Wet"));
    m_items.append(new Filler("",":/resources/images/brush_fill.png","Fill"));
    m_items.append(new Line("",":/resources/images/brush_line.png","Line"));
    m_items.append(new Circle("",":/resources/images/brush_drawcircle.png","Circle"));
    m_items.append(new Box("",":/resources/images/brush_rect.png","Box"));
    m_items.append(new CopyStamp("",":/resources/images/brush_copy.png","Copy/Paste"));
    m_items.append(new RotateAround("",":/resources/images/brush_rotate.png","Rotate"));
    m_items.append(new ShapeBoxFilter("",":/resources/images/brush_circle_background.png", "Background brush"));


    m_current = m_items[0];

    m_buttonsLayout = new QGridLayout();
    m_optionsLayout = new QGridLayout();
    m_ly->addLayout(m_optionsLayout,0,0);
    m_ly->addLayout(m_buttonsLayout,1,0);


    BuildGUI(m_buttonsLayout, windowWidth);
    BuildToolOptions();
}

void Toolbox::BuildToolOptions()
{
    if (m_optionsLayout==nullptr)
        return;
    Util::clearLayout(m_optionsLayout);
    if (m_current->m_options.keys().count()==0)
        return;
    int row = 2;
    m_optionsLayout->addWidget(new QLabel(m_current->m_tooltip+" options"),0,0);
    QFrame* hFrame = new QFrame;
    hFrame->setFrameShape(QFrame::HLine);
    m_optionsLayout->addWidget(hFrame,1,0);
 /*    m_optionsLayout->addWidget(hFrame,1,0);
    QPushButton* help = new QPushButton("Help");
    m_optionsLayout->addWidget(help,2,0);*/
    m_optionsLayout->setColumnStretch(0,10);
    m_optionsLayout->setColumnStretch(1,1);
    for (QString name: m_current->m_options.keys()) {
        m_current->m_options[name]->Build(m_optionsLayout,row);


        row+=2;
    }
    QFrame* hFrame2 = new QFrame;
    hFrame2->setFrameShape(QFrame::HLine);
    m_optionsLayout->addWidget(hFrame2,row,0);


}

void Toolbox::BuildGUI(QGridLayout *ly, int windowWidth)
{
    int size = 64;
    size = 80*(windowWidth/1920.0);
    int row=0;
    int col=0;
    m_windowWidth = windowWidth;
    Util::clearLayout(ly);
    for (int i=0;i<m_items.count();i++) {
        QPushButton *b = new QPushButton();
        //b->setGeometry(0,0,40,40);
        QPalette p;
  //      p.setColor(QPalette::Button, m_list[j].color);
//        p.setColor(QPalette::Window, m_list[j].color);*/
        if (m_current==m_items[i])
            b->setStyleSheet("background-color: #204080");
  //      b->setPalette(p);
   //     b->setMaximumWidth(20);
   //     b->setMinimumWidth(20);
        b->setAutoFillBackground( true );
        b->setText(m_items[i]->m_name);
        b->setFixedSize(QSize(size, size));
        b->setToolTip(m_items[i]->m_tooltip);
        QObject::connect( b, &QPushButton::clicked,  [=](){ handleButton(i);  } );

        ly->addWidget(b,row,col);

        b->setIcon(m_items[i]->m_icon);
        b->setIconSize(QSize(b->width(), b->height())*0.9);

        m_buttons.append(b);
        col++;
        if (col==2) {
            col=0;
            row++;
        }

    }
}

void Toolbox::handleButton(int data)
{
    m_current = m_items[data];
    m_current->Init();
    BuildGUI(m_buttonsLayout,m_windowWidth);
    BuildToolOptions();
//    qDebug() << "Setting toolbox "<< data;
}
