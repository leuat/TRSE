#include "morgasm.h"

MOrgasm::MOrgasm()
{

}

QString MOrgasm::Process(QString s, OrgasmLine& ol)
{
//    qDebug() << "Processing : " <<s;
    QString expr = s.trimmed();
    if (expr.trimmed()=="")
        return "";

    expr = expr.replace("[","(").replace("]",")");
    expr = expr.replace("(+","(");
//    qDebug() << "Processing: " << expr;
    long val = 0;
    QString repl = "";
    if (m_passType==OrgasmData::PASS_LABELS)
        repl = "$1000";
    if (expr.contains("<") || expr.contains(">"))
        repl= "$10";
    expr = expr.replace("#","$");
    QString oexpr = expr;
    expr = expr.replace("<","").replace(">","");
    if (!(expr.startsWith("(") && expr.endsWith(")"))) {
        if (m_regs.contains(expr))
            return expr;
//        qDebug() << "PROCESS: " <<expr <<oexpr << repl;
        expr = OrgasmData::BinopExpr(oexpr, val, repl);

    }
    if (m_passType==OrgasmData::PASS_SYMBOLS) {
        QString tst = expr;
        QString org = expr;
        int cur = 0;
        bool ok =  false;
    //      tst = tst.replace("(","");
      //    tst = tst.replace(")","");

        if (!tst.startsWith("$"))
//            qDebug() << tst;
            for (QString& sym : m_symbolsList)
            {
//                qDebug() << "Contains symbol: " <<tst <<sym;
                //qDebug() << "Testing for symbol " << sym << tst<<m_symbolsList;
                if (!tst.contains(sym))
                    continue;
                if (tst=="nz") {
                    ok = true;
//                    qDebug() << "setting nz";
                    continue;
                }


                int i = m_symbols[sym];
                if (tst==("<"+sym)) {
                    expr = expr.replace("<"+sym,"$"+QString::number(i&0xFF,16));
                    ok = true;
                    break;
                }
                if (tst==(">"+sym)) {
                   expr = expr.replace(">"+sym,"$"+QString::number((i>>8)&0xFF,16));
                   ok = true;
                    break;
                }

                expr = OrgasmData::ReplaceWord(expr,sym,"$"+QString::number(i,16));
                if (org!=expr) {
                    m_symbolsList.move(cur,0);
                    ok = true;
                    break;
                }
                cur++;
            }
        if (!ok && !Util::isNumber(tst) && !tst.startsWith("($") && !tst.startsWith("(0x")){ // && !tst.startsWith("*")) {
  //          qDebug() << "Testing for symbol " << tst<<m_symbolsList;

            throw OrgasmError("Symbol '"+tst+"' undefined",ol);
        }


    }
    return expr;
}
int MOrgasm::getTypeFromParams(QString s)
{
    s=s.toLower();
    //        qDebug() << " Getting type from : " << s;
    if (s=="")
        return Op6809::inh;
    if (s.startsWith("#"))
        return Op6809::imm;
    long i = 0;
    bool ok;
    if (s.contains(",")) {
//        ok = Util::NumberFromStringHex(s.split(",")[0],i);
    }
    else
    {
        if (!m_regs.contains(s.trimmed()))
            return Op6809::ext;
    }
    return Op6809::inh;
}

void MOrgasm::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
{
//    QByteArray d = ol.m_instruction.Assemble(ol.m_expr, m_opCodes, pd, m_symbols, m_pCounter,m_constants, m_regs, m_symbolsList);

    QString expr = ol.m_expr;
    QString orgexpr = expr;
    QByteArray data;
    ol.m_instruction.m_opCode = ol.m_instruction.m_opCode.toLower();

    if (ol.m_instruction.m_opCode=="processor")
        return;
//    qDebug() << line;

    if (ol.m_instruction.m_opCode=="org")
        return;

    int type = getTypeFromParams(expr);


    if (expr.contains("*")) {
//        line = line.replace("*", Util::numToHex(pCounter));
        QString add = expr;//expr.simplified().split(" ")[1].replace(" ", "");
        add = add.replace("*", Util::numToHex(m_pCounter));
        add = Util::BinopString(add);
        expr = add;//expr.split(" ")[0] + " " + add;
    //    qDebug() << "* : " << expr;

    }
    QString orgExpr = expr;
//    if (pass==OrgasmData::PASS_SYMBOLS)
    {

        QStringList l2 = expr.simplified().split(",");

        for (QString& c : m_constList) {
            l2[0] = OrgasmData::ReplaceWord(l2[0],c,m_constants[c]);

        }


        QString tst = l2[0];
        QString org = l2[0];
        tst = tst.replace("#", "").simplified();
        int cur = 0;
        if (!tst.startsWith("$"))
        for (QString& sym : m_symbolsList)
        {
//            qDebug() << "Testing for symbol " << sym << tst;
            if (!tst.contains(sym))
                continue;

            int i = m_symbols[sym];

            l2[0] = OrgasmData::ReplaceWord(l2[0],sym,"$"+QString::number(i,16));
            if (org!=l2[0]) {
                m_symbolsList.move(cur,0);
                break;
            }
            cur++;
        }

        expr = l2[0];
        if (l2.count()>1)
            expr+=","+l2[1];

//        qDebug() << "ORGASM " <<expr << org << ol.m_expr;

        if (expr!="") {
            long val = 0;
            // Replace with *something* temporary
            QString repl = "$1000";
            if (expr.contains("<") || expr.contains(">"))
                repl= "#$10";
            QString oexpr = expr;
            expr = expr.replace("<","").replace(">","");
            if (!(expr.startsWith("(") && expr.endsWith(")"))) {
                bool hasHash = false;
                if (expr.simplified().startsWith("#"))
                    hasHash = true;
                expr = OrgasmData::BinopExpr(oexpr, val, repl);
                if (hasHash)
                    expr = "#"+expr;
            }

            if (val==-1 && pd==OrgasmData::PASS_SYMBOLS) {
                throw OrgasmError("Unknown operation: " +orgExpr,ol);
            }

        }
    }





//    qDebug() << " ** " << line;

    QString m_opCode = ol.m_instruction.m_opCode;




//    OrgasmInstruction::Type type = OrgasmInstruction::imp;
    MOSOperandCycle cyc;

//    if (opCode=="org")
  //      return false;

    QSharedPointer<Op6809> ins = nullptr;
    if (m_instructions.contains(m_opCode))
        ins = m_instructions[m_opCode];
    else
    if (pd==OrgasmData::PASS_SYMBOLS) {
        qDebug() << "ERROR on line : " << m_opCode + " " +expr;
        throw OrgasmError("Opcode type not implemented or illegal: " + m_opCode + "  type " +QString::number(type) + "        on line " + ol.m_expr,ol );
    }
    if (ins==nullptr)
        return;


    int val=0;
    if (type!=OrgasmInstruction::none) {
        QString num = expr.split(",")[0].replace("#","").replace("(","").replace(")","");
        val = Util::NumberFromStringHex(num);
    }




    if (m_opCode=="bpl" || m_opCode=="bne" || m_opCode=="beq" || m_opCode=="bcc" || m_opCode=="bcs" || m_opCode=="bvc" || m_opCode=="bmi" || m_opCode=="bvc" || m_opCode=="bvs") {
        int diff = (val)-m_pCounter-2;
        if (abs(diff)>=128 && pd==OrgasmData::PASS_SYMBOLS) {
            throw OrgasmError("Branch out of range : " +QString::number(diff) + " :" + m_opCode + " " +expr + " on line " + QString::number(ol.m_lineNumber)+"<br>Please remember that you can use the keyword <b>'offpage'</b> in your block to let OrgAsm know that the code should perform off-page jumps.",ol);
        }
        data.append((uchar)diff);
    }
    else

    if (type==OrgasmInstruction::zp || type==OrgasmInstruction::zpx || type==OrgasmInstruction::zpy || type == OrgasmInstruction::izx || type == OrgasmInstruction::izy || type == OrgasmInstruction::izz|| type==OrgasmInstruction::imm) {
        data.append(val);
    }
    else
    if (type==OrgasmInstruction::abs || type==OrgasmInstruction::abx || type == OrgasmInstruction::aby || type == OrgasmInstruction::ind) {
        data.append(val&0xFF);
        data.append((val>>8)&0xFF);
    }
    if (pd != OrgasmData::PASS_SYMBOLS)
        expr = orgExpr;




    if (data.length()>0) {
        m_data.append(data);
        m_pCounter+=data.length();
    }
}



/*
 *
 * Load the 6809 instruction codes from disk
 *
 *
*/
void MOrgasm::LoadCodes(int CPUflavor)
{


    auto lst = Util::loadTextFile(":/resources/text/opcodes_6809.txt").split("\n");


    m_instructions.clear();
    for (QString s: lst) {
        s = s.trimmed().simplified().toLower();
        if (s=="") continue;
        if (s.startsWith("#")) continue;

        QStringList lst = s.split(",");
        Op6809 *op = new Op6809;
        op->name = lst[0].simplified();

        for (int i=0;i<lst.count()-1;i++) {
            bool ok;
            int code = lst[i+1].toInt(&ok, 16);
            op->opcodes.append( code );
        }
        m_instructions[op->name] = QSharedPointer<Op6809>(op);
    }


}
/*
 *
 * Washes parameter for opcode
 * for instance: (a) iy de etc are returned as is
 * (iy + #10) returned as "(iy+*)
 * anything else (**) or **
 *
*/
QString MOrgasm::WashForOpcode(QString test, QString &value,OrgasmLine& ol)
{
    if (test=="")
        return "";
    if (isRegister(test.toLower()))
        return test.toLower();

    QString t = test;
    t = t.remove("(").remove(")");

    if (t.toLower().startsWith("ix+")) {
        value = t.split("+")[1];
        return ("(ix+*)");
    }
    // Null test
    if (m_opCode!="jp") {
        if (test.toLower()=="(iy)") {
            value ="0";
            return "(iy+*)";
        }
        if (test.toLower()=="(ix)"){
            value ="0";
            return "(ix+*)";
        }

    }
    if (t.toLower().startsWith("iy+")) {
        value = t.split("+")[1];
        return ("(iy+*)");
    }

    if (isRegister(t))
        return test;

    value = Process(test,ol);
    // everything else starting with ( is an address **
    if (test.startsWith("(")) {
        return "(**)";
    }


    // Symbols are also addresses
    return "**";
}


