#ifndef TUTORIALS_H
#define TUTORIALS_H

#include <QString>
#include <QStringList>
#include <QVector>
#include <QTreeView>
#include <QTreeWidget>
#include <QTreeWidgetItem>



class Tutorial {
public:
    QString m_system;
    QString m_name;
    QString m_file;
    QString m_text;
    Tutorial(QString system, QString name, QString file, QString text) {
        m_system = system;
        m_text = text;
        m_file = file;
        m_name = name;
    }
};


class Tutorials
{
public:
    Tutorials();
    QVector<Tutorial> m_tutorials;
    QMap<QString, QTreeWidgetItem*> m_roots;
    void Read();
    void PopulateTreeList(QTreeWidget* tree);

    void addTreeChild(QTreeWidgetItem *parent,
                      QString name, QString description);


    void addTreeRoots(QTreeWidget* tree);
};

#endif // TUTORIALS_H
