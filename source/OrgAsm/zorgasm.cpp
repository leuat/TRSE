#include "zorgasm.h"

ZOrgasm::ZOrgasm()
{

}


QString ZOrgasm::Process(QString s)
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
        if (!tst.startsWith("$"))
            for (QString& sym : m_symbolsList)
            {
                //            qDebug() << "Testing for symbol " << sym << tst;
//                qDebug() << "Contains symbol: " <<tst <<sym;
                if (!tst.contains(sym))
                    continue;


                int i = m_symbols[sym];
                if (tst==("<"+sym)) {
                    expr = expr.replace("<"+sym,"$"+QString::number(i&0xFF,16));
                    break;
                }
                if (tst==(">"+sym)) {
                   expr = expr.replace(">"+sym,"$"+QString::number((i>>8)&0xFF,16));
                    break;
                }

                expr = OrgasmData::ReplaceWord(expr,sym,"$"+QString::number(i,16));
                if (org!=expr) {
                    m_symbolsList.move(cur,0);
                    break;
                }
                cur++;
            }

    }




/*    if (val==-1 && pd==OrgasmData::PASS_SYMBOLS) {
        throw OrgasmError("Unknown operation: " +orgExpr,ol);
    }
*/
    return expr;
}
/*
void ZOrgasm::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
{
    //    QByteArray d = ol.m_instruction.Assemble(ol.m_expr, m_opCodes, pd, m_symbols, m_pCounter,m_constants, m_regs, m_symbolsList);
//    qDebug() << "START " <<ol.m_instruction.m_opCode<< ol.m_expr;
    ol.m_expr = ol.m_expr.replace("&","$");
    QString expr = ol.m_expr;
    QString orgexpr = expr;
    QByteArray data;
    ol.m_instruction.m_opCode = ol.m_instruction.m_opCode.toLower();

    if (ol.m_instruction.m_opCode=="processor")
        return;
    //    qDebug() << line;

    if (ol.m_instruction.m_opCode=="org")
        return;



    if (expr.contains("*")) {
        //        line = line.replace("*", Util::numToHex(pCounter));
        QString add = expr;//expr.simplified().split(" ")[1].replace(" ", "");
        add = add.replace("*", Util::numToHex(m_pCounter));
        add = Util::BinopString(add);
        expr = add;//expr.split(" ")[0] + " " + add;
        //    qDebug() << "* : " << expr;

    }
    // Fix up jr nz, balle
    //    if (opCode=="org")
    //      return false;

    if (ol.m_instruction.m_opCode=="jr" || ol.m_instruction.m_opCode=="jp") {
        if (expr.contains(","))
        {
            ol.m_instruction.m_opCode = ol.m_instruction.m_opCode+"_"+expr.split(",").first().trimmed();
            expr = expr.split(",").last().trimmed();
            ol.m_expr = expr;
    //        qDebug() << "**** REPLACE :" << ol.m_instruction.m_opCode<< expr;
        }
    }
    QString orgExpr = expr;
    //    if (pass==OrgasmData::PASS_SYMBOLS)
    {
        QStringList l2 = expr.simplified().split(",");
//        qDebug() << "ZORG 1" <<l2;
        for (QString& c : m_constList) {
            l2[0] = OrgasmData::ReplaceWord(l2[0],c,m_constants[c]);

        }
    }





    //    qDebug() << " ** " << line;

    QString m_opCode = ol.m_instruction.m_opCode;




    OrgasmInstruction::Type type = OrgasmInstruction::imp;
    MOSOperandCycle cyc;

    // transform ld a,b to "ld<a>b (none)"
    // ld a,10  to  ld<a 10 (imm)


    if (expr.contains(",")) {
        QStringList lst = expr.split(",");
        QString a1 = lst[0].simplified().trimmed();
        QString a2 = lst[1].simplified().trimmed();
        if (isRegister(a1)) {
            m_opCode +="<"+a1;
            expr = a2;
        }
        if (isRegister(a2)) {
            m_opCode +=">"+a2;
            expr = a1;
        }
        if (isRegister(a1) && isRegister(a2))
           expr = "";
        else expr = Process(expr);


    }
    // if the extra expression is a register, attach it
    if (isRegister(expr.trimmed())) {
//        qDebug() << "IS REGISTER : "<<m_opCode<<expr;
        m_opCode +="<"+expr;
        expr = "";

    }
    else expr = Process(expr);

    expr = expr.trimmed();
    m_opCode = m_opCode.trimmed();

    expr = expr.remove("[");
    expr = expr.remove("]");

    // Finally, check if (c) or (d) or (e) etc
    QString test = expr;
    test = test.remove("(").remove(")");
    if (m_regs.contains(test)) {
        // Uh oh: look out for "in a,(c)"
        m_opCode+=">"+expr;
        expr = "";
    }
    // "Special" registers: discern between ld hl,val and ld hl,(val) by adding a "()"
    if (expr.startsWith("(")) {
        if (m_opCode.endsWith("<hl") ||
                m_opCode.endsWith("<bc") ||
                m_opCode.endsWith("<de") ||
                m_opCode.endsWith("<a") ||
                m_opCode.endsWith("<iy") ||
                m_opCode.endsWith("<ix"))
            m_opCode+="()";
    }
    // Account for strange instructions: im 0 and im 1
    if (m_opCode=="im" || m_opCode.startsWith("bit")) {
        m_opCode+="_"+expr;
        expr = "";
    }


    if (m_opCodes.contains(m_opCode))
        cyc = m_opCodes.m_opCycles[m_opCode];
    else {
        if (m_opCode =="cpu" || m_opCode=="end")
            return; // Ignore
    //    qDebug() << "WOOOT "<<m_opCode<<m_opCodes.m_opCycles.contains(m_opCode);
        throw OrgasmError("Unknown opcode: " + m_opCode,ol);
    }
    if (cyc.m_opcodes.count()==0)
        throw OrgasmError("Opcode illegal or not implemented yet: '" + m_opCode + "' on line '" + orgexpr+"'",ol);


    if (expr!="") {
        type =  ol.m_instruction.getTypeFromParamsZ80(expr);
        if (m_opCode.startsWith("jp") || m_opCode.startsWith("call")) {
            type = OrgasmInstruction::abs;
//            qDebug() << "Replacing to ABS " << m_opCode <<expr;

        }

    }
    else
        type = OrgasmInstruction::none;


    // Override types or pass symbol
    //    if (pass==OrgasmData::PASS_LABELS) {
    if ((int)type>cyc.m_opcodes.length())
        throw OrgasmError("Unknown or non-implemented opcode: " + m_opCode,ol);

    int code = cyc.m_opcodes[(int)type];
    // Type not found? then go ABS!
    if (code==0) {
        code = cyc.m_opcodes[(int)OrgasmInstruction::abs];
        type = OrgasmInstruction::abs;
    }

//    qDebug() <<"OPCODE: " << m_opCode <<Util::numToHex(code) << type <<expr;


    if (code==0 && pd==OrgasmData::PASS_SYMBOLS) {
        qDebug() << "ERROR on line : " << m_opCode + " " +expr << "Type:"<<type;
        throw OrgasmError("Opcode type not implemented or illegal: " + m_opCode + "  type " +QString::number(type) + "        on line " + ol.m_expr,ol );
    }


    // 16-bit data val to add (or 8)
    int val=0;
    if (type!=OrgasmInstruction::none) {
        QString num = expr.split(",")[0].replace("#","$").replace("(","").replace(")","");
//        Util::m_currentForceConversionType = 16; // FORCE ALWAYS HEX

        val = Util::NumberFromStringHex(Util::BinopString(num));
//        if (code==0x32)
  //          qDebug() << "VAL : "<<m_opCode<<expr<< Util::numToHex(val) << num << expr;
    }


    // 16-bit extra OP code
    if (code>=0x100)
        data.append((code>>8)&0xff);

    data.append(code&0xFF);


    if (m_opCode.startsWith("jr") ) {
        int diff = (val)-m_pCounter-2;
//        qDebug() << " ****** RELATIVE: " <<m_opCode<<expr<< diff <<m_pCounter;
        if (abs(diff)>=128 && pd==OrgasmData::PASS_SYMBOLS) {
            throw OrgasmError("Branch out of range : " +QString::number(diff) + " :" + m_opCode + " " +expr + " on line " + QString::number(ol.m_lineNumber)+"<br>Please remember that you can use the keyword <b>'offpage'</b> in your block to let OrgAsm know that the code should perform off-page jumps.",ol);
        }
        data.append((uchar)diff);
    }
    else

        if (type==OrgasmInstruction::imm) {
            data.append(val);
        }
        else
            if (type==OrgasmInstruction::abs || type==OrgasmInstruction::abx || type == OrgasmInstruction::aby || type == OrgasmInstruction::ind) {
                data.append(val&0xFF);
                data.append((val>>8)&0xFF);
            }
    if (pd != OrgasmData::PASS_SYMBOLS)
        expr = orgExpr;

    if (m_opCode=="ld<a>(iy)")
        data.append(val);




    if (data.count()>0) {
        m_data.append(data);
        m_pCounter+=data.count();
    }
}
*/



void ZOrgasm::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
{
    //    QByteArray d = ol.m_instruction.Assemble(ol.m_expr, m_opCodes, pd, m_symbols, m_pCounter,m_constants, m_regs, m_symbolsList);
//    qDebug() << "START " <<ol.m_instruction.m_opCode<< ol.m_expr;
    ol.m_expr = ol.m_expr.replace("&","$");
    QString expr = ol.m_expr;
    QString orgexpr = expr;
    QByteArray data;
    ol.m_instruction.m_opCode = ol.m_instruction.m_opCode.toLower();

    if (ol.m_instruction.m_opCode=="processor")
        return;
    //    qDebug() << line;

    if (ol.m_instruction.m_opCode=="org")
        return;



    if (expr.contains("*")) {
        //        line = line.replace("*", Util::numToHex(pCounter));
        QString add = expr;//expr.simplified().split(" ")[1].replace(" ", "");
        add = add.replace("*", Util::numToHex(m_pCounter));
        add = Util::BinopString(add);
        expr = add;//expr.split(" ")[0] + " " + add;
        //    qDebug() << "* : " << expr;

    }
    // Fix up jr nz, balle
    //    if (opCode=="org")
    //      return false;

    if (ol.m_instruction.m_opCode=="jr" || ol.m_instruction.m_opCode=="jp") {
        if (expr.contains(","))
        {
            ol.m_instruction.m_opCode = ol.m_instruction.m_opCode+" "+expr.split(",").first().trimmed();
            expr = expr.split(",").last().trimmed();
            ol.m_expr = expr;
    //        qDebug() << "**** REPLACE :" << ol.m_instruction.m_opCode<< expr;
        }
    }
    QString orgExpr = expr;
    //    if (pass==OrgasmData::PASS_SYMBOLS)
    {
        QStringList l2 = expr.simplified().split(",");
//        qDebug() << "ZORG 1" <<l2;
        for (QString& c : m_constList) {
            l2[0] = OrgasmData::ReplaceWord(l2[0],c,m_constants[c]);

        }
    }





    //    qDebug() << " ** " << line;

    QString m_opCode = ol.m_instruction.m_opCode;




    OrgasmInstruction::Type type = OrgasmInstruction::imp;
    MOSOperandCycle cyc;

    // transform ld a,b to "ld<a>b (none)"
    // ld a,10  to  ld<a 10 (imm)
    expr = expr.replace("[","(").replace("]",")");
    QString value = 0;
    if (expr.contains(",")) {
        QStringList lst = expr.split(",");
        QString a1 = WashForOpcode(lst[0].simplified().trimmed(),value);
        QString a2 = WashForOpcode(lst[1].simplified().trimmed(),value);
        m_opCode +=" "+a1+","+a2;
        expr = "";
    }

    if (expr!="") {
        QString a1 = WashForOpcode(expr.simplified().trimmed(),value);
        // Now double test:
        QString addChar = " ";

        if (m_opCode.startsWith("jp ") || m_opCode.startsWith("jr "))
            addChar=","; // Needs a COMMA after jr nz, jp pe, etc
        m_opCode +=addChar+a1;
    }
    // Remover extra "*" at the end of jrs)

    // Determine if immediate
    if (!m_opCode.contains("(**)") && !m_opCode.startsWith("jp ") && !m_opCode.startsWith("call"))
        if (m_opCode.contains("**")) {
            bool is8bit = true;
            for (QString& s:m_16bitRegs)
                if (m_opCode.contains(s))
                    is8bit = false;

            if (is8bit) {
                qDebug() << "Replacing: "<<m_opCode;
                m_opCode = m_opCode.replace("**","*");
            }
        }

    // Determine type
    type = OrgasmInstruction::none;
    if (m_opCode.contains("*"))
        type = OrgasmInstruction::imm;

    if (m_opCode.contains("**"))
        type = OrgasmInstruction::abs;

    if (m_opCode.startsWith("jr")) {
        if (m_opCode.endsWith("**"))
           m_opCode.remove(m_opCode.count()-1,1);
        type = OrgasmInstruction::rel;
    }
    /*
     *
     * STRANGE INSTRUCTIONS
     *
     *
    */
    m_opCode = m_opCode.trimmed();
    if (m_opCode.contains("(iy")) {
        m_opCode.replace("(iy","(iy+*");
        type = OrgasmInstruction::imm;
    }
    if (m_opCode.contains("(ix")) {
        m_opCode.replace("(ix","(ix+*");
        type = OrgasmInstruction::imm;
    }
    if (m_opCode.startsWith("im")) {
        m_opCode = "im "+value;
        value = "";
        expr = "";
        type = OrgasmInstruction::none;
    }
    if (m_opCode.startsWith("bit ")) {
        m_opCode = ol.m_orgLine.trimmed().toLower().simplified();
        value = "";
        expr = "";
        type = OrgasmInstruction::none;
    }
//    qDebug() << m_opCode << expr << value << ol.m_orgLine << type;

    int code = 0;
    if (m_opCodes.contains(m_opCode))
        code = m_opCodes[m_opCode];
    else {
        if (m_opCode =="cpu" || m_opCode=="end")
            return; // Ignore

        throw OrgasmError("Unknown opcode: " + m_opCode,ol);
    }


    if (code==0 && pd==OrgasmData::PASS_SYMBOLS) {
        qDebug() << "ERROR on line : " << m_opCode + " " +expr << "Type:"<<type;
        throw OrgasmError("Opcode type not implemented or illegal: " + m_opCode + "  type " +QString::number(type) + "        on line " + ol.m_expr,ol );
    }


    // calculate the actual data
    int val=0;
    if (type!=OrgasmInstruction::none) {
        QString num = value.replace("#","$").replace("(","").replace(")","");
        val = Util::NumberFromStringHex(Util::BinopString(num));
    }

    // Write opcode

    // 16-bit extra OP code
    if (code>=0x100)
        data.append((code>>8)&0xff);
    // 8 bit part
    data.append(code&0xFF);


    // OPERANDS:

    // A relative jump? (jr)
    if (type==OrgasmInstruction::rel)  {
        int diff = (val)-m_pCounter-2;
//        qDebug() << " ****** RELATIVE: " <<m_opCode<<expr<< diff <<m_pCounter;
        if (abs(diff)>=128 && pd==OrgasmData::PASS_SYMBOLS) {
            throw OrgasmError("Branch out of range : " +QString::number(diff) + " :" + m_opCode + " " +expr + " on line " + QString::number(ol.m_lineNumber)+"<br>Please remember that you can use the keyword <b>'offpage'</b> in your block to let OrgAsm know that the code should perform off-page jumps.",ol);
        }
        data.append((uchar)diff);
    }
    else
        // Immediate 8-bit vale?
        if (type==OrgasmInstruction::imm) {
            data.append(val);
        }
        else
            // 16-bit value?
            if (type==OrgasmInstruction::abs) {
                data.append(val&0xFF);
                data.append((val>>8)&0xFF);
            }
    if (pd != OrgasmData::PASS_SYMBOLS)
        expr = orgExpr;



    if (data.count()>0) {
        m_data.append(data);
        m_pCounter+=data.count();
    }
}



void ZOrgasm::LoadCodes(int CPUflavor)
{
    QString filename = ":/resources/text/opcodes_z80.txt";
    QString data = Util::loadTextFile(filename);
    QStringList lst = data.split("\n");
    m_opCodes.clear();
    for (QString s: lst) {
        s=s.trimmed().simplified().toLower();
        if (s=="") continue;
        if (s.startsWith(";")) continue;
        auto l = s.split("->");
        QString c = l[0].trimmed();
        if (!m_opCodes.contains(c))
            m_opCodes[c] = Util::NumberFromStringHex(l[1].trimmed());
    }


}

QString ZOrgasm::WashForOpcode(QString test, QString &value)
{
     // return "hl"
    if (test=="")
        return "";
    if (isRegister(test))
        return test;

//    qDebug() << "Testing: "<<test;

    QString t = test;
    t = t.remove("(").remove(")");
    // return "(hl)";
    if (isRegister(t))
        return test;

    value = Process(test);
    // everything else starting with ( is an address **
    if (test.startsWith("(")) {
        return "(**)";
    }
    // Symbols are also addresses
    return "**";
}

