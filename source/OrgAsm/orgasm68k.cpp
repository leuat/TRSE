#include "orgasm68k.h"

OrgAsm68k::OrgAsm68k()
{
    m_regs.clear();

    isLittleEndian = false;

}

int OrgAsm68k::getTypeFromParams(OrgasmLine& ol)
{
    QString op = ol.m_instruction.m_opCode.toLower().simplified();
    QString s = ol.m_expr.simplified();
    return 0;
}

int OrgAsm68k::getParsedValue(OrgasmLine& ol, int& size, int type)
{
    QString expr = ol.m_expr.simplified();
    QString op = ol.m_instruction.m_opCode.toLower().simplified();
    return getParsedInt(expr);
}

int OrgAsm68k::getParsedInt(QString expr)
{
//    if (m_passType == OrgasmData::PASS_SYMBOLS)
    std::sort(m_symbolsList.begin(), m_symbolsList.end(), [](QString a, QString b)
                                     {
                                         return a.length() > b.length();
                                     });
bool d = false;

        for (auto& l:m_symbolsList) {
            if (expr.contains(l)) {
                int num = 0x1000; // some temp 16-bit number
                if (m_passType == OrgasmData::PASS_SYMBOLS)
                    num = m_symbols[l];
                auto oe = expr;

                expr = expr.replace(l,QString::number(num));
                expr = expr.replace("#"+l,QString::number(num));
//                if (oe.contains("Screen_bytetbl1"))
  //                  qDebug() << expr << l;

            }
        }

    expr = Util::BinopString(expr);
    return Util::NumberFromStringHex(expr);

}

void OrgAsm68k::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
{
    //    QByteArray d = ol.m_instruction.Assemble(ol.m_expr, m_opCodes, pd, m_symbols, m_pCounter,m_constants, m_regs, m_symbolsList);
//    if (ol.m_orgLine.contains("ldb"))
  //      qDebug() <<ol.m_orgLine;
    QString expr = ol.m_expr;
    QByteArray data;
    ol.m_instruction.m_opCode = ol.m_instruction.m_opCode.toLower();
    QString op = ol.m_instruction.m_opCode.simplified().toLower();
    //    qDebug() << op;
    if (!m_instructions.contains(op) && op!="=")
        throw OrgasmError("Unknown instruction (opcode missing or not implemented yet. Bug Leuat!) : "+ol.m_orgLine.simplified(),ol);
    bool b = false;
/*    if (ol.m_expr.contains("*")) {
        qDebug() << ol.m_expr;
        b= true;
    }*/
/*    if (expr.contains("*")) {
        qDebug() << expr;
    }
*/
        if (expr.contains("**")) {
        //        line = line.replace("*", Util::numToHex(pCounter));
        QString add = expr;//expr.simplified().split(" ")[1].replace(" ", "");
        add = add.replace("**", "*"+Util::numToHex(m_pCounter));
        add = Util::BinopString(add);
        expr = add;//expr.split(" ")[0] + " " + add;
        //        qDebug() << "* : " << expr;
        qDebug() << "A1" <<expr;

    }
        if (expr.trimmed()=="*") {
        qDebug() << "B0 "<< expr;
             expr = expr.replace("*", Util::numToHex(m_pCounter));
            qDebug() << "B1 "<< expr;
             qDebug() <<ol.m_instruction.m_opCode;

    }


    ol.m_expr.replace("*",Util::numToHex(m_pCounter));
/*    if (b)
        qDebug() << ol.m_expr;*/
    if (ol.m_instruction.m_opCode=="processor")
        return;
    //    qDebug() << line;

    if (ol.m_instruction.m_opCode=="org")
        return;


    if (ol.m_instruction.m_opCode!="=") {
        int type = getTypeFromParams(ol);
    //    qDebug() << op<<type;
    //    if (type==Opm68k::imm || type==Opm68k::ext || type==Opm68k::inh || type==Opm68k::rel)
        Write(data,ol,type);
    }
    else {
        int val = Util::NumberFromStringHex(Util::BinopString(expr));
        data.append(val>>8);
        data.append(val&255);

        qDebug() << "HERE";
        qDebug() << expr << QString::number(val,16);

    }
    //  else
    //    throw OrgasmError("Unknown instruction / opcode : "+ol.m_orgLine.simplified(),ol);



    m_data.append(data);
    m_pCounter+=data.length();

}




/*
 *
 * Load the 6809 instruction codes from disk
 *
 *
*/
void OrgAsm68k::LoadCodes(int CPUflavor)
{


    auto lst = Util::loadTextFile(":/resources/text/opcodes_m68k.txt").split("\n");


    m_instructions.clear();
    for (QString s: lst) {
        s = s.trimmed().simplified().toLower();
        if (s=="") continue;
        if (s.startsWith("#")) continue;

        QStringList lst = s.split(" ");
        Opm68k *op = new Opm68k;
        op->name = lst[1].simplified();
        op->pattern = lst[0].simplified();
        op->size = 2;
        m_instructions[op->name] = QSharedPointer<Opm68k>(op);
    }


}

Opm68k* OrgAsm68k::getOpcode(OrgasmLine&ol) {
    QString code = ol.m_instruction.m_opCode.toLower();
    if (!m_instructions.contains(code))
        throw OrgasmError("Instruction '"+code+"' unrecognised or not implemented.",ol);
    return m_instructions[code].get();
}

void OrgAsm68k::Write(QByteArray &data, OrgasmLine &l, int type) {
    Opm68k* op = getOpcode(l);
    int val = 0;
    int size = 2;
/*    int code = op->opcodes[type];
    int size = op->size[type];
    int ds = 1;
    if (code>=256) {
        data.append((code>>8)&0xFF);
        ds+=1;
    }
    data.append(code&0xFF);
    if (type==Opm68k::inh)
        return;
    int val = 0;
    //if (m_passType==OrgasmData::PASS_SYMBOLS)
    {
        val = getParsedValue(l,size, type);
        if (type==Opm68k::rel) {

            int add = ds+1;
//            qDebug() << l.m_orgLine << QString::number(val,16) << QString::number(code,16);
            // accomodate for lbra opcode size
            if (l.m_instruction.m_opCode.startsWith("l"))
                add+=1;
            //                if (val<m_pCounter) add-=1;
            val-=m_pCounter+add;
        }
        //           qDebug() << Util::numToHex(val) << l.m_expr << code;
    }
*/

    // tfr, puls,
    //    qDebug() << l.m_orgLine << size;
    WriteNumber(data, val, size);
}

void OrgAsm68k::WriteNumber(QByteArray &data, int val, int size)
{
    if (size>=3)
        data.append((val>>16)&0xFF);
    if (size>=2)
        data.append((val>>8)&0xFF);

    data.append((val)&0xFF);



}


