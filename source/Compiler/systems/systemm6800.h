#ifndef SYSTEMM6800_H
#define SYSTEMM6800_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/LeLib/util/fc8/FC8Compression.h"
class SystemM68000 : public AbstractSystem
{
public:
    SystemM68000(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    virtual bool is8bit() override {
        return false;
    }

    QString CompressFile(QString fileName) override {
        QString old = fileName;
        FC8 fc;
        fileName = fileName+ "_c";
//      if (Util::fileChanged(filename) || !QFile(old).exists())
        fc.Encode(old,fileName);
        return fileName;

    }

/*    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);*/
};

#endif // SYSTEMM6800_H
