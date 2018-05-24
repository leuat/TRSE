#include "cinifile.h"


void CIniFile::Load(QString fname) {

    filename = fname;
    if (!QFile::exists(fname)) {
        qDebug() << "Could not find file " << fname;
        return;
    }
    qDebug() << fname << "exists";

    QFile file(fname);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream f(&file);

    while(!f.atEnd()) {

        QString line = f.readLine();



        QStringList tok = line.split("=");
        if (tok.size()==2) {
            CItem it;
            it.dval = -1;
            it.name = tok[0].toLower().trimmed();
            it.strval = tok[1].toLower().trimmed();
            bool ok;
            it.dval  = tok[1].toFloat(&ok);
            if (ok) {
                it.strval="";
            }
            else it.dval = 0;

            if (tok[1].split(":").count()==3) {
                QStringList v = tok[1].split(":");
                it.vec.setX( v[0].toFloat());
                it.vec.setY( v[1].toFloat());
                it.vec.setZ( v[2].toFloat());
            }


            if (tok[1].split(",").count()!=1) {
                it.strval = "";
                it.dval = -1;
                it.lst = tok[1].split(",");
                it.lst.removeFirst();
                for (QString& s : it.lst) {
                    s = s.trimmed();
                 //   qDebug() << s;
                }
            }
            items.push_back(it);
        }
    }
    file.close();
}

void CIniFile::Save(QString fname)
{
    if (QFile::exists(fname)) {
        QFile::remove(fname);
    }

    QFile file(fname);
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream f(&file);
    for (CItem i: items) {
        //  qDebug() << i.name << " " << QString::number(i.dval) << ", " << i.strval << "\n";
        f << i.name << " = ";
        if (i.strval!="")
           f << i.strval << "\n";
        else
          if (i.lst.count()!=0) {
              f<<",";
              for (int j=0;j<i.lst.count();j++) {
                  f<<i.lst[j];
                 // qDebug() << " << " << i.lst[j];
                  if (j!=i.lst.count()-1)
                      f<<", ";
              }
            f<<"\n";
          }
          else
              if (i.vec.length()!=0) {
                  f<<QString::number(i.vec.x()) << ":";
                  f<<QString::number(i.vec.y()) << ":";
                  f<<QString::number(i.vec.z()) << "\n";
              }

              else {
            f << QString::number(i.dval) << "\n";
          //  qDebug() << QString::number(i.dval) << "\n";
              }

    }
    file.close();

}
