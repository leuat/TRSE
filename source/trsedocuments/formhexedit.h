#ifndef FORMHEXEDIT_H
#define FORMHEXEDIT_H

#include <QWidget>
#include "trsedocument.h"

namespace Ui {
class FormHexEdit;
}

class FormHexEdit : public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormHexEdit(QWidget *parent = nullptr);
    ~FormHexEdit();
    QByteArray m_data;
    CIniFile m_colors;
    QString m_rawFilename;
    void Destroy() override {}

    bool Load(QString filename) override;
    void Save(QString filename) override;

    void Reload() override;

    void InitDocument(WorkerThread* t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) override;


private slots:
    void on_comboBox_currentIndexChanged(const QString &arg1);

private:
    Ui::FormHexEdit *ui;
};

#endif // FORMHEXEDIT_H
