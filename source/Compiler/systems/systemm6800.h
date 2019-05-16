#ifndef SYSTEMM6800_H
#define SYSTEMM6800_H

#include "abstractsystem.h"
#include <QProcess>
#include "source/LeLib/util/util.h"

class SystemM68000 : public AbstractSystem
{
public:
    SystemM68000(CIniFile* settings, CIniFile* proj) : AbstractSystem(settings, proj) {

    }

    virtual void Assemble(QString& text, QString file, QString currentDir);
    virtual void PostProcess(QString& text, QString file, QString currentDir);
};

#endif // SYSTEMM6800_H
