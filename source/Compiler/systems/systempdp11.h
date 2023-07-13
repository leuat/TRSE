#ifndef SYSTEMPDP11_H
#define SYSTEMPDP11_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"
#include "source/LeLib/util/fc8/FC8Compression.h"

class SystemPDP11 : public AbstractSystem
{
public:
    SystemPDP11(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj);

    virtual bool is8bit() override {
        return false;
    }
    int getPointerSize() override {
        return 4;
    }
    virtual TokenType::Type getPointerType() override { return TokenType::Type::LONG;}

    QString CompressFile(QString fileName) override {
        QString old = fileName;
        FC8 fc;
        fileName = fileName+ "_c";
//      if (Util::fileChanged(filename) || !QFile(old).exists())
        fc.Encode(old,fileName);
        return fileName;

    }
    void Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab) override;


/*    virtual void Assemble(QString& text, QString file, QString currentDir, QSharedPointer<SymbolTable>  symTab);
    virtual void PostProcess(QString& text, QString file, QString currentDir);*/
};

#endif // SYSTEMM6800_H
