#ifndef DIALOGEXPORT_H
#define DIALOGEXPORT_H

#include <QDialog>
#include "source/LeLib/limage/limage.h"
#include <QMap>
namespace Ui {
class DialogExport;
}

class DialogExport : public QDialog
{
    Q_OBJECT

public:
    explicit DialogExport(QWidget *parent = 0);

    void Init(LImage* img);
    ~DialogExport();

    LImage* m_image;
    QMap<QString, QLineEdit*> m_les;
    bool isOk=true;
    void FillParams();
    void Apply();
private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::DialogExport *ui;
};

#endif // DIALOGEXPORT_H
