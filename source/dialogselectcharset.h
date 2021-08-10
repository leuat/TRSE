#ifndef DIALOGSELECTCHARSET_H
#define DIALOGSELECTCHARSET_H

#include <QDialog>
#include <QKeyEvent>
#include "source/LeLib/limage/charsetimage.h"

namespace Ui {
class DialogSelectCharset;
}

class DialogSelectCharset : public QDialog
{
    Q_OBJECT
    LImage* m_charset = nullptr;
public:

    explicit DialogSelectCharset(LImage* charset, QWidget *parent = nullptr);
    ~DialogSelectCharset();
    int m_char=-1;
    void keyPressEvent(QKeyEvent *e) override;
    void UpdateCharSet();
    bool m_start=false;


private slots:
    void on_lstCharMap_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

private:
    Ui::DialogSelectCharset *ui;
};

#endif // DIALOGSELECTCHARSET_H
