#ifndef FORMFJONG_H
#define FORMFJONG_H

#include <QWidget>
#include "trsedocument.h"
#include "source/LeLib/luascript.h"
#include "source/dialogeffects.h"
#include "source/PmmEdit/highlighter.h"

namespace Ui {
class FormFjong;
}

class FormFjong : public TRSEDocument
{
    Q_OBJECT

public:

    explicit FormFjong(QWidget *parent = nullptr);
    ~FormFjong();
    QFont m_font;
    void Destroy() override {}

    void SetText(QString txt);
    Highlighter* highlighter = nullptr;

    void Save(QString filename) override;
    void Load(QString filename) override;

    void UpdateFromIni();


    void Init(LuaScript& s);


    void Build() override { Run(); }
    void Run() override;

    void SetupHighlighter();


private:
    Ui::FormFjong *ui;
};

#endif // FORMFJONG_H
