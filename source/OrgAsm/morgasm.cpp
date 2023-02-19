#include "morgasm.h"

MOrgasm::MOrgasm()
{
    m_codeReg.clear();
    m_codeReg["d"] = 0b0000;
    m_codeReg["x"] = 0b0001;
    m_codeReg["y"] = 0b0010;
    m_codeReg["u"] = 0b0011;
    m_codeReg["s"] = 0b0100;
   m_codeReg["pc"] = 0b0101;
    m_codeReg["w"] = 0b0110;
    m_codeReg["v"] = 0b0111;
    m_codeReg["a"] = 0b1000;
    m_codeReg["b"] = 0b1001;
    m_codeReg["cc"] = 0b1010;
    m_codeReg["dp"] = 0b1011;
    m_codeReg["e"] = 0b1110;
    m_codeReg["f"] = 0b1111;
}

QString MOrgasm::Process(QString s, OrgasmLine& ol)
{
//    qDebug() << "Processing : " <<s;
    QString expr = s.trimmed();
    if (expr.trimmed()=="")
        return "";

    if (m_passType==OrgasmData::PASS_LABELS) {

    }

/*    expr = expr.replace("[","(").replace("]",")");
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
    */
    if (m_passType==OrgasmData::PASS_SYMBOLS) {
        QString tst = expr;
        QString org = expr;
        int cur = 0;
        bool ok =  false;
    //      tst = tst.replace("(","");
      //    tst = tst.replace(")","");



    }
    return expr;
}
int MOrgasm::getTypeFromParams(QString op, QString s)
{
    if (s=="tfr") return Op6809::imm;
    s=s.toLower().simplified();
    //        qDebug() << " Getting type from : " << s;

    if (s=="")
        return Op6809::inh;
    if (s.startsWith("#"))
        return Op6809::imm;


    if (m_instructions[op]->opcodes[Op6809::rel]!=0)
        return Op6809::rel;

    return Op6809::ext;

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
    QByteArray data;
    ol.m_instruction.m_opCode = ol.m_instruction.m_opCode.toLower();
    QString op = ol.m_instruction.m_opCode.simplified().toLower();

    if (ol.m_instruction.m_opCode=="processor")
        return;
//    qDebug() << line;

    if (ol.m_instruction.m_opCode=="org")
        return;

    int type = getTypeFromParams(op,expr);
//    qDebug() << op<<type;
    if (type==Op6809::imm || type==Op6809::ext || type==Op6809::inh || type==Op6809::rel)
        Write(data,ol,type);
    else
        throw OrgasmError("Unknown instruction / opcode : "+ol.m_orgLine.simplified(),ol);



    m_data.append(data);
    m_pCounter+=data.length();

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

        for (int i=0;i<((lst.count()-1)/2);i++) {
            bool ok;
            int code = lst[i*2+1].toInt(&ok, 16);
            int size = lst[i*2+2].toInt(&ok, 16);
            op->opcodes.append( code );
            op->size.append(size);
        }
 //       qDebug() << op->name << op->opcodes;
        m_instructions[op->name] = QSharedPointer<Op6809>(op);
    }


}

 Op6809* MOrgasm::getOpcode(OrgasmLine&ol) {
    QString code = ol.m_instruction.m_opCode.toLower();
    if (!m_instructions.contains(code))
        throw OrgasmError("Instruction '"+code+"' unrecognised or not implemented.",ol);
    return m_instructions[code].get();
}

void MOrgasm::Write(QByteArray &data, OrgasmLine &l, int type) {
    Op6809* op = getOpcode(l);
    int code = op->opcodes[type];
    int size = op->size[type];
    if (code>=256)
        data.append((code>>8)&0xFF);
    data.append(code&0xFF);
    if (type==Op6809::inh)
        return;
    int val = 0;
    if (m_passType==OrgasmData::PASS_SYMBOLS) {
        if (m_symbolsList.contains(l.m_expr.simplified())) {
            val = m_symbols[l.m_expr.simplified()];
            if (type==Op6809::rel) {
                val-=m_pCounter+3;
            }
 //           qDebug() << Util::numToHex(val) << l.m_expr << code;
        }

        else
            val = Util::NumberFromStringHex(l.m_expr);
    }
    WriteNumber(data, val, size);
 }

void MOrgasm::WriteNumber(QByteArray &data, int val, int size)
{
    if (size>=2)
        data.append((val>>8)&0xFF);
    data.append((val)&0xFF);



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


