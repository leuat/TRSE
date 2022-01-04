#ifndef FORMHELP_H
#define FORMHELP_H

#include <QWidget>
#include <QListWidget>
#include "source/trsedocuments/helpdocumentbuilder.h"

namespace Ui {
class formHelp;
}

class HelpType {
public:
    QString id;
    QString name;
    HelpType() {}
    HelpType(QString i, QString n) {
        id=i;
        name=n;
    }
};





class formHelp : public QWidget
{
    Q_OBJECT

public:
    explicit formHelp(QWidget *parent=nullptr);
    ~formHelp();


    QVector<HelpType> m_helpTypes;
    QVector<QString> m_currentItems;

    QString m_currentType;
    QString m_currentWord = "";
    bool m_curIsTru = false;
    int m_currentSearchItem = 0;
    int m_curTopic, m_curItem, m_idx;

    QString sz1 = "14pt";

//    Highlighter* m_highlighter = nullptr;
    void LoadItems(int idx);
    void LoadItem(QString word);
    void FillTopics();

    QString ApplyColors(QString in);
    void Search(QString txt);
    void SetFontSize(int size);
    void BuildTRU(QStringList truFiles);

    void AppendItem(QListWidget* w, QString s);
    void SearchForItem(QString item);
    HelpDocumentBuilder m_hdb;

private slots:
//    void on_pushButton_clicked();

    void on_lstTopic_itemClicked(QListWidgetItem *item);

    void on_lstItems_itemClicked(QListWidgetItem *item);

    void on_leSearch_textChanged(const QString &arg1);

    void on_lstItems_currentRowChanged(int currentRow);


    void on_leSearch_returnPressed();



private:
    Ui::formHelp *ui;
};

#endif // FORMHELP_H
