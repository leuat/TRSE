#ifndef FORMTTREDIT_H
#define FORMTTREDIT_H

#include <QWidget>
#include "trsedocument.h"
#include "source/LeLib/ttrfile.h"
#include "source/trsetracker/widgetpattern.h"
#include "source/trsetracker/ttrplayer.h"

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
    bool ignoreApplyPatterns = false;
    int m_currentInstrument = -1;
    QVector<WidgetPattern*> m_curPatterns;
    QVector<int> m_curPatternValues;
    QMap<QString,QString> m_instruments;
    TTRPlayer m_player;

    void Destroy() override {}
    void SaveCurrentInstrument();

    void InitTRT(int channels, int rows);

    bool Load(QString filename) override;
    void Save(QString filename) override;

    void Reload() override;

    void InitDocument(WorkerThread* t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> iniProject) override;

    void FillGUIFromData();
    void ApplyCurrentOrder();
    void LoadPredefinedInstruments();


private slots:


    void acceptSound(int, QByteArray sound);

    void ReloadPatterns();
    void UpdatePatterns();
    void HandleUpdatePatterns(WidgetPattern*, int);
    void HandleMove(int dir, int pos, int col);



    void on_cmbPattern_currentIndexChanged(int index);

    void on_cmbInstruments_currentIndexChanged(int index);

    void on_btnNewPattern_clicked();

    void on_cmbPredefinedInstrument_currentIndexChanged(int index);

    void on_btnSetInstrument_clicked();


    void on_btnNewOrder_clicked();

    void on_btnDeleteOrder_clicked();
    void ReloadOrders();
    void ReloadInstruments();

    void on_btnDeletePattern_clicked();


    void on_btnNewOrder_2_clicked();

    void on_cbmSystem_currentIndexChanged(int index);

    void on_btnPlay_clicked();

    void on_lstOrders_currentRowChanged(int currentRow);

    void on_cmbTRSEInstrument_currentIndexChanged(int index);

private:
    Ui::FormTTREdit *ui;
};

#endif // FormTTREdit_H
