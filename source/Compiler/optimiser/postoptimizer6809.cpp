#include "source/Compiler/optimiser/postoptimizer6809.h"
#include <QDebug>
#include "source/LeLib/util/util.h"

PostOptimiser6809::PostOptimiser6809()
{
    m_registers = QStringList() <<"a"<<"b"<<"y"<<"x"<<"u"<<"d"<<"s";
    m_branches = QStringList() <<"bra" <<"jsr" << "jmp";
    m_registerChangingCommands = QStringList() << "bra" <<"jsr"<<"jmp";
    m_bops = QStringList() << "add"<<"sub"<<"xor"<<"or"<<"and"<<"inc"<<"dec"<<"adc"<<"sbc";
//    m_axModifiers = QStringList() << "div"<<"idiv"<<"mul"<<"imul";
}


void PostOptimiser6809::InitRegisters()
{
    m_regs.clear();
}

void PostOptimiser6809::Analyze(SourceLine &line) {
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
*/

    if (cmd=="tfr") {
        QString reg = par[0];
        auto l0 = prevLine->m_orgLine.toLower().simplified();
        auto l1 = line.m_orgLine.toLower().simplified();
        if (l0==l1)
            line.m_forceOptimise = true;
        if (l0=="tfr x,d" && l1=="tfr d,x") {
            prevLine->m_forceOptimise = true;
            line.m_forceOptimise = true;
        }
        if (l0=="tfr d,x" && l1=="tfr x,d") {
            prevLine->m_forceOptimise = true;
            line.m_forceOptimise = true;
        }
        if (l0=="tfr d,x" && l1=="tfr x,y") {
            prevLine->m_forceOptimise = true;
            line.m_orgLine = "\ttfr d,y";
        }

//        tfr d,x
//        tfr x,y

        //  ldy #blah
        //  tfr y,d
        //  to  ldd #blah
        if (l1 =="tfr y,d") {
            if (prevLine->m_cmd=="ldy") {
                line.m_forceOptimise = true;
                prevLine->m_orgLine = prevLine->m_orgLine.replace("ldy","ldd");
            }
        }


        if (m_registers.contains(reg))
        {
            // Don't assume "mov ax,dx" to hold

            if (!m_registers.contains(par[1]))
                line.m_potentialOptimise = true;



//            if (par[1].contains("[") || par[1].contains("("))
  //              line.m_potentialOptimise = false;

            // Check the HL set
            if (prevPar.count()>=2 && prevCmd=="ld") {
                if (prevPar[0]==par[1] && prevPar[1]==par[0]) {
                    line.m_potentialOptimise = true;
                    line.m_forceOptimise = true;
                }
            }

//            if (line.m_potentialOptimise)
  //              qDebug() << "Changing : " <<reg<<par[1] << line.m_orgLine;

            ChangeReg(line, reg, par[1]);

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
    prevCmd = cmd;
    prevPar = par;
    prevLine = &line;


}

void PostOptimiser6809::ChangeReg(SourceLine &line, QString reg, QString val)
{

    line.m_changeRegs[reg] = val;
//    if (reg[1]=='h' || reg[1]=='l')
    if (reg.length()>=2) {
        if (val.startsWith("$")) {
        line.m_changeRegs[QString(reg[0])] = Util::numToHex(Util::NumberFromStringHex(val)&0xFF);
        line.m_changeRegs[QString(reg[1])] = Util::numToHex((Util::NumberFromStringHex(val)>>8)&0xFF);
        }
        else {
            line.m_changeRegs[QString(reg[0])] = "";
            line.m_changeRegs[QString(reg[1])] = "";
        }
    }

    // setting ld a,b etc.. to a register.. cancel value
    if (m_registers.contains(val)) {
        line.m_changeRegs[reg]  ="";
        if (reg.length()>=2) {
            line.m_changeRegs[QString(reg[0])] = "";
            line.m_changeRegs[QString(reg[1])] = "";

        }
        line.m_potentialOptimise = false;

    }
}
