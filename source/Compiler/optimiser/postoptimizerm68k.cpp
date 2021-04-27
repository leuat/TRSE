#include "postoptimizerm68k.h"
#include <QDebug>

PostOptimiserM68K::PostOptimiserM68K()
{
    m_registers = QStringList() <<"d0"<<"d1"<<"d2"<<"d3"<<"d4"<<"d5"<<"d6"<<"d6"<<"a0"<<"a1"<<"a2"<<"a3"<<"a4"<<"a5"<<"a6";
    m_branches = QStringList() <<"bra" <<"jmp"<<"bne"<<"jsr"<<"rts" <<"beq" <<"bpl"<<"bmi"<<"bvc"<<"bvs";
    m_registerChangingCommands = QStringList() << "bra"<<"jmp"<<"jsr"<<"rts";
    m_bops = QStringList() << "add"<<"sub"<<"xor"<<"or"<<"and"<<"mul"<<"div";
//    m_axModifiers = QStringList() << "div"<<"mul"<<"imul";
}


void PostOptimiserM68K::InitRegisters()
{
    m_regs.clear();
}

void PostOptimiserM68K::Analyze(SourceLine &line) {
    QString s = line.m_orgLine.trimmed().simplified().trimmed().toLower();
    if (s.startsWith(";") || s.startsWith("//"))
        return;
    if (s=="")
        return;


    if (!line.m_orgLine.startsWith("\t")) {
//        qDebug() << "IS LABEL : "<<s <<line.m_orgLine;
        line.m_isLabel = true;

    }

    s = s.remove("word").remove("byte").simplified().split(";").first().simplified();
    QStringList lst = s.split(" ");
    if (lst.count()==0)
        return;

    QStringList par;
    QString cmd = lst[0];
    line.m_cmd = cmd;
    QString rest = s.remove(cmd).simplified();
    if (lst.count()>1)
        par = rest.split(",");
    for (QString&st : par)
        st = st.simplified().trimmed();


/*
    if (m_axModifiers.contains(cmd)) {
        line.m_changeRegs["ax"]="";
    }
*/
    if (cmd=="move") {
        QString reg = par[1];
//        qDebug() << "reg:" <<reg <<line.m_orgLine << m_registers.contains(reg);
        if (m_registers.contains(reg))
        {
            // Don't assume "mov ax,dx" to hold
            if (!m_registers.contains(par[0]))
                line.m_potentialOptimise = true;

            ChangeReg(line, reg, par[0]);
//            line.m_changeRegs[reg] = par[1];
        }
    }
/*
    if (m_bops.contains(cmd)) {
        QString reg = par[0];
        if (m_registers.contains(reg))
        {
            line.m_potentialOptimise = false;
            ChangeReg(line, reg, ""); // Clear current register
        }

    }

*/
    QString w ="";
    auto l = cmd.split(".");
    cmd = l.first();
    if (l.count()>=2)
        w = l[1];

    if (cmd=="lea") {
        line.m_changeRegs[par[1]] = par[0];
        line.m_potentialOptimise = true;
    }
}

void PostOptimiserM68K::ChangeReg(SourceLine &line, QString reg, QString val)
{

    line.m_changeRegs[reg] = val;
    if (reg[1]=='h' || reg[1]=='l')
        line.m_changeRegs[QString(reg[0])+"x"] = "";

/*    if (reg[1]=='x') {
        line.m_changeRegs[QString(reg[0])+"l"] = "";
        line.m_changeRegs[QString(reg[0])+"h"] = "";
    }
*/
}
