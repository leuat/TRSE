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

    void SetData(TRSEDocument* doc, CIniFile* colors, QSharedPointer<DataStorage> ds);
    QByteArray& getData();

private slots:
    void on_cbmPattern_currentIndexChanged(int index);

signals:
    void emitReloadPatterns();

private:
    Ui::WidgetPattern *ui;
};

#endif // WIDGETPATTERN_H
