#ifndef FORMRTF_H
#define FORMRTF_H

#include <QWidget>
#include "trsedocument.h"

namespace Ui {
class FormRTF;
}

class FormRTF : public TRSEDocument
{
    Q_OBJECT

public:
    explicit FormRTF(QWidget *parent = nullptr);
    ~FormRTF();
    bool Load(QString filename) override;

    void InitDocument(WorkerThread *t, QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pro) override;
    void Destroy() override {
    }

private:
    Ui::FormRTF *ui;
};

#endif // FORMRTF_H
