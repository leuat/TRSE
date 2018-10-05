#ifndef MACHINECODEANALYZER_H
#define MACHINECODEANALYZER_H

#include <QByteArray>
#include <QFile>
#include <QVector>
#include "source/Compiler/assembler/assembler.h"


class MCZP {
 public:
    int m_zp, m_addr;
    MCZP(int zp, int addr) {
        m_zp = zp;
        m_addr = addr;
    }
    MCZP() {

    }
};

class MachineCodeAnalyzer
{
public:
    int startAddr = 0x800;

    QByteArray m_data;
    MachineCodeAnalyzer();
    void Load(QString file);
    QVector<MCZP> m_mczp;
    void ClassifyZP(QVector<MemoryBlock*> blocks);

    void AnalyzeZP();


};

#endif // MACHINECODEANALYZER_H
