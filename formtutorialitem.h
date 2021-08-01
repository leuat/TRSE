#ifndef FORMTUTORIALITEM_H
#define FORMTUTORIALITEM_H

#include <QWidget>

namespace Ui {
class FormTutorialItem;
}

class FormTutorialItem : public QWidget
{
    Q_OBJECT

    QString m_tutorialFile;

public:
    explicit FormTutorialItem(QWidget *parent = nullptr);
    ~FormTutorialItem();
    void Initialise(QString name, QString description, QString image, QString file);

private slots:
    void on_pushButton_clicked();

private:
    Ui::FormTutorialItem *ui;

signals:
    void emitLoadTutorialProject(QString file);

};

#endif // FORMTUTORIALITEM_H
