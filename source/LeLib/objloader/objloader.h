#ifndef OBJLOADER_H
#define OBJLOADER_H

#include <QString>
#include <QStringList>
#include <QFile>
#include <QVector>
#include <QVector3D>
#include <QDebug>
#include "source/LeLib/util/util.h"
#include <QtEndian>
class Face {
public:
    int v1, v2, v3;
    int fn;
};


class ObjLoader
{
public:

    QString m_filename;
    QStringList m_data;
    QVector<Face> m_faces;
    QVector<QVector3D> m_vertices, m_normals, m_reducedNormals;



    ObjLoader();
    ObjLoader(QString f) {
        Load(f);
    }

    void Load(QString fn);

    void Parse();

    void ExportAmigaVerts(QString vertices, float scale, QVector3D shift);
    void ExportAmigaNormalsLines(QString filename, float scale);
    void ExportAmigaLinesFromFaces(QString faces);
    void ExportAmigaFaces(QString faces);

};

#endif // OBJLOADER_H
