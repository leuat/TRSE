#ifndef DIALOGPROJECTSETTINGS_H
#define DIALOGPROJECTSETTINGS_H

#include <QDialog>
#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"

namespace Ui {
class DialogProjectSettings;
}

class DialogProjectSettings : public QDialog
{
    Q_OBJECT
public:
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


public:
    explicit DialogProjectSettings(QWidget *parent = nullptr);
    ~DialogProjectSettings();

private slots:
    void on_btnClose_clicked();

private:
    void FillFromIni();
    void FillToIni();
    void Close();
    Ui::DialogProjectSettings *ui;
};

#endif // DIALOGPROJECTSETTINGS_H
