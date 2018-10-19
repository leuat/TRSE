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

#include "util.h"
#include <QDebug>
//#include <QtGlobal>

QString Util::path = "";
QElapsedTimer Util::globalTimer;
bool Util::CancelSignal = false;


void Util::Tokenize(const string& str,
                      vector<string>& tokens,
                      const string& delimiters)
{
  // Skip delimiters at beginning.
  string s = str;
  int wn= s.find(13);
  if (wn!=-1) s.erase(wn,1);
  
  string::size_type lastPos = s.find_first_not_of(delimiters, 0);
  // Find first "non-delimiter".
  string::size_type pos     = s.find_first_of(delimiters, lastPos);
  
  while (string::npos != pos || string::npos != lastPos)
    {
      // Found a token, add it to the vector.
      tokens.push_back(s.substr(lastPos, pos - lastPos));
      // Skip delimiters.  Note the "not_of"
      lastPos = s.find_first_not_of(delimiters, pos);
      // Find next "non-delimiter"
      pos = s.find_first_of(delimiters, lastPos);
    }
  
}	


  
const char* Util::read_textfile(string filename) {
  ifstream f(filename.c_str(), ios::in);
  string cnt, sum;
  sum = "";
  while(!f.eof()) {
    f >> cnt; 
    sum = sum + cnt;
  }
  f.close();       
  return sum.c_str();           
}


void Util::verify_file(string filename) {
  ifstream f(filename.c_str(), ios::in | ios::binary);
  if (!f.is_open())
    throw string("Unable to find file: " + filename);
  f.close();
}
bool Util::verify_file_bool(string filename) {
  ifstream f(filename.c_str(), ios::in | ios::binary);
  if (!f.is_open())
    return false;
  f.close();
  return true;
}

QString Util::BinopString(QString a) {
    QString pa ="";
    QString pb = "";
    a=a.simplified().trimmed().toLower();
    if (a.contains("(")) {
        a = a.replace("(","").replace(")","");
        pa = "(";
        pb= ")";
    }

    if (!(a.contains("+") || a.contains("-")))
        return pa+a+pb;


    QString b = a;
    QStringList lst = b.replace("+","|").replace("-","|").split("|");

    QString q=a;
    QString str = "0123456789$#%abcdef ";
    for (int i=0;i<str.length();i++)
        q=q.replace(str[i],"");

//    qDebug() << q;
//    qDebug() << lst;
//    exit(1);
    int val = 0;
    bool ok=Util::NumberFromStringHex(lst[0],val);
    if (!ok)
        return pa+a+pb;

    for (int i=0;i<q.length();i++) {
        int v;
        ok = Util::NumberFromStringHex(lst[i+1],v);
        if (!ok)
            return pa+a+pb;
        if (q[i]=="+")
            val+=v;
        if (q[i]=="-")
            val-=v;
    }

    return pa + Util::numToHex(val) + pb;

    /*if (a.contains("+")) {
        QStringList l = a.split("+");
        int n1, n2;
        bool ok1=NumberFromStringHex(l[0], n1);
        bool ok2=NumberFromStringHex(l[1], n2);

        if (ok1&&ok2)
            return pa + pa+numToHex(n1+n2) + pb;

    }
    if (a.contains("-")) {
        QStringList l = a.split("-");
        int n1, n2;
        bool ok1=NumberFromStringHex(l[0], n1);
        bool ok2=NumberFromStringHex(l[1], n2);

        if (ok1&&ok2)
            return pa + pa+numToHex(n1-n2) + pb;

    }
    */
    return pa+a+pb;

}
QString Util::ReplaceWords(QString line, QString word) {

    return "";
}



string Util::trim(string strin)
{
  string str = strin;
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
  return str;
}

int Util::VerifyHexAddress(QString s)
{
    int val;
    bool ok;
    if (s.startsWith("$")) {
        val=s.replace("$","").toInt(&ok, 16);
    }
    else
    if (s.startsWith("0x")) {
            val=s.replace("0x","").toInt(&ok, 16);
        }
    else val=s.toInt(&ok, 10);
    if (!ok)
        return -1;
    return val;

}

QString Util::numToHex(int v)
{
    return "$" + QString::number(v,16);
}

QColor Util::colorScale(QColor &col, int mean, int std)
{
    float f = 1.0/(2*std)*255;
    QVector3D c;



    c.setX((col.red()-mean)*f + mean);
    c.setY((col.green()-mean)*f + mean);
    c.setZ((col.blue()-mean)*f + mean);
    c = clamp(c,0,255);
    return QColor(c.x(), c.y(), c.z());

}

QString Util::findFileInDirectory(QString search, QString dir, QString extension)
{
    QDirIterator it(dir, QStringList() << "*." + extension, QDir::Files);
    while (it.hasNext()) {
        QString f = it.next();
        if (f.contains(search))
            return f;
    }
    return "";

}

QString Util::listFiles(QDir directory, QString searchFile)
{
        QDir dir(directory);
        QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot);
        foreach(QFileInfo finfo, list) {
                if (finfo.isDir()) {
                        QString s = listFiles(QDir(finfo.absoluteFilePath()), searchFile);
                        if (s!="")
                            return s;
                }
                if (finfo.fileName().toLower()==searchFile.toLower())
                    return finfo.absoluteFilePath();

        }
        return "";
}

QString Util::findFileInSubDirectories(QString search, QString dir, QString extension)
{
    QDir directory(dir);
    return listFiles(directory, search);

}

float Util::clamp(float val, const float mi, const float ma) {
    val = min(ma, val);
    val = max(mi, val);
    return val;
}

QColor Util::Gamma(QColor c, float xexp, float shift)
{
    float x = c.red();
    float y = c.green();
    float z = c.blue();
    x = pow(x - shift, xexp);
    y = pow(y - shift, xexp);
    z = pow(z - shift, xexp);
    x = clamp(x,0,255);
    y = clamp(y,0,255);
    z = clamp(z,0,255);
    return QColor(x,y,z);
}

QVector3D Util::clamp(const QVector3D val, const float min, const float max)
{
    QVector3D v = val;
    v.setX(clamp(val.x(),min,max));
    v.setY(clamp(val.y(),min,max));
    v.setZ(clamp(val.z(),min,max));
    return v;
}

QString Util::getFileName(QString dir, QString baseName, QString type)
{
    QDirIterator it(dir,
                    QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
    int maxNumber = 1;
    while (it.hasNext()) {
        QString filename = it.next();
        QStringList split = filename.split("/");
        filename = split[split.length()-1];
        if (filename.contains(baseName)) {
            filename = filename.replace(baseName, "");
            filename = filename.replace("."+type, "");
            int num = filename.toInt();
            maxNumber = max(maxNumber, num);
        }
    }
    maxNumber++;
    return baseName + QString::number(maxNumber).rightJustified(4, '0');// + "." + type;

}



float Util::smoothstep(float edge0, float edge1, float x)
{
    // Scale, bias and saturate x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}

