#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"
#include "systemtrs80coco.h"


SystemTRS80CoCo::SystemTRS80CoCo(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System6809(settings,proj)
{

}

void SystemTRS80CoCo::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{

}

void SystemTRS80CoCo::PostProcess(QString &text, QString file, QString currentDir)
{

}

void SystemTRS80CoCo::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini)
{

}
