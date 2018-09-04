#include "toolbox.h"


Toolbox::Toolbox()
{

}


void Toolbox::Initialize(QGridLayout* ly)
{
    m_items.clear();
    m_items.append(new ShapeBox("",":/resources/images/brush_circle.png"));
    m_items.append(new Spray("",":/resources/images/brush_spray.png"));
    m_items.append(new Dither("",":/resources/images/brush_dither.png"));
    m_items.append(new Filler("",":/resources/images/brush_fill.png"));
    m_items.append(new Line("",":/resources/images/brush_line.png"));
    m_items.append(new CopyStamp("CS",""));
    m_items.append(new RotateAround("Rotate",""));


    m_current = m_items[0];

    BuildGUI(ly);

}

void Toolbox::BuildGUI(QGridLayout *ly)
{
    int size = 50;
    int row=0;
    int col=0;
    for (int i=0;i<m_items.count();i++) {
        QPushButton *b = new QPushButton();
        //b->setGeometry(0,0,40,40);
/*        QPalette p;
        p.setColor(QPalette::Button, m_list[j].color);
        p.setColor(QPalette::Window, m_list[j].color);*/
//        b->setStyleSheet("background-color: rgb("+txtCol + "); color: rgb(0, 0, 0)");
  //      b->setPalette(p);
        b->setMaximumWidth(40);
        b->setMinimumWidth(40);
        b->setAutoFillBackground( true );
        b->setText(m_items[i]->m_name);
        b->setFixedSize(QSize(size, size));
        QObject::connect( b, &QPushButton::clicked,  [=](){ handleButton(i); } );

        ly->addWidget(b,row,col);

        b->setIcon(m_items[i]->m_icon);
        b->setIconSize(QSize(b->width(), b->height()*2));

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
//    qDebug() << "Setting toolbox "<< data;
}
