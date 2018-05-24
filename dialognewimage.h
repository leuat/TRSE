#ifndef DIALOGNEWIMAGE_H
#define DIALOGNEWIMAGE_H

#include <QDialog>
#include <QDebug>
#include "source/LeLib/limage/imageleveleditor.h"

namespace Ui {
class DialogNewImage;
}

class DialogNewImage : public QDialog
{
    Q_OBJECT

public:
    explicit DialogNewImage(QWidget *parent = 0);
    void Initialize(QStringList cmbData);
    int retVal = -1;
    bool isResize = false;
    bool started = true;
    CharmapGlobalData m_meta;
    void SetResizeMeta(CharmapGlobalData gd);
    void ToMeta();
    void CreateInfo();


    ~DialogNewImage();
private slots:
    void slotOk();

    void on_comboBox_currentIndexChanged(int index);

    void on_leScreenWidth_textChanged(const QString &arg1);

    void on_leScreenHeight_textChanged(const QString &arg1);

    void on_leLevelsX_textChanged(const QString &arg1);

    void on_leLevelsY_textChanged(const QString &arg1);

    void on_leExtraDataSize_textChanged(const QString &arg1);

    void on_leStartX_textChanged(const QString &arg1);

    void on_leStartY_textChanged(const QString &arg1);

    void on_leChunkSize_textChanged(const QString &arg1);

    void on_leDataChunks_textChanged(const QString &arg1);

private:
    Ui::DialogNewImage *ui;
};

#endif // DIALOGNEWIMAGE_H
