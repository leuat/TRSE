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
    QString name, strval;
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
                else
                    items[i].lst<<val;
                return;
            }

        }
        CItem i;
        i.name = name;
//        if (isUnique)
  //          AddUniqueString(&items[i], val);

        i.lst<<val;
        items.append(i);

    }

    void setStringList(QString name, QStringList val) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed()) {
                items[i].lst = val;
                return;
            }

        }
        CItem i;
        i.name = name;
//        if (isUnique)
  //          AddUniqueString(&items[i], val);

        i.lst = val;
        items.append(i);

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
    }

    int getInt(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed())
                return (int)items[i].dval;
        }
        qDebug() << "CIniFile: Could not find parameter " + name;
    }

    double getdouble(QString name) {
        for (int i=0;i<items.size();i++) {
            if (items[i].name==name.toLower().trimmed())
                return items[i].dval;
        }
        qDebug() << "CIniFile: Could not find parameter " + name;
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
