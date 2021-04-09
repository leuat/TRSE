#ifndef DIALOGCUSTOMWARNING_H
#define DIALOGCUSTOMWARNING_H

#include <QDialog>
#include "source/LeLib/util/cinifile.h"

namespace Ui {
class DialogCustomWarning;
}

class DialogCustomWarning : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCustomWarning(QWidget *parent = nullptr);
    QString m_field;
    CIniFile* m_ini;

    void Init(QString title, QString message, QString iniField, CIniFile* ini);

    ~DialogCustomWarning();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::DialogCustomWarning *ui;
};

#endif // DIALOGCUSTOMWARNING_H
