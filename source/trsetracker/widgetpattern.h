#ifndef WIDGETPATTERN_H
#define WIDGETPATTERN_H

#include <QWidget>
#include "source/ImageEditor/hexview.h"
namespace Ui {
class WidgetPattern;
}

class WidgetPattern : public QWidget
{
    Q_OBJECT

public:
    explicit WidgetPattern(QWidget *parent = nullptr);
    ~WidgetPattern();

    QComboBox* getPatternCmb();

    void SetData(TRSEDocument* doc, CIniFile* colors, QSharedPointer<DataStorage> ds, int curPatt, int curColumn);
    void UpdateData(QByteArray& ba);
    QByteArray& getData();
    int m_curPattern, m_curColumn;
    void RefreshAll();
    void SetCursorPosition(int pos);

private slots:
    void on_cbmPattern_currentIndexChanged(int index);
    void PropagateUpdate();
    void acceptMove(int direction, int pos);
    void acceptSound(QByteArray ba) {
        emit emitSound(m_curColumn, ba);
    }

signals:
    void emitSound(int, QByteArray sound);
    void emitReloadPatterns();
    void emitUpdatePatterns(WidgetPattern* who, int curPattern);
    void emitMove(int dir, int pos, int curColumn);


private:
    Ui::WidgetPattern *ui;
};

#endif // WIDGETPATTERN_H
