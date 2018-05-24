#ifndef DIALOGTRSESETTINGS_H
#define DIALOGTRSESETTINGS_H

#include <QDialog>
#include "source/LeLib/util/cinifile.h"

namespace Ui {
class DialogTRSESettings;
}

class DialogTRSESettings : public QDialog
{
    Q_OBJECT


public:
    explicit DialogTRSESettings(QWidget *parent = 0);

    CIniFile* m_ini;
    void SetInit(CIniFile* ini) {
        m_ini = ini;
        FillFromIni();
    }

    QString fromStringList(QStringList lst) {
        QString whole ="";
        for (QString s: lst) {
            whole+=s;
            if (lst.last()!=s)
                whole=whole+", ";
        }
        return whole;
    }

    QStringList toStringList(QString s) {
        QStringList lst = s.split(",");
        return lst;
    }


    void FillFromIni();
    void FillToIni();

    ~DialogTRSESettings();

private slots:
    void on_pushButton_clicked();

    void on_btnEmulator_clicked();

    void on_btnDasm_clicked();

    void on_btnExomizer_clicked();

private:
    Ui::DialogTRSESettings *ui;
};

#endif // DIALOGTRSESETTINGS_H
