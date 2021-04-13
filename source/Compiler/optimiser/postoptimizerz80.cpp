#include "source/Compiler/optimiser/postoptimizerz80.h"
#include <QDebug>

PostOptimiserZ80::PostOptimiserZ80()
{
    m_registers = QStringList() <<"a"<<"b"<<"c"<<"d"<<"af"<<"bc"<<"de"<<"hl"<<"ix"<<"iy";
    m_branches = QStringList() <<"call" <<"jp";
    m_registerChangingCommands = QStringList() << "call" <<"jp";
    m_bops = QStringList() << "add"<<"sub"<<"xor"<<"or"<<"and";
//    m_axModifiers = QStringList() << "div"<<"idiv"<<"mul"<<"imul";
}


void PostOptimiserZ80::InitRegisters()
{
    m_regs.clear();
}

void PostOptimiserZ80::Analyze(SourceLine &line) {
    QString s = line.m_orgLine.trimmed().simplified().trimmed().toLower();
    if (s.startsWith(";") || s.startsWith("//"))
        return;
    if (s=="")
        return;


    if (!line.m_orgLine.startsWith("\t") && s.contains(":")) {
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


    // Not implemented anything yet!
    // Below are stuff for the x86
/*

    if (m_axModifiers.contains(cmd)) {
        line.m_changeRegs["ax"]="";
    }

    if (cmd=="mov") {
        QString reg = par[0];
        if (m_registers.contains(reg))
        {
            // Don't assume "mov ax,dx" to hold
            if (!m_registers.contains(par[1]))
                line.m_potentialOptimise = true;
            if (par[1].contains("es:di") || par[1].contains("ds:si"))
                line.m_potentialOptimise = false;

            ChangeReg(line, reg, par[1]);
//            line.m_changeRegs[reg] = par[1];
        }
    }

    if (m_bops.contains(cmd)) {
        QString reg = par[0];
        if (m_registers.contains(reg))
        {
            line.m_potentialOptimise = false;
            ChangeReg(line, reg, ""); // Clear current register
        }

    }


    if (cmd=="les" || cmd=="lea") {
        line.m_changeRegs[par[0]] = par[1];
        line.m_changeRegs["es"] = par[1];
        line.m_potentialOptimise = true;
    }*/

}

void PostOptimiserZ80::ChangeReg(SourceLine &line, QString reg, QString val)
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
