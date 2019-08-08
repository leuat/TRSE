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
    int f1,f2,f3;
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

    QString ExportAmigaVerts(QString vertices, float scale, QVector3D shift);
    QString ExportAmigaVertexNormals(QString filename, float scale);
    QString ExportAmigaFaceNormals(QString faces, float scale);
    QString ExportAmigaNormalsLines(QString filename, float scale);
    QString ExportAmigaLinesFromFaces(QString faces, float minLength);
    QString ExportAmigaFaces(QString faces);


    QString ExportLua(QString vertices, QString name, float scale, QVector3D shift);

};

#endif // OBJLOADER_H
