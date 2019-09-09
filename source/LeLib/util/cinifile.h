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

#ifndef CINIFILE_H
#define CINIFILE_H

#include <QString>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QVector>
#include <QVector3D>
#include <QColor>

class CItem {
public:
    QString name="", strval="";
    QStringList lst;
    double dval;
    QVector3D vec;
};

class CIniFile  {
public:
    QString filename;

    QVector<CItem> items;

    CIniFile() {}
    void Load(QString fname);
    void Save(QString fname);

    void Save() {
        Save(filename);
    }

    bool contains(QString name) {
        for (int i=0;i<items.size();i++)
            if (items[i].name==name.toLower().trimmed())
                return true;

       return false;

    }

    QString getString(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed())
                return items[i].strval;
        }
        return "";
    }


    QVector3D getVec(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed())
                return items[i].vec;
        }
        return QVector3D(0,0,0);
    }
    QColor getColor(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                QColor c;
                c.setRed(items[i].vec.x());
                c.setGreen(items[i].vec.y());
                c.setBlue(items[i].vec.z());
                return c;
            }
        }
        return QColor(0,0,0);
    }

    QStringList getStringList(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed())
                return items[i].lst;
        }
        return QStringList();
    }

    void setString(QString name, QString val) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                items[i].strval = val;
                return;
            }

        }
        CItem i;
        i.name = name;
        i.strval  = val;
        items.append(i);

    }

    void setFloat(QString name, float val) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                items[i].dval = val;
                items[i].strval = "";
                return;
            }

        }
        CItem i;
        i.name = name;
        i.dval  = val;
        i.strval = "";
        items.append(i);

    }


    void setVec(QString name, QVector3D val) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                items[i].vec = val;
                return;
            }

        }
        CItem i;
        i.name = name;
        i.vec  = val;
        items.append(i);

    }

    void addStringList(QString name, QString val, bool isUnique) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                if (isUnique)
                    AddUniqueString(&items[i], val);
                else {
                    items[i].lst<<val;
                    items[i].strval = "";
                }
                return;
            }

        }
        CItem i;
        i.name = name;
//        if (isUnique)
  //          AddUniqueString(&items[i], val);

        i.lst<<val;
        i.strval = "";
        items.append(i);

    }

    void setStringList(QString name, QStringList val) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                items[i].lst = val;
                items[i].strval = "";
                return;
            }

        }
        CItem i;
        i.name = name;
//        if (isUnique)
  //          AddUniqueString(&items[i], val);

        i.lst = val;
        items.append(i);
        i.strval="";
    }



    void removeFromList(QString name, QString val) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                items[i].lst.removeAll(val);
                return;
            }

        }

    }

    void AddUniqueString(CItem* it,QString str) {
        it->lst.removeAll(str);
        it->lst.insert(0, str);
        it->strval = "";
    }

    bool getBool(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                if (items[i].strval=="true")
                    return true;
                return false;
            }
        }
        qDebug() << "CIniFile: Could not find parameter " + name;
        return false;
    }

    int getInt(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed())
                return (int)items[i].dval;
        }
        qDebug() << "CIniFile: Could not find parameter " + name;
        return 0;
    }

    double getdouble(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed())
                return items[i].dval;
        }
        qDebug() << "CIniFile: Could not find parameter " + name;
        return 0;
    }

/*    bool find(QString key, QString value) {
        QQString actualValue = QQString::fromStdQString(getQString(key.toLower().toStdQString()));
        return value.toLower().compare(actualValue) == 0;
    }

    bool find(QString key, int value) {
        return getint(key.toStdQString()) == value;
    }

    bool find(QString key, bool value) {
        return getbool(key.toStdQString()) == value;
    }*/
};
#endif // CINIFILE_H
