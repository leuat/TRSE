/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "cinifile.h"


void CIniFile::Load(QString fname) {

    filename = fname;
    if (!QFile::exists(fname)) {
        qDebug() << "Could not find file " << fname;
        return;
    }
//    qDebug() << fname << "exists";

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
            it.strval = tok[1].trimmed();
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

//            qDebug() << "Loading :  " << it.name;

            if (tok[1].split(",").count()!=1) {
                it.strval = "";
                it.dval = -1;
                it.lst = tok[1].split(",");
                it.lst.removeFirst();
                for (QString& s : it.lst) {
                    s = s.trimmed();
  //                  qDebug() <<s;
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
//        qDebug() << i.name <<i.strval;
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
