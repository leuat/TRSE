#ifndef DIALOGEXPORT3D_H
#define DIALOGEXPORT3D_H

#include <QDialog>
#include "source/LeLib/objloader/objloader.h"

#include "source/LeLib/util/cinifile.h"

namespace Ui {
class DialogExport3D;
}

class DialogExport3D : public QDialog
{
    Q_OBJECT



public:
    explicit DialogExport3D(CIniFile* ini, QWidget *parent = nullptr);
    ~DialogExport3D();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void Message(QString txt);

    void FillFromIni();
    void FillToIni();

private:
    Ui::DialogExport3D *ui;
    CIniFile* m_ini = nullptr;
};

#endif // DIALOGEXPORT3D_H
