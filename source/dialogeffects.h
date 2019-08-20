#ifndef DIALOGEFFECTS_H
#define DIALOGEFFECTS_H

#include <QDebug>
#include <QDialog>
#include "source/effects/abstractdemoeffect.h"
#include "source/effects/demoeffecttwister.h"
#include "source/effects/demoeffectraytracer.h"
#include "source/LeLib/luascript.h"
#include "source/LeLib/limage/compression.h"
#include <QPixmap>

namespace Ui {
class DialogEffects;
}


class DialogEffects : public QDialog
{
    Q_OBJECT


public:
    explicit DialogEffects(QString file, QWidget *parent = nullptr);
    ~DialogEffects();
    void Abort();
    void Init(QString dir);
private:

    static int Message(lua_State *L);
    QString m_file;

    void Create();
    Ui::DialogEffects *ui;
    int m_avg=0;
    void LoadScript(QString file);
    void UpdateGlobals();
//    QMap<QString, Material> m_materials;


public slots:
    void UpdateImage();
private slots:
/*    void on_pushButton_clicked();
    void on_btnToggleAnim_clicked();
    void on_btnToggleC64_clicked();
    void on_btnSave_clicked();
    void on_comboBox_activated(const QString &arg1);*/
};

#endif // DIALOGEFFECTS_H
