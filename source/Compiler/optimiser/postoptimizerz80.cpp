#include "source/Compiler/optimiser/postoptimizerz80.h"
#include <QDebug>
#include "source/LeLib/util/util.h"

PostOptimiserZ80::PostOptimiserZ80()
{
    m_registers = QStringList() <<"a"<<"b"<<"c"<<"d"<<"h"<<"l"<<"d"<<"e"<<"af"<<"bc"<<"de"<<"hl"<<"ix"<<"iy"<<"p"<<"sp";
    m_branches = QStringList() <<"call" <<"jp";
    m_registerChangingCommands = QStringList() << "call" <<"jp" << "pop";
    m_bops = QStringList() << "add"<<"sub"<<"xor"<<"or"<<"and"<<"inc"<<"dec"<<"adc"<<"sbc";
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


    if (line.m_orgLine.contains(";keep"))
        return;
    // Not implemented anything yet!
    // Below are stuff for the x86
/*

    if (m_axModifiers.contains(cmd)) {
        line.m_changeRegs["ax"]="";
    }
*/
    if (cmd=="ld") {
        QString reg = par[0];
        if (prevLine->m_orgLine.toLower().simplified()==line.m_orgLine.toLower().simplified()) {
            line.m_forceOptimise = true;

        }
        if (m_registers.contains(reg))
        {
            // Don't assume "mov ax,dx" to hold

            if (!m_registers.contains(par[1]))
                line.m_potentialOptimise = true;



            if (par[1].contains("[") || par[1].contains("("))
                line.m_potentialOptimise = false;

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


    if (cmd=="ex" && line.m_orgLine.contains(";keep")) {
//        qDebug() << "h0 " << line.m_orgLine << cmd << prevLine << par;

        if (par.count()>=2 && prevLine!=nullptr)
        {
            /*Replaces:
                 * ld hl,$10
                 * ex de,hl
                 *
                 * with
                 *
                 * ld de,$10
                */

            if (par[0]=="de" && par[1]=="hl") {
//                qDebug() << "h1 " << line.m_orgLine << prevLine->m_orgLine;;
                if (prevPar.count()>=2 && prevCmd == "ld" && prevPar[0]=="hl") {
  //                  qDebug() << "h2 " << prevLine->m_orgLine;
                    line.m_potentialOptimise = true;
                    line.m_forceOptimise = true;
                    prevLine->m_orgLine = "\t"+prevLine->m_orgLine.trimmed().simplified().replace("hl,","de,");

                    prevLine->m_changeRegs["de"] = line.m_changeRegs["hl"];
                    prevLine->m_changeRegs.remove("hl");
                    prevLine->m_potentialOptimise = false;
                    prevLine->m_forceOptimise = false;

                }
                if (prevPar.count()>=2 && prevCmd == "ex" && prevPar[0]=="de" && prevPar[1]=="hl")
                {
                    prevLine->m_forceOptimise = true;
                    line.m_forceOptimise = true;
                }

            }
        }


 /*           if (m_lastMemoryStoredValue.contains(reg))
/             if (par[1]==m_lastMemoryStoredValue[reg]) {
                 line.m_potentialOptimise = true;
                 qDebug() << "Checking for : "<<reg <<m_lastMemoryStoredValue.contains(reg)<<m_lastMemoryStoredValue[reg]<<par[1];
                 line.m_forceOptimise = true;
             }
*/
//            qDebug() << "Changing reg: "<<reg<<par[1];
//            line.m_changeRegs[reg] = par[1];
  /*      else if (par.count()>=2){
            QString r2  =par[1];
            if (m_registers.contains(r2)) {
                m_lastMemoryStoredValue[r2] = par[0];
                qDebug() << "Setting stored memory: "<<r2 <<par[0];
            }
        }
*/
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

/*

    if (cmd=="les" || cmd=="lea") {
        line.m_changeRegs[par[0]] = par[1];
        line.m_changeRegs["es"] = par[1];
        line.m_potentialOptimise = true;
    }*/

}

void PostOptimiserZ80::ChangeReg(SourceLine &line, QString reg, QString val)
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
/*    if (reg[1]=='x') {
        line.m_changeRegs[QString(reg[0])+"l"] = "";
        line.m_changeRegs[QString(reg[0])+"h"] = "";
    }
*/
}
