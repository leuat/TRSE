#ifndef FORMHELP_H
#define FORMHELP_H

#include <QWidget>
#include <QListWidget>

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
    int m_currentSearchItem = 0;
    int m_curTopic, m_curItem, m_idx;
//    Highlighter* m_highlighter = nullptr;
    void LoadItems(int idx);
    void LoadItem(QString word);
    void FillTopics();

    void Search(QString txt);


    void AppendItem(QListWidget* w, QString s);
    void SearchForItem(QString item);

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
