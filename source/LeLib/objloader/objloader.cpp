#include "objloader.h"

ObjLoader::ObjLoader()
{

}

void ObjLoader::Load(QString fn) {
    m_filename = fn;
    QFile f(fn);
    f.open(QFile::ReadOnly| QFile::Text);
    QString s = f.readAll();

    m_data = s.split("\n");
    f.close();
}

void ObjLoader::Parse()
{
    m_vertices.clear();
    m_faces.clear();
    for (QString l: m_data) {
        QStringList lst = l.trimmed().simplified().toLower().split(" ");
        if (lst.count()==0)
            continue;
        if (lst[0]=="v")
            m_vertices.append(QVector3D(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat()));
        if (lst[0]=="f") {
            QVector<int> ilst;
            for (int i=1;i<lst.count();i++)
                ilst.append(lst[i].split("/")[0].toInt());

            if (ilst.count()==3) {
                Face f;
                f.v1 = ilst[0]-1;
                f.v2 = ilst[1]-1;
                f.v3 = ilst[2]-1;
                m_faces.append(f);
            }
            if (ilst.count()==4) {
                Face f;
                f.v1 = ilst[0]-1;
                f.v2 = ilst[1]-1;
                f.v3 = ilst[2]-1;
                m_faces.append(f);
                Face f2;
                f2.v1 = ilst[0]-1;
                f2.v2 = ilst[2]-1;
                f2.v3 = ilst[3]-1;
                m_faces.append(f2);
            }
//            qDebug() << f.v1 << f.v2 << f.v3;
            //qDebug() << "Appending";
//            m_faces.append(QVector3D(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat()));
        }
    }
}
