#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QVector>
#include <QVector3D>
#include <QDebug>

class Face {
public:
    int v1, v2, v3;
};


class ObjLoader
{
public:

    QString m_filename;
    QStringList m_data;
    QVector<Face> m_faces;
    QVector<QVector3D> m_vertices;



    ObjLoader();
    ObjLoader(QString f) {
        Load(f);
    }

    void Load(QString fn);

    void Parse();

};

#endif // OBJLOADER_H
