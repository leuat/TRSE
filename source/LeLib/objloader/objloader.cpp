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
    try {
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

            if (ilst.count()==3) {
                Face f;
                f.v1 = ilst[0]-1;
                f.v2 = ilst[1]-1;
                f.v3 = ilst[2]-1;

                f.f1 = flst[0]-1;
                f.f2 = flst[1]-1;
                f.f3 = flst[2]-1;
                m_faces.append(f);
            }
            if (ilst.count()==4) {
                Face f;
                f.v1 = ilst[0]-1;
                f.v2 = ilst[1]-1;
                f.v3 = ilst[2]-1;
                f.f1 = flst[0]-1;
                f.f2 = flst[1]-1;
                f.f3 = flst[2]-1;
                m_faces.append(f);
                Face f2;
                f2.v1 = ilst[0]-1;
                f2.v2 = ilst[2]-1;
                f2.v3 = ilst[3]-1;
                f2.f1 = flst[0]-1;
                f2.f2 = flst[2]-1;
                f2.f3 = flst[3]-1;
                m_faces.append(f2);
            }
            if (ilst.count()!=3) {
//                qDebug() << "Size : " << QString::number(ilst.count());
            }
//            qDebug() << f.v1 << f.v2 << f.v3;
            //qDebug() << "Appending";
//            m_faces.append(QVector3D(lst[1].toFloat(),lst[2].toFloat(),lst[3].toFloat()));
        }
    }
    } catch (std::exception e) {
        throw QString("Error during parsing : " + QString(e.what()));
    }
}

QString ObjLoader::ExportAmigaLinesFromFaces(QString faces, float minLength)
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

         norms.append(m_normals[m_faces[i].f1]);
         norms.append(m_normals[m_faces[i].f2]);
         norms.append(m_normals[m_faces[i].f3]);

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

        QVector3D pv1 = m_vertices[  qFromBigEndian(v1)/12];
        QVector3D pv2 = m_vertices[ qFromBigEndian(v2)/12];
        if ((pv1-pv2).length()<minLength)
            ok=false;

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
    QFile fi2(faces);
    fi2.open(QFile::WriteOnly);
//    qDebug() << "bf" <<cnt/2;
    cnt = cnt&0b1111111111100;
  //  qDebug() << "after " <<cnt/2;
    m_reducedNormals.resize(cnt/2);
    fi2.write((const char*)cfacs,cnt*sizeof(short));
    fi2.close();

    delete[] cfacs;
    //qDebug() << QString::number(cnt/2);;
    return faces +" no lines : " + QString::number(cnt/2);

}

QString ObjLoader::ExportAmigaFaces(QString faces)
{
    if (QFile::exists(faces))
        QFile::remove(faces);


     unsigned short *facs = new unsigned short[3*m_faces.count()];

    for (int i=0;i<m_faces.count();i++) {
        facs[3*i] = qFromBigEndian((unsigned short)(m_faces[i].v1*12));
        facs[3*i+1] = qFromBigEndian((unsigned short)(m_faces[i].v2*12));
        facs[3*i+2] = qFromBigEndian((unsigned short)(m_faces[i].v3*12));

//        facs[3*i+2] = (int)(m_faces[i].v3*scale);
    }
    QFile f2(faces);
    f2.open(QFile::WriteOnly);
    f2.write((const char*)facs,3*m_faces.count()*sizeof(short));
    f2.close();
    delete[] facs;

    return "no faces : "+ QString::number(m_faces.count());
}

QString ObjLoader::ExportAmigaFaceNormals(QString filename, float scale)
{
    if (QFile::exists(filename))
        QFile::remove(filename);

    char *verts = new char[3*m_faces.count()];
    qDebug() << "Facecount: " << m_faces.count();
    for (int i=0;i<m_faces.count();i++) {
        int j = m_faces[i].f1;
        verts[3*i+0] = (char)(m_normals[j].x()*scale);
        verts[3*i+1] = (char)(m_normals[j].y()*scale);
        verts[3*i+2] = (char)(m_normals[j].z()*scale);
//        qDebug() << QString::number(verts[3*i]);
    }
//    qDebug() << m_reducedNormals;
    QFile f1(filename);
    f1.open(QFile::WriteOnly);
    f1.write((const char*)verts,3*m_faces.count()*sizeof(char));
    f1.close();
    delete[] verts;

    return filename + ": # face normals: " + QString::number(m_faces.count());


}

QString ObjLoader::ExportAmigaVerts(QString vertices, float scale, QVector3D shift)
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
    QFile f1(vertices);
    f1.open(QFile::WriteOnly);
    f1.write((const char*)verts,3*m_vertices.count()*sizeof(int));
    f1.close();
    delete[] verts;


    return vertices + ": # vertices: " +QString::number(m_vertices.count());

}

QString ObjLoader::ExportLua(QString vertices, QString name, float scale, QVector3D shift)
{
    if (QFile::exists(vertices))
        QFile::remove(vertices);

    QFile f1(vertices);
    f1.open(QFile::WriteOnly);
    QTextStream s(&f1);
    s<< name << "_verts = { \n";
    for (int i=0;i<m_vertices.count();i++) {
//        qDebug() << m_vertices[i];
        s<< ((m_vertices[i].x()*scale+shift.x()))<<", ";
        s<< ((m_vertices[i].y()*scale+shift.y()))<<", ";
        QString endd=", \n";
        if (i==m_vertices.count()-1)
            endd = "\n}\n";
        s<< ((m_vertices[i].z()*scale+shift.z())) << endd;
    }

    s<< name << "_normals = { \n";
    for (int i=0;i<m_faces.count();i++) {
//        qDebug() << m_vertices[i]
        int j = m_faces[i].f1;
        s<< ((m_normals[j].x()*64))<<", ";
        s<< ((m_normals[j].y()*64))<<", ";
        QString endd=", \n";
        if (i==m_normals.count()-1)
            endd = "\n}\n";
        s<< ((m_normals[j].z())*64) << endd;
    }

    s<< name << "_faces = { \n";
    for (int i=0;i<m_faces.count();i++) {
//        qDebug() << m_vertices[i];
        s<< ((m_faces[i].v1))<<", ";
        s<< ((m_faces[i].v2))<<", ";
        QString endd=", \n";
        if (i==m_faces.count()-1)
            endd = "\n}\n";
        s<< ((m_faces[i].v3))<< endd;

    }


    f1.close();


    return vertices + ": # vertices: " +QString::number(m_vertices.count());

}


QString ObjLoader::ExportAmigaNormalsLines(QString filename, float scale)
{
    if (QFile::exists(filename))
        QFile::remove(filename);

    char *verts = new char[3*m_reducedNormals.count()];
    for (int i=0;i<m_reducedNormals.count();i++) {
//        qDebug() << m_vertices[i];
/*        verts[3*i+0] = qFromBigEndian((char)(m_reducedNormals[i].x()*scale));
        verts[3*i+1] = qFromBigEndian((char)(m_reducedNormals[i].y()*scale));
        verts[3*i+2] = qFromBigEndian((char)(m_reducedNormals[i].z()*scale));/*/
        verts[3*i+0] = (char)(m_reducedNormals[i].x()*scale);
        verts[3*i+1] = (char)(m_reducedNormals[i].y()*scale);
        verts[3*i+2] = (char)(m_reducedNormals[i].z()*scale);
//        qDebug() << QString::number(verts[3*i]);
    }
//    qDebug() << m_reducedNormals;
    QFile f1(filename);
    f1.open(QFile::WriteOnly);
    f1.write((const char*)verts,3*m_reducedNormals.count()*sizeof(char));
    f1.close();
    delete[] verts;

    return filename + ": # normals: " + QString::number(m_reducedNormals.count());

}
