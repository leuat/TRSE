#ifndef OPCODES6502_H
#define OPCODES6502_H
#include <QString>
#include <QStringList>
#include <QMap>


class MOSOperandCycle {
public:
    QString m_name;
    int m_implied;
  //  int m_acc;
    int m_immediate;
    int m_absolute;
    int m_absoluteWithParam;
    int m_zeropage;
    int m_zeropageWithParam;
    int m_opcode;
    MOSOperandCycle() {}
    MOSOperandCycle(QString name, int implied, int immediate, int absolute, int abswp, int zp, int zpwp) {
        m_name = name;
        m_implied = implied;
 //       m_acc = acc;
        m_immediate = immediate;
        m_absolute = absolute;
        m_absoluteWithParam = abswp;
        m_zeropage = zp;
        m_zeropageWithParam = zpwp;
//        m_opcode = opcode;
    }
};

class MOSOperation {
  public:
    QString operand;
    QString param1 = "";
    QString param2 ="";
    bool isNumeric = false;
    bool is16bit = false;
    bool isZeroPage = false;
};



class Opcodes6502
{
public:
    Opcodes6502();
    void InitMosOpCycles();
    QMap<QString, MOSOperandCycle> m_opCycles;
    bool contains(QString s) {
        return m_opCycles.contains(s);
    }
    MOSOperandCycle operator [](QString s) {
            return m_opCycles[s];
       }
};

#endif // OPCODES6502_H
