#include "zorgasm.h"

ZOrgasm::ZOrgasm()
{

}

/*
 * Processes an instruction parameter.
 * If a register (ix) or a or de, return.
 * if not, giving it a temporary value of $1000 during
 * the first pass
 *
 * In the next pass, calculate the actual value of the binary expression
 * ie.
 * ld hl, myAddress +4
 * myAddress = $4000
 * then replace with $4004
 *
*/
QString ZOrgasm::Process(QString s, OrgasmLine& ol)
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
            for (QString& sym : m_symbolsList)
            {
//                qDebug() << "Contains symbol: " <<tst <<sym;
                if (!tst.contains(sym))
                    continue;


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
        if (!ok && !Util::isNumber(tst)) {
            throw OrgasmError("Symbol '"+tst+"' undefined",ol);
        }


    }
    return expr;
}


void ZOrgasm::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
{
    ol.m_expr = ol.m_expr.replace("&","$");
    QString expr = ol.m_expr;
    QString orgexpr = expr;
    QByteArray data;
    ol.m_instruction.m_opCode = ol.m_instruction.m_opCode.toLower();

    if (m_ignoreCommands.contains(ol.m_instruction.m_opCode))
        return;

    // Relative jump to current address
    if (expr.trimmed().endsWith("$")) { expr=expr.replace("$","*"); }
    if (expr.contains("*")) {
        QString add = expr;//expr.simplified().split(" ")[1].replace(" ", "");
        add = add.replace("*", Util::numToHex(m_pCounter));
        add = Util::BinopString(add);
        expr = add;//expr.split(" ")[0] + " " + add;

    }
    // Fix up jr nz, something

    if (ol.m_instruction.m_opCode=="jr" || ol.m_instruction.m_opCode=="jp" || ol.m_instruction.m_opCode=="call") {
        if (expr.contains(","))
        {
            ol.m_instruction.m_opCode = ol.m_instruction.m_opCode+" "+expr.split(",").first().trimmed();
            expr = expr.split(",").last().trimmed();
            ol.m_expr = expr;
        }
    }

    // Insert constants
    QString orgExpr = expr;
    //    if (pass==OrgasmData::PASS_SYMBOLS)
    {
        QStringList l2 = expr.simplified().split(",");
        for (QString& c : m_constList) {
            l2[0] = OrgasmData::ReplaceWord(l2[0],c,m_constants[c]);

        }
    }


    // Get opcode
    m_opCode = ol.m_instruction.m_opCode.toLower();
    MOSOperandCycle cyc;
    // Force () instead of []
    expr = expr.replace("[","(").replace("]",")");
    QString value = "";
    QString value2 = "";
    // Treat instructions of the type ld a,b
    // WashforOpcode returns "**","(**)" or just the register a,b,(hl) etc
    // WasForOpcode also calculates the one free value parameter
    // (actually some very few opcodes have 2 input parameters)

    if (expr.contains(",")) {
        QStringList lst = expr.split(",");
        QString a1 = WashForOpcode(lst[0].simplified().trimmed(),value,ol);
        QString a2 = WashForOpcode(lst[1].simplified().trimmed(),value2,ol);
        if (value=="") {
            value = value2;
            value2 ="";
        }
        m_opCode +=" "+a1+","+a2;
        expr = "";
    }
    // Still have an expression?
    if (expr!="") {
        // Wash it for (**) etc and get the value
        QString a1 = WashForOpcode(expr.simplified().trimmed(),value,ol);
        // Now double test:
        QString addChar = " ";

        if (m_opCode.startsWith("jp ") || m_opCode.startsWith("jr ") || m_opCode.startsWith("call "))
            addChar=","; // Needs a COMMA after jr nz, jp pe, etc
        m_opCode +=addChar+a1;
    }
    // Remover extra "*" at the end of jrs)

    // After washed, imms are always **. Determine if immediate is 8-bit *
    if (!m_opCode.contains("(**)") && !m_opCode.startsWith("jp ") && !m_opCode.startsWith("call"))
        if (m_opCode.contains("**")) {
            bool is8bit = false;
            QString tst = m_opCode.split(" ").last();
            for (QString& s:m_8bitRegs)
                if (tst.contains(s))
                    is8bit = true;
            for (QString& s:m_16bitRegs)
                if (tst.contains(s))
                    is8bit = false;

            // Replace ** with * - 8 bit instruction
//            qDebug() << "IS 8 bit : "<<is8bit<<m_opCode;
            if (is8bit)
                m_opCode = m_opCode.replace("**","*");

        }
    // Make sure out is always imm
    m_opCode = m_opCode.replace("out (**)","out (*)");
    m_opCode = m_opCode.replace("in a,(**)","in a,(*)");

    // Missing your favorite opcode? Try a brand new one without the extra "*". What a hack, but works!
    if (!m_opCodes.contains(m_opCode))
        if (m_opCode.endsWith("**"))
            m_opCode.remove(m_opCode.count()-1,1);

    // Determine type
    OrgasmInstruction::Type type = OrgasmInstruction::none;
    // Imm 8 bit
    if (m_opCode.contains("*"))
        type = OrgasmInstruction::imm;
    // imm 16 bit
    if (m_opCode.contains("**"))
        type = OrgasmInstruction::abs;
    // jr is special: will get marked with a 16-bit address, but is
    // actually only 8-bit relative. Remove a * if needed.
    if (m_opCode.startsWith("jr") || m_opCode.startsWith("djnz")) {
        if (m_opCode.endsWith("**"))
           m_opCode.remove(m_opCode.count()-1,1);
        type = OrgasmInstruction::rel;
    }
    /*
     *
     * STRANGE INSTRUCTIONS
     * needs special care
     * because they
     * are fragile flowers
     *
    */

    if (m_opCode.startsWith("im")) {
        m_opCode = "im "+value;
        value = "";
        expr = "";
        type = OrgasmInstruction::none;
    }
    if (m_opCode.startsWith("bit ") || m_opCode.startsWith("res ") ||  m_opCode.startsWith("set ")) {
  //      qDebug() <<"BIT before " <<m_opCode<<value << expr<<ol.m_orgLine;
        if (m_opCode.contains("+*)")) { // uh oh handle ix iy+*
            m_opCode = m_opCode.replace("**",QString(ol.m_orgLine.trimmed().simplified()[4]));
            value = value2;
            value2 = "";
        }
        else {
            m_opCode = m_opCode.split(" ").first()+ " " +QString::number(Util::NumberFromStringHex(value))+","+m_opCode.split(",")[1];
            value = "";
            expr = "";
        }

        type = OrgasmInstruction::none;
        if (m_opCode.contains("+*")) // contains ix+* etc
            type = OrgasmInstruction::imm;

    }
    // What a silly-looking opcode
    if (m_opCode.startsWith("rst")) {
//        qDebug() << "RST " << m_opCode<<value;
        if (value.startsWith("$"))
           m_opCode = "rst "+value.mid(1,3)+"h";
        else
            m_opCode = "rst "+value;

        if (pd!=OrgasmData::PASS_SYMBOLS)
            m_opCode = "rst 00h";
        value = "";
        expr = "";
        type = OrgasmInstruction::none;
    }
    if (m_opCode.startsWith("ret ")) {
        m_opCode = ol.m_orgLine.toLower().trimmed().simplified();
        value = "";
        expr = "";
        type = OrgasmInstruction::none;
    }
    // is out (c),0 really a thing at all?
    if (m_opCode.startsWith("out (c)") ) {
        if (value=="0")
            m_opCode = "out (c),0";
        value = "";
        expr = "";
        type = OrgasmInstruction::none;
    }
    m_opCode = m_opCode.toLower();

//    qDebug() <<"OP: " <<m_opCode <<value<<expr <<ol.m_orgLine;




    int code = 0;
    // Get actual opcode
    if (m_opCodes.contains(m_opCode))
        code = m_opCodes[m_opCode];
    else {
        throw OrgasmError("Unknown opcode: " + m_opCode,ol);
    }
//    if (code==0x3E)
  //     qDebug() << Util::numToHex(m_pCounter)<<  m_opCode << Util::numToHex(code) <<expr << value<<value2 << ol.m_orgLine << type;

    // calculate the parameter numeric values
    int val=0;
    int val2=0;
    bool hasValue2 = value2!="";

    if (type!=OrgasmInstruction::none) {
        QString num = value.replace("#","$").replace("(","").replace(")","");
        val = Util::NumberFromStringHex(Util::BinopString(num));
    }
    // Second strange value?
    if (hasValue2) {
        QString num = value2.replace("#","$").replace("(","").replace(")","");
        val2 = Util::NumberFromStringHex(Util::BinopString(num));
    }

    // Write opcode to data

    // 24-bit extra opcode
    bool is32bit = false;
    if (code>=0x10000) {
        data.append((code>>16)&0xff);
        is32bit = true;
    }
    // Append 16-bit opcode
    if (code>=0x100)
        data.append((code>>8)&0xff);

    // 8 bit part
    if (!is32bit)
       data.append(code&0xFF);


    // OPERANDS:

    // A relative jump? (jr)
    if (type==OrgasmInstruction::rel)  {
        // ooh this one might fail for relative jumps <$100.. is that a thing?
        if (val<=0xFF)
            data.append((uchar)val);
        else {
            // Get the relative address
            int diff = (val)-m_pCounter-2;
            //        qDebug() << " ****** RELATIVE: " <<m_opCode<<expr<< diff <<m_pCounter;
            if (abs(diff)>=128 && pd==OrgasmData::PASS_SYMBOLS) {
                throw OrgasmError("Branch out of range : " +QString::number(diff) + " :" + m_opCode + " " +expr + " on line " + QString::number(ol.m_lineNumber)+"<br>Please remember that you can use the keyword <b>'offpage'</b> in your block to let OrgAsm know that the code should perform off-page jumps.",ol);
            }
            data.append((uchar)diff);
        }
    }
    else
        // Immediate 8-bit vale?
        if (type==OrgasmInstruction::imm) {
            data.append(val);
            // Has additional data? only imm has this (some select few operations)
            if (hasValue2)
                data.append(val2);
        }
        else
            // 16-bit abs value?
            if (type==OrgasmInstruction::abs) {
                data.append(val&0xFF);
                data.append((val>>8)&0xFF);
            }
    if (pd != OrgasmData::PASS_SYMBOLS)
        expr = orgExpr;

    // 8 bit part in 32-bit code. For some strange reason, add it *after* the data. I guess this is how the CPU reads it.
    if (is32bit)
       data.append(code&0xFF);

    if (data.count()>0) {
        m_data.append(data);
        m_pCounter+=data.count();
    }
}


/*
 *
 * Load the z80 instruction codes from disk
 *
 *
*/
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
/*
 *
 * Washes parameter for opcode
 * for instance: (a) iy de etc are returned as is
 * (iy + #10) returned as "(iy+*)
 * anything else (**) or **
 *
*/
QString ZOrgasm::WashForOpcode(QString test, QString &value,OrgasmLine& ol)
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
    // return "(hl)";
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


