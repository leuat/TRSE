#ifndef DIALOGEFFECTS_H
#define DIALOGEFFECTS_H

#include <QDebug>
#include <QDialog>
#include "source/effects/abstractdemoeffect.h"
#include "source/effects/demoeffecttwister.h"

namespace Ui {
class DialogEffects;
}

class DialogEffects : public QDialog
{
    Q_OBJECT

    AbstractDemoEffect* m_effect = nullptr;

public:
    explicit DialogEffects(QWidget *parent = nullptr);
    ~DialogEffects();

    void Abort();

private:
    void Create();
    Ui::DialogEffects *ui;


public slots:
    void UpdateImage();
private slots:
    void on_pushButton_clicked();
    void on_btnToggleAnim_clicked();
    void on_btnToggleC64_clicked();
    void on_btnSave_clicked();
};

#endif // DIALOGEFFECTS_H
