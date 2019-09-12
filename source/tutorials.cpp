#include "tutorials.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
Tutorials::Tutorials()
{


}

void Tutorials::Read()
{
    QFile inputFile(":resources/text/tutorials.txt");
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QStringList data = in.readLine().split(";");
          if (data.count()==4)
              m_tutorials.append(Tutorial(data[0].trimmed(),data[1].trimmed(),data[2].trimmed(),data[3]));
       }
       inputFile.close();
    }
}

void Tutorials::PopulateTreeList(QTreeWidget *tree)
{
/*     QTreeWidgetItem *treeItem = new QTreeWidgetItem(tree);
     treeItem->setText(0, "Tutorials");
      treeItem->setText(1, "All tutorials");
      addTreeChild(treeItem, "C64", "Commodore 64");
      addTreeChild(treeItem, "C64", "Child_second");*/
    addTreeRoots(tree);
    int idx=0;
    QTreeWidgetItem* first = nullptr;
    for (Tutorial& t: m_tutorials) {

         QTreeWidgetItem *treeItem = new QTreeWidgetItem();
         if (first==nullptr)
             first =treeItem;
         treeItem->setText(0,t.m_name);
         treeItem->setData(0,Qt::UserRole,t.m_file + ";"+t.m_text);

         m_roots[t.m_system]->addChild(treeItem);
     }

    tree->expandAll();
    tree->setCurrentItem(first);
}

void Tutorials::addTreeChild(QTreeWidgetItem *parent, QString name, QString description)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    QTreeWidgetItem *treeItem = new QTreeWidgetItem();

    // QTreeWidgetItem::setText(int column, const QString & text)
    treeItem->setText(0, name);
    treeItem->setText(1, description);

    // QTreeWidgetItem::addChild(QTreeWidgetItem * child)
    parent->addChild(treeItem);
}

void Tutorials::addTreeRoots(QTreeWidget* tree)
{
    // QTreeWidgetItem(QTreeWidget * parent, int type = Type)
    int idx=0;
    for (Tutorial& t: m_tutorials) {
        if (!m_roots.contains(t.m_system)) {
            QTreeWidgetItem *treeItem = new QTreeWidgetItem(tree);

            // QTreeWidgetItem::setText(int column, const QString & text)
            treeItem->setText(0, t.m_system);
            treeItem->setText(1, "");
            treeItem->setData(0,Qt::UserRole,"");

            m_roots[t.m_system] = treeItem;
        }
    }
}

