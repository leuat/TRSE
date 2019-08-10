#ifndef FORMFJONG_H
#define FORMFJONG_H

#include <QWidget>
#include "trsedocument.h"
#include "source/LeLib/luascript.h"
#include "source/dialogeffects.h"
#include "source/PmmEdit/highlighter.h"
#include "source/dialoghelp.h"

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
    int m_searchFromPos;
    int m_currentFromPos = 0;

    void SetText(QString txt);
    Highlighter* highlighter = nullptr;

    void Save(QString filename) override;
    void Load(QString filename) override;

    void UpdateFromIni();


    void Init(LuaScript& s);

    void keyPressEvent(QKeyEvent *e);


    void Build() override { Run(); }
    void Run() override;

    void SetupHighlighter();

    void SearchInSource();


private slots:
    void on_leSearch_textChanged(const QString &arg1);
    void on_leSearch_returnPressed();

private:
    Ui::FormFjong *ui;
};

#endif // FORMFJONG_H
