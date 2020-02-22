#include "tutorials.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Tutorials::Tutorials()
{


}

void Tutorials::Read(QString file)
{
    QFile inputFile(file);
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
           QString d = in.readLine();
           if (d.trimmed().simplified().startsWith("#"))
               continue;
          QStringList data = d.split(";");
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
         treeItem->setForeground(0,QColor(190,220,240,255));

         m_roots[t.m_system]->addChild(treeItem);
     }

    tree->expandAll();
    tree->setCurrentItem(first);
}

void Tutorials::PopulateTemplateList(QListWidget *w, QString system)
{
    w->clear();
    if (m_tutorials.count()==0)
        return;
    int row = 0;
    int col = 0;
    int maxCol = 3;
    QListWidgetItem* cur = nullptr;
//    qDebug() << "Tutorials " << m_tutorials.count();
 //   w->setRowCount(m_tutorials.count()/3+1);
 //   w->setColumnCount(maxCol);
    int idx = 0;
    for (Tutorial& t: m_tutorials) {
        if (t.m_system.toLower()!=system.toLower()) {
            idx++;
            continue;
        }

        QListWidgetItem* it = new QListWidgetItem(t.m_name);
        if (cur == nullptr) cur = it;
        QString iconFile = Util::GetSystemPrefix() + "project_templates/"+t.m_file + "/project.png";
//        qDebug() << "TEMPLATES " <<iconFile << QFile::exists(iconFile);
        QPixmap icon = QPixmap(iconFile);
        it->setIcon(icon);

        it->setData(Qt::UserRole,idx);


        w->addItem(it);
//        w->setItem(row,col,it);
        col++;
        if (col==maxCol) {
            col = 0;
            row++;
        }
        idx++;

    }
    int ww = 160;
    int h = ww/1.5;
    w->setIconSize(QSize(ww,h));
    w->setCurrentItem(cur);
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
            QPixmap img(":resources/images/" +t.m_system.toUpper()+".png");

            treeItem->setIcon(0,img);
            treeItem->setData(0,Qt::UserRole,"");
            m_roots[t.m_system] = treeItem;
        }
    }
    tree->setIconSize(QSize(64,48));


}

