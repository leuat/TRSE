#ifndef TOOLBOX_H
#define TOOLBOX_H

#include "source/toolboxitem.h"
#include <QVector>
#include <QLayout>
#include <QPushButton>


class Toolbox
{
public:
    Toolbox();

    QVector<ToolboxItem*> m_items;
    QVector<QPushButton*> m_buttons;


    void Initialize(QLayout* layout);

    void BuildGUI(QLayout* ly);
    ToolboxItem* m_current;

    void handleButton(int data);


};

#endif // TOOLBOX_H
