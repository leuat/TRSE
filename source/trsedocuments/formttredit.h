#ifndef FORMTTREDIT_H
#define FORMTTREDIT_H

#include <QWidget>
#include "trsedocument.h"
#include "source/LeLib/ttrfile.h"
#include "source/trsetracker/widgetpattern.h"

namespace Ui {
class FormTTREdit;
}

class FormTTREdit : public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormTTREdit(QWidget *parent = nullptr);
    ~FormTTREdit();
    TTRFile m_ttr;
    CIniFile m_colors;
    QString m_rawFilename;
    int m_currentInstrument = -1;
    QVector<WidgetPattern*> m_curPatterns;
    QVector<int> m_curPatternValues;
    void Destroy() override {}
    void SaveCurrentInstrument();

    bool Load(QString filename) override;
    void Save(QString filename) override;

    void Reload() override;

    void InitDocument(WorkerThread* t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) override;

    void FillGUIFromData();
    void ApplyCurrentOrder();

private slots:

    void ReloadPatterns();
    void UpdatePatterns();



    void on_cmbPattern_currentIndexChanged(int index);

    void on_cmbInstruments_currentIndexChanged(int index);

private:
    Ui::FormTTREdit *ui;
};

#endif // FormTTREdit_H
