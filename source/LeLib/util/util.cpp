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
int Util::hexType = 0;
//int Util::m_currentForceConversionType = 10;



QString Util::toString(QStringList lst) {
    QString ret="";
    for (QString& s:lst)
        ret+=s+",";
    ret.remove(ret.count()-1,1);
    return ret;
}

unsigned long Util::Endian_DWord_Conversion(unsigned long dword)
{
    return ((dword>>24)&0x000000FF) | ((dword>>8)&0x0000FF00) | ((dword<<8)&0x00FF0000) | ((dword<<24)&0xFF000000);
}

unsigned long Util::Endian_Word_Conversion(unsigned short dword)
{
    return ((dword>>24)&0x000000FF) | ((dword>>8)&0x0000FF00) | ((dword<<8)&0x00FF0000) | ((dword<<24)&0xFF000000);
}



QString Util::BinopString(QString a) {
    QString pa ="";
    QString pb = "";
    QString str = "0123456789$#%abcdefABCDEF <>";
    a=a.simplified().trimmed();

    if (a.contains("'")) {
      for (QChar c:str)
          a = a.replace("'"+QString(c)+"'",Util::numToHex(c.toLatin1()));
    }


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
    for (int i=0;i<str.length();i++)
        q=q.replace(str[i],"");

//    qDebug() << q;
//    qDebug() << lst;
//    exit(1);
    long val = 0;
    bool ok=Util::NumberFromStringHex(lst[0],val);
    if (!ok)
        return pa+a+pb;

    for (int i=0;i<q.length();i++) {
        long v;
//        ok = false;
        ok = Util::NumberFromStringHex(lst[i+1],v);
        if (!ok)
            return pa+a+pb;
        if (q[i]=='+')
            val+=v;
        if (q[i]=='-')
            val-=v;
    }
//    qDebug() << "BinopString  DONE ";

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

bool Util::NumberFromStringHex(QString s, long &num) {
    bool ok = true;
    s=s.trimmed();

    long val = 0;

    int type = 0;
    s = s.remove("#");
    if (s.startsWith("<")) {
        type = 1;
        s = s.replace("<","");
    }
    if (s.startsWith(">")) {
        type = 2;
        s = s.replace(">","");
    }
    if (s.startsWith("$"))
        val = s.remove("$").toLong(&ok, 16);
    else
        if (s.toLower().startsWith("0x"))
            val = s.remove("0x").toLong(&ok, 16);
        else
            if (s.toLower().startsWith("%"))
                val= s.remove("%").toLong(&ok, 2);
            else
                val = s.toInt(&ok, 10);

    if (type==1)  val = (val)&0xFF;
    if (type==2)  val = (val>>8)&0xFF;
    num = val;
    return ok;
}

uchar Util::flipBit(uchar val, int bit)
{
    uchar v = 1<<bit;
    if ((val & v) == v)
        val = val ^v;
    else
        val = val | v;


    return val;
}

QString Util::findFileInDirectories(QString fileName, QStringList dirs)
{
    for (QString d : dirs) {
        QString s = d + QDir::separator() + fileName;
        if (QFile::exists(s))
            return s;
    }
    return "";
}



QVector3D Util::fromSpherical(float r, float t, float p) {
    return QVector3D( r*sin(t)*cos(p), r*sin(t)*sin(p), r*cos(t)  );
}

QVector3D Util::floor(const QVector3D v) {
    return QVector3D( std::max(0.0f, v.x()), std::max(0.0f,v.y()), std::max(0.0f,v.z())  );
}

QVector3D Util::Rotate2D(QVector3D point, QVector3D center, float angle) {
    QVector3D rot;
    point = point - center;
    rot.setX(point.x()*cos(angle)-point.y()*sin(angle));
    rot.setY(point.y()*cos(angle)+point.x()*sin(angle));
    return rot + center;

}

bool Util::IntersectSphere(QVector3D o, QVector3D d, QVector3D r, QVector3D &isp1, QVector3D &isp2, double &t0, double &t1) {

    r.setX(1.0/(r.x()*r.x()));
    r.setY(1.0/(r.y()*r.y()));
    r.setZ(1.0/(r.z()*r.z()));


    QVector3D rD = QVector3D(d.x()*r.x(), d.y()*r.y(), d.z()*r.z());
    QVector3D rO = QVector3D(o.x()*r.x(), o.y()*r.y(), o.z()*r.z());


    double A = QVector3D::dotProduct(d,rD);
    double B = 2.0*(QVector3D::dotProduct(d, rO));
    double C = QVector3D::dotProduct(o, rO) - 1.0;

    double S = (B*B - 4.0f*A*C);

    if (S<=0) {
        isp1 = QVector3D(0,0,0);
        isp2 = QVector3D(0,0,0);
        t0 = 0;
        t1 = 0;
        return false;
    }

    t0 =  (-B - sqrt(S))/(2.0*A);
    t1 =  (-B + sqrt(S))/(2.0*A);

    isp1 = o+d*t0;
    isp2 = o+d*t1;

    return true;
}

void Util::clearLayout(QLayout *layout, bool deleteWidgets)
{
    while (QLayoutItem* item = layout->takeAt(0))
    {
        if (deleteWidgets)
        {
            if (QWidget* widget = item->widget())
                widget->deleteLater();
        }
        if (QLayout* childLayout = item->layout())
            clearLayout(childLayout, deleteWidgets);
        delete item;
    }
}

QColor Util::toColor(QVector3D c) {
    if (c.x()>255) c.setX(255);
    if (c.y()>255) c.setY(255);
    if (c.z()>255) c.setZ(255);
    if (c.x()<0) c.setX(0);
    if (c.y()<0) c.setY(0);
    if (c.z()<0) c.setZ(0);

    return QColor(c.x(),c.y(),c.z());
}

QString Util::GetSystemPrefix()
{
    QString dir = "";
    dir = QApplication::applicationDirPath();
    if (dir.toLower().endsWith("release") || dir.toLower().endsWith("debug"))
        dir = "";
    else dir+=QDir::separator();
#ifdef __linux__
    dir = Util::path;
    dir = QApplication::applicationDirPath()+"/../";
#endif
#ifdef __APPLE__
//    dir = Util::path;
    dir = QApplication::applicationDirPath()+"/../../";
#endif
    return dir;
}
QString Util::ReplaceWords(QString line, QString word) {

    return "";
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

QString Util::numToHex(long v)
{
    QString o = QString::number(v,16);
    if (o.count()==1) o="0"+o;
    return "$" +o;
}

QByteArray Util::toQByteArray(QVector<int> &data)
{
    QByteArray a;
    for (int& i: data)
        a.append((uchar)i);
    return a;
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

int Util::isEqual(QColor a, QColor b) {
    if (a.red()!=b.red())
        return 0;
    if (a.green()!=b.green())
        return 0;
    if (a.blue()!=b.blue())
        return 0;

    return 1;
}

void Util::WriteInt32(QByteArray &ba, int val)
{
    ba.append((val>>24)&0xFF);
    ba.append((val>>16)&0xFF);
    ba.append((val>>8)&0xFF);
    ba.append((val)&0xFF);

}
void Util::WriteInt16(QByteArray &ba, int val)
{
    ba.append((val>>8)&0xFF);
    ba.append((val)&0xFF);

}

QStringList Util::fixStringListSplitWithCommaThatContainsStrings(QStringList lst)
{
    QStringList fixList;
    bool isInString = false;
    QString cur = "";
    for (auto s: lst) {
        bool add = true;
        cur += s;
        if (s.startsWith("\"")) {
            isInString = true;
            add = false;
        }

        if (s.endsWith("\"")) {
            isInString = false;
            add = true;
        }
        if (isInString) cur+=",";
        if (add) {
            fixList << cur;
            cur = "";
            isInString = false;
        }
    }
    return fixList;
}

int Util::getShiftCount(int i) {
    if (i==1) return 0;
    if (i==2) return 1;
    if (i==4) return 2;
    if (i==8) return 3;
    if (i==16) return 4;
    if (i==32) return 5;
    if (i==64) return 6;
    if (i==128) return 7;
    if (i==256) return 8;
    return -1;
}


bool Util::QStringIsSimilar(QString a, QString b, qreal percentage, int n, Qt::CaseSensitivity caseSense)
//Iterates substrings in groups of n chars from a und finds these in b.
//The number of hits is then divided by the length of the shorter string.
//To properly take word beginnings and endings into account
//spaces are being inserted before and after the strings.
{
    if (a.isEmpty()||b.isEmpty()) return false;
    qreal hits=0;
    a=QString(" ").repeated(n-1)+a+QString(" ").repeated(n-1);
    b=QString(" ").repeated(n-1)+b+QString(" ").repeated(n-1);
    QString part;
    for (int i=0;i<a.count()-(n-1);i++)
    {
        part=a.mid(i,n);
        if (b.contains(part,caseSense)) hits++;
    }
    if (a.length()<b.length()) return (percentage < (100*hits/(a.length()-(n-1))));
    else return (percentage < (100*hits/(b.length()-(n-1))));
}

QStringList Util::FindFilesOfType(QString dir, QString type)
{
    QStringList l;
    QDirIterator it(dir, QStringList() << "*."+type, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext())
        l << it.next().toLower().remove(dir.toLower());
    return l;
}

void Util::CopyFile(QString i, QString o) {
    if (QFile::exists(o)) {
        QFile ff(o);
        ff.remove();
    }
    QFile::copy(i,o);
    QFile::setPermissions(i, QFileDevice::ReadOwner|QFileDevice::WriteOwner);
}

void Util::CopyFileBytes(QString i, QString o) {
    if (QFile::exists(o)) {
        QFile ff(o);
        ff.remove();
    }
    QByteArray a = loadBinaryFile(i);
    SaveByteArray(a,o);
}

bool Util::CopyRecursively(QString sourceFolder, QString destFolder)
{
    bool success = false;
    QDir sourceDir(sourceFolder);

    if(!sourceDir.exists())
        return false;

    QDir destDir(destFolder);
    if(!destDir.exists())
        destDir.mkdir(destFolder);

    QStringList files = sourceDir.entryList(QDir::Files);
    for(int i = 0; i< files.count(); i++) {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        success = QFile::copy(srcName, destName);
        if(!success)
            return false;
    }

    files.clear();
    files = sourceDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    for(int i = 0; i< files.count(); i++)
    {
        QString srcName = sourceFolder + QDir::separator() + files[i];
        QString destName = destFolder + QDir::separator() + files[i];
        success = CopyRecursively(srcName, destName);
        if(!success)
            return false;
    }

    return true;
}

void Util::ConvertFileWithLoadAddress(QString input, QString output, int address)
{
    QFile f(input);
    f.open(QFile::ReadOnly);
    QByteArray a = f.readAll();
    f.close();

    if (QFile::exists(output)) {
        QFile f(output);
        f.remove();
    }
    //    qDebug() << a.size() << input;
    //  exit(1);
    a.insert(0,(address>>8)&0xFF);
    a.insert(0,(address)&0xFF);

    QFile o(output);
    o.open(QFile::WriteOnly);
    o.write(a);
    o.close();
}

void Util::ConvertFileWithLoadAddress(QString input, QString output)
{
    QFile f(input);
    f.open(QFile::ReadOnly);
    QByteArray a = f.readAll();
    f.close();

    if (QFile::exists(output)) {
        QFile f(output);
        f.remove();
    }
    a.remove(0,2);

    QFile o(output);
    o.open(QFile::WriteOnly);
    o.write(a);
    o.close();
}

QVector3D Util::abss(QVector3D a)
{
    return QVector3D(abs(a.x()), abs(a.y()), abs(a.z()));
}

QVector3D Util::maxx(QVector3D a, QVector3D b)
{
    return QVector3D(std::max(a.x(),b.x()), std::max(a.y(),b.y()), std::max(a.z(),b.z()));
}

int Util::C64StringToInt(QString f) {
    int val;
    bool ok;
    if (f.contains("$")) {
        val = f.replace("$","0x").toInt(&ok,16);
    }
    else
        val = f.toInt(&ok,10);

    return val;

}

bool Util::SameSide(const QVector3D &p1, const QVector3D &p2, const QVector3D &a, const QVector3D &b) {
    QVector3D cp1 = QVector3D::crossProduct(b-a, p1-a);
    QVector3D cp2 = QVector3D::crossProduct(b-a, p2-a);
    return QVector3D::dotProduct (cp1, cp2) >= 0;
    //        else return false
}

QString Util::fixFolder(QString folderName) {
    if (folderName[folderName.count()-1]=='\\')
        return folderName;
    if (folderName[folderName.count()-1]=='/')
        return folderName;
    return folderName + "/";
}

std::string Util::c2x(int x, int y) {
    std::string s;
    s = char('A' + y);
    s += std::to_string(x+1);
    return s;
}

float Util::minmax(float v, float a, float b)
{
    v = std::max(v,a);
    v = std::min(v,b);
    return v;
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

float Util::floatRandom(const float &min, const float &max) {
    static std::mt19937 generator;
    std::uniform_real_distribution<float> distribution(min, max);
    return distribution(generator);
}

wchar_t *Util::QStringToWchar(QString t) {
    wchar_t* arr = new wchar_t[t.size()+1];
    t.toWCharArray(arr);
    arr[t.size()]=0;
    return arr;
}

void Util::SaveByteArray(QByteArray &data, QString file) {
    QFile f(file);
    f.open(QFile::WriteOnly);
    f.write(data);
    f.close();

}

QString Util::fromStringList(QStringList lst)
{
    QString ret;
    int i=0;
    for (QString s : lst) {
        ret+=s;
        if (i!=lst.count()-1)
            ret+="\n";
        i++;
    }

    return ret;
}


long Util::NumberFromStringHex(QString s) {
    long val = 0;
    bool ok = NumberFromStringHex(s,val);
    return val;
}

QString Util::findFileInSubDirectories(QString search, QString dir, QString extension)
{
    QDir directory(dir);
    return listFiles(directory, search);

}

float Util::clamp(float val, const float mi, const float ma) {
    val = std::min(ma, val);
    val = std::max(mi, val);
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

float Util::ColorLength(QColor &c) {
    return sqrt(c.red()*c.red() + c.green()*c.green() + c.blue()*c.blue());
}

bool Util::isNumber(QString s)
{
    int base = 10;
    if (s.startsWith("$")) {
        base = 16;
        s = s.remove("$");
    }
    if (s.startsWith("0x")) {
        base = 16;
        s = s.remove("0x");
    }
    if (s.startsWith("#")) {
        base = 16;
        s = s.remove("#");
    }
    if (s.startsWith("%")) {
        base = 2;
        s = s.remove("%");
    }
    if (s.startsWith("0b")) {
        base = 2;
        s = s.remove("0b");
    }
    bool ok = false;
    int i = s.toInt(&ok,base);
    return ok;
}

void Util::drawBox(QImage *backImage, QImage *img, int i, int j, int size, QRgb color) {
    int imageSize = img->width();
    QRgb mark = QColor(1,1,1).rgba();
    for (int x=std::max(0, i-size/2);x<=std::min(imageSize-1, i+size/2);x++)
        for (int y=std::max(0, j-size/2);y<=std::min(imageSize-1, j+size/2);y++) {
            QColor col = QColor::fromRgba(backImage->pixel(x,y));
            if (col.red()==0) {
                img->setPixel(x,y,color);
                if (x==i && y== j)
                    backImage->setPixel(x,y,mark);
            }
        }
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
            maxNumber = std::max(maxNumber, num);
        }
    }
    maxNumber++;
    return baseName + QString::number(maxNumber).rightJustified(4, '0');// + "." + type;

}

QString Util::getFileWithoutEnding(QString f)
{
    QStringList lst = f.split(".");
    QString s = "";
    for (int i=0;i<lst.count()-1;i++) {
        s+=lst[i];
        if (i!=lst.count()-2)
            s+=".";

    }
    return s;
}

QString Util::loadTextFile(QString filename) {
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString data = in.readAll();
    file.close();
    return data;
}

void Util::SaveTextFile(QString fname, QString text)
{
    if (QFile::exists(fname)) {
        QFile::remove(fname);
    }
    if (fname=="")
        return;
    QFile file(fname);
    file.open(QIODevice::WriteOnly| QIODevice::Text);
    QTextStream f(&file);
    f<<text;
    file.close();

}

QByteArray Util::loadBinaryFile(QString filename) {
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QByteArray ba = file.readAll();
    file.close();
    return ba;
}

QString Util::MilisecondToString(int ms) {
    //        ms+=1000;
    int ds = ms/100;
    int s = (ms/1000);
    int m = (s/60);
    int h = (m/60);
    int d = h/24;
    ds = ds % 10;
    s = s % 60;
    m = m % 60;
    h = h % 24;
    QString str = "";
    if (d!=0)
        str+= QString::number(d) + "d ";
    if (h!=0)
        str+= QString::number(h) + "h ";
    if (m!=0)
        str+= QString::number(m) + "m ";
    str+= QString::number(s) + "." + QString::number(ds) + "s ";
    return str;
}

QVector3D Util::maxQvector3D(const QVector3D a, const QVector3D b) {
    return QVector3D(std::max(a.x(), b.x()),std::max(a.y(), b.y()),std::max(a.z(), b.z()));
}

bool Util::Mollweide(QVector3D &out, float i, float j, float l0, float R, float size) {

    /*        float x = 4*R*sqrt(2)*(2*i/(float)size-1);
        float yy = j*2 - size/2;
        float y = R*sqrt(2)*(2*yy/(float)size-1);


        float t = asin(y/(R*sqrt(2.0)));
        out = QVector3D( asin( (2.0*t + sin(2.0*t))/M_PI),l0 + M_PI*x / (2*R*sqrt(2.0)*cos(t)),0  );
        out.setX(out.x()+M_PI/2);
        out.setY(-out.y()*0.5);
        if (out.y()>-M_PI && out.y()<M_PI)
            return true;
*/
    return false;

}

int Util::getInt16(QByteArray &ba, int pos)
{
    return ((uchar)ba[pos+1]<<8) | (uchar)ba[pos];
}

void Util::setInt16(QByteArray &ba, int pos, int val)
{
    ba[pos+1] = (val>>8)&0xFF;
    ba[pos] = (val)&0xFF;
}

void Util::appendInt16(QByteArray &ba, int val)
{
    ba.append((val)&0xFF);
    ba.append((val>>8)&0xFF);

}

QStringList Util::ByteArrayToHexQStringList(QByteArray &ba)
{
    QStringList l;
    for (int i=0;i<ba.size();i++) {
        l<<Util::numToHex((uchar)ba[i]).remove("$");
    }
    return l;
}

QByteArray Util::HexQStringListToByteArray(QStringList &lst)
{
    QByteArray ba;
    for (QString s:lst) {
        ba.append((uchar)Util::NumberFromStringHex(s));
    }
    return ba;
}

QPoint Util::mapToWindow(QWidget *from, QPoint pt) {
    QWidget *wnd = from->window();
    while(from && from!=wnd){
        pt = from->mapToParent(pt);
        from = from->parentWidget();
    }
    return pt;
}

QPixmap Util::CreateColorIcon(QColor col, int s) {
    QImage img(s,s,QImage::Format_RGB32);

    QColor c2(col.red()/2, col.green()/2, col.blue()/2);
    for (int y=0;y<s;y++)
        for (int x=0;x<s;x++) {
            //            if (s-1-y>x)

            //              img.setPixelColor(x,y, m_list[col].color);
            //        else
            if (y==0 || y==s-1 ||x==0 || x==s-1)
                img.setPixelColor(x,y, c2);
            else
                img.setPixelColor(x,y, col);


        }
    return QPixmap::fromImage(img);

}

QMap<QString, long> Util::sm_fileSizes;


bool Util::fileChanged(QString filename)
{
    long size = QFile(filename).size();
    // First time
    if (!sm_fileSizes.contains(filename)) {
        sm_fileSizes[filename] = size;
        return true;
    }
    // Exists, but sizes are different
    if (sm_fileSizes[filename]!=size) {
        sm_fileSizes[filename] = size;
        return true;
    }
    // Sizes are the same! No change. Probably.
    return false;

}

int Util::CountFilesInAllDirectories(QString dir, QStringList fileTypes)
{
    int cnt = 0;
    QDirIterator it(dir , QDir::Files , QDirIterator::Subdirectories); //QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot
    while (it.hasNext())
    {
        QString s = it.next();
        for (QString& ft: fileTypes)
          if (s.toLower().endsWith("."+ft))
              cnt+=1;
        if (fileTypes.count()==0) cnt++;
    }
    return cnt;
}



float Util::smoothstep(float edge0, float edge1, float x)
{
    // Scale, bias and saturate x to 0..1 range
    x = clamp((x - edge0) / (edge1 - edge0), 0.0, 1.0);
    // Evaluate polynomial
    return x*x*(3 - 2 * x);
}


