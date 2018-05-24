#ifndef DIALOGHELP_H
#define DIALOGHELP_H

#include <QDialog>
#include <QString>
#include <QStringList>
#include <QVector>
#include <QListWidgetItem>


namespace Ui {
class DialogHelp;
}

class HelpTopic {
public:
    QString m_text;
    QString m_info;
    QVector<HelpTopic*> m_topics;

};

class Helper {
public:
    HelpTopic m_topics;
    void LoadFromResource();
    QStringList m_lst;
    int m_curIdx = 0;
    int m_curTab = 0;
    void Build(HelpTopic& t, int curTab);
};


class DialogHelp : public QDialog
{
    Q_OBJECT

public:
    explicit DialogHelp(QWidget *parent, QString search);
    ~DialogHelp();
    Helper m_helper;

    int m_curTopic, m_curItem;


    void FillTopics();
    void FillItems(int idx);
    void FillHelpText();
private slots:
    void on_pushButton_clicked();

    void on_lstTopic_itemClicked(QListWidgetItem *item);

    void on_lstItems_itemClicked(QListWidgetItem *item);

    void on_leSearch_textChanged(const QString &arg1);

    void on_lstItems_currentRowChanged(int currentRow);

private:
    Ui::DialogHelp *ui;
};

#endif // DIALOGHELP_H
