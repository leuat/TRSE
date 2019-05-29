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
        if (lst[0]=="vn")
            m_normals.append(QVector3D(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat()));
        if (lst[0]=="f") {
            QVector<int> ilst;
            QVector<int> flst;
            for (int i=1;i<lst.count();i++) {
                ilst.append(lst[i].split("/")[0].toInt());
                flst.append(lst[i].split("/").last().toInt());
            }
            qDebug() << flst;

            if (ilst.count()==3) {
                Face f;
                f.v1 = ilst[0]-1;
                f.v2 = ilst[1]-1;
                f.v3 = ilst[2]-1;
                f.fn = flst[0]-1;
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

void ObjLoader::ExportAmigaLinesFromFaces(QString faces)
{
    if (QFile::exists(faces))
        QFile::remove(faces);

    QVector<unsigned short> facs,f2;
    QVector<QVector3D> norms;
//    facs.resize(6*m_faces.count());

    for (int i=0;i<m_faces.count();i++) {
        facs.append(qFromBigEndian((unsigned short)(m_faces[i].v1*12)));
        facs.append(qFromBigEndian((unsigned short)(m_faces[i].v2*12)));
        facs.append(qFromBigEndian((unsigned short)(m_faces[i].v2*12)));
        facs.append(qFromBigEndian((unsigned short)(m_faces[i].v3*12)));
        facs.append(qFromBigEndian((unsigned short)(m_faces[i].v3*12)));
        facs.append(qFromBigEndian((unsigned short)(m_faces[i].v1*12)));
        for (int j=0;j<3;j++) {
            norms.append(m_normals[m_faces[i].fn]);
        }

//        facs[3*i+2] = (int)(m_faces[i].v3*scale);
    }
    int cnt = 0;
    m_reducedNormals.clear();
    for (int i=0;i<facs.size()/2;i++) {
        unsigned short v1 = facs[i*2];
        unsigned short v2 = facs[i*2+1];
        bool ok= true;
        for (int j=0;j<f2.count()/2;j++) {
            if ((v1==f2[2*j] && v2==f2[2*j+1]) || (v2==f2[2*j] && v1==f2[2*j+1]))
                ok = false;
        }
        if (ok) {
            f2.append(v1);
            f2.append(v2);
            m_reducedNormals.append(norms[i]);

        }
    }
    cnt = f2.count();
    unsigned short *cfacs = new unsigned short[cnt];
    for (int i=0;i<cnt;i++)
        cfacs[i]=f2[i];
    qDebug() << faces <<" no lines : " << cnt/2;
    QFile fi2(faces);
    fi2.open(QFile::WriteOnly);
    fi2.write((const char*)cfacs,cnt*sizeof(short));
    fi2.close();
    delete[] cfacs;

}

void ObjLoader::ExportAmigaFaces(QString faces)
{
    if (QFile::exists(faces))
        QFile::remove(faces);

    unsigned short *facs = new unsigned short[3*m_faces.count()];
    for (int i=0;i<m_faces.count();i++) {
        facs[3*i] = qFromBigEndian((unsigned short)(m_faces[i].v1*4*3));
        facs[3*i+1] = qFromBigEndian((unsigned short)(m_faces[i].v2*4*3));
        facs[3*i+2] = qFromBigEndian((unsigned short)(m_faces[i].v3*4*3));

//        facs[3*i+2] = (int)(m_faces[i].v3*scale);
    }
    qDebug() << "no lines : " << 3*m_faces.count();
    QFile f2(faces);
    f2.open(QFile::WriteOnly);
    f2.write((const char*)facs,3*m_faces.count()*sizeof(short));
    f2.close();
    delete[] facs;

}

void ObjLoader::ExportAmigaVerts(QString vertices, float scale, QVector3D shift)
{
    if (QFile::exists(vertices))
        QFile::remove(vertices);

    int *verts = new int[3*m_vertices.count()];
    for (int i=0;i<m_vertices.count();i++) {
//        qDebug() << m_vertices[i];
        verts[3*i+0] = qFromBigEndian((int)(m_vertices[i].x()*scale+shift.x()));
        verts[3*i+1] = qFromBigEndian((int)(m_vertices[i].y()*scale+shift.y()));
        verts[3*i+2] = qFromBigEndian((int)(m_vertices[i].z()*scale+shift.z()));
    }
    qDebug() << vertices << ": # vertices: " << QString::number(m_vertices.count());
    QFile f1(vertices);
    f1.open(QFile::WriteOnly);
    f1.write((const char*)verts,3*m_vertices.count()*sizeof(int));
    f1.close();
    delete[] verts;



}

void ObjLoader::ExportAmigaNormalsLines(QString filename, float scale)
{
    if (QFile::exists(filename))
        QFile::remove(filename);

    int *verts = new int[3*m_reducedNormals.count()];
    for (int i=0;i<m_reducedNormals.count();i++) {
//        qDebug() << m_vertices[i];
        verts[3*i+0] = qFromBigEndian((int)(m_reducedNormals[i].x()*scale));
        verts[3*i+1] = qFromBigEndian((int)(m_reducedNormals[i].y()*scale));
        verts[3*i+2] = qFromBigEndian((int)(m_reducedNormals[i].z()*scale));
    }
    qDebug() << filename << ": # normals: " << QString::number(m_reducedNormals.count());
//    qDebug() << m_reducedNormals;
    QFile f1(filename);
    f1.open(QFile::WriteOnly);
    f1.write((const char*)verts,3*m_reducedNormals.count()*sizeof(int));
    f1.close();
    delete[] verts;


}
