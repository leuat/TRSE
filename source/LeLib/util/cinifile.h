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

    bool contains(QString name);

    QString getString(QString name);


    QVector3D getVec(QString name);
    QColor getColor(QString name);

    QStringList getStringList(QString name);

    void setString(QString name, QString val);

    void setFloat(QString name, float val);


    void setVec(QString name, QVector3D val);

    void addStringList(QString name, QString val, bool isUnique);

    void setStringList(QString name, QStringList val);



    void removeFromList(QString name, QString val);

    void AddUniqueString(CItem* it,QString str);

    bool getBool(QString name);

    int getInt(QString name);

    double getdouble(QString name);

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
