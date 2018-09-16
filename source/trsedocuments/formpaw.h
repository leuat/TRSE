#ifndef FORMPAW_H
#define FORMPAW_H

#include <QWidget>
#include <QVector>
#include <QDir>
#include "trsedocument.h"
#include "source/LeLib/util/cinifile.h"
#include "source/LeLib/util/util.h"

namespace Ui {
class FormPaw;
}

class PawFile {
public:
    QString inFile, cFile, address, name;
    QString incCFile;
    int packedSize=0;

    PawFile(QString ifile, QString cfile, QString addr, QString n) {
        name = n;
        address = addr;
        cFile = cfile;
        inFile = ifile;

    }
    PawFile() {}
};

class FormPaw : public TRSEDocument
{
    Q_OBJECT

    CIniFile m_pawData;
    QVector<PawFile> m_files;
    QString output;
public:
    explicit FormPaw(QWidget *parent = nullptr);
    ~FormPaw();


    void FillFromIni();
    void FillToIni();

    void Save(QString filename) override;
    void Load(QString filename) override;
    void Build() override;
    void CreateIncludefile();

    void Destroy() override {}

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::FormPaw *ui;
};

#endif // FORMPAW_H
