#ifndef CUSTOMQTABLEWIDGET_H
#define CUSTOMQTABLEWIDGET_H

#include <QTableWidget>
#include "source/trsedocuments/trsedocument.h"

class CustomQTableWidget : public QTableWidget
{
public:
    CustomQTableWidget(QWidget* parent);

    void InitDoc(TRSEDocument* doc);

    TRSEDocument* m_doc = nullptr;
protected:
    void keyPressEvent(QKeyEvent *e) override;

};

#endif // CUSTOMQTABLEWIDGET_H
