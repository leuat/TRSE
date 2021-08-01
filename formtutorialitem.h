#ifndef FORMTUTORIALITEM_H
#define FORMTUTORIALITEM_H

#include <QWidget>
#include <QMouseEvent>

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

/*    void leaveEvent(QEvent *event) override;
    void enterEvent(QEnterEvent *event) override;*/
private slots:
    void on_pushButton_clicked();

private:
    Ui::FormTutorialItem *ui;

signals:
    void emitLoadTutorialProject(QString file);

};

#endif // FORMTUTORIALITEM_H
