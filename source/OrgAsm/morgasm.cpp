#include "morgasm.h"

MOrgasm::MOrgasm()
{
    m_codeReg.clear();
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

    m_pushPullReg["ccr"] = 1;
    m_pushPullReg["a"] = 2;
    m_pushPullReg["b"] = 4;
    m_pushPullReg["dpr"] = 8;
    m_pushPullReg["x"] = 16;
    m_pushPullReg["y"] = 32;
    m_pushPullReg["s"] = 64;
    m_pushPullReg["u"] = 64;
    m_pushPullReg["pc"] = 128;
    m_pushPullReg["d"] =  m_pushPullReg["a"] | m_pushPullReg["b"];


    m_lea["x"] = 8;
    m_lea["y"] = 0xa;
    m_lea["u"] = 0xc;
    m_lea["s"] = 0xe;
    m_lea["a"] = 6;
    m_lea["b"] = 5;
    m_lea["d"] = 0xb;
    m_lea["none"] = 0x4;
    m_lea["num"] = 0x9;
    m_lea["pc"] = 0xc;

    m_lda["s"] = 6;
    m_lda["u"] = 4;
    m_lda["x"] = 0;
    m_lda["y"] = 2;
    m_lda["pc"] = 8;


}



int MOrgasm::getTypeFromParams(OrgasmLine& ol)
{
    QString op = ol.m_instruction.m_opCode.toLower().simplified();
    QString s = ol.m_expr.simplified();
    if (op=="tfr" || op=="pshs" || op=="puls") {
        return Op6809::imm;
    }
    s=s.toLower().simplified();
    //        qDebug() << " Getting type from : " << s;

    if (s=="")
        return Op6809::inh;

    if (s.contains(",")) {
        return Op6809::ind;

    }


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
int MOrgasm::getLeaParams(OrgasmLine &ol, int& size)
{
    auto lst = ol.m_expr.simplified().toLower().split(",");
    int val = 0;
    size = 1;
    int shl=0;
    for (auto& s : lst)
        s = s.simplified().toLower();
    bool isNegative = 0;
    QString p1 = lst[0];
    QString p2 = lst[1];
    if (p1.startsWith("-"))
        isNegative = 1;
    if (m_lea.contains(p1))
        val |= m_lea[lst[0]];
    else {
        if (p1=="")
            val|=m_lea["none"];
        else {
            int shift = Util::NumberFromStringHex(p1);
            if (abs(shift)<16) {
                // Special case for s
                val=shift&15;
                return val | ((m_lda[p2]+isNegative)<<4);
            }
            if (abs(shift)<256) {
                // 8 bit num
                val=(val|(m_lea["num"]-1))<<8;
                shl=8;
                val|=(shift&0xFF);
                size+=1;
            }
            else
               {
                // 16 bit num
                    val=(val|(m_lea["num"]))<<16;
                    shl=16;
                    val|=(shift&0xFFFF);
                    size+=2;
                }
        }
        //        else
        //          throw OrgasmError("Incorrect lea: unknown parameter "+p1,ol);

    }
    if (m_lea.contains(p2))
        val |= (m_lea[p2]<<4) << shl;

    //    qDebug() << ol.m_orgLine << Util::numToHex(val);

    return val;
}

int MOrgasm::getLdaParams(OrgasmLine &ol, int &size)
{
    auto lst = ol.m_expr.simplified().toLower().split(",");
    int val = 0;
    size = 1;
    int shl=0;
    for (auto& s : lst)
        s = s.simplified().toLower();

    QString p1 = lst[0];
    QString p2 = lst[1];
    int isNegative = 0;
    if (p1.startsWith("-"))
        isNegative = 1;
//    if (ol.m_orgLine.contains("-1000"))
  //      qDebug() << "*** "<< ol.m_orgLine;

    int incType = 0;
    if (m_lea.contains(p1) && m_lea.contains(p2))
        return m_lea[p1] | (m_lea[p2]<<4);
    else {
        if (p1=="") {
            if (p2.contains("+")) incType = 1;
            if (p2.contains("-")) incType = 2;
            p2 = p2.remove("+").remove("-");
            // does it have + or -?
            if (incType==0)
                val|=m_lea["none"];
            if (incType==2)
                val|=2; // - opcode
            if (m_lea.contains(p2))
                val |= (m_lea[p2]<<4) << shl;
            return val;
        }

        else {
            val = 0;
            int shift = Util::NumberFromStringHex(p1);
            bool isNeg = shift<0;
    //        if (ol.m_orgLine.contains("-1000"))
      //          qDebug() << "*** "<<size << shift << p1;
            if (abs(shift)>=16 || p2=="pc") {
                size+=1;
                int an = 0xFFFF;
                int orand = 8;
                if (abs(shift)>=256) {
                    orand = 9;
                    size+=1;
                }
                else {
                    // 8 bit negative number
                    if (isNeg)
                        an = 0xff;

                }
                if (p2=="pc")
                    val = ((m_lea[p2]) | (orand)<<4)<<((size-1)*8);
                else
                    val = ((m_lea[p2]<<4) | (orand))<<((size-1)*8);
                val = val | (shift&an);
                if (ol.m_orgLine.contains("-1000"))
                    qDebug() << ol.m_orgLine << Util::numToHex(val) << p2 << size;

                return val;
            }

            if (m_lda.contains(p2))
                val |= m_lda[p2]<<4;

            //val=(val|m_lda["num"])<<16;
            //shl=16;
            val|=(shift&15);
            //size+=2;
        }
        //        else
        //          throw OrgasmError("Incorrect lea: unknown parameter "+p1,ol);

    }

    //    qDebug() << ol.m_orgLine << Util::numToHex(val);

    return val;

}


int MOrgasm::getParsedValue(OrgasmLine& ol, int& size, int type)
{
    QString expr = ol.m_expr.simplified();
    QString op = ol.m_instruction.m_opCode.toLower().simplified();
    if (op=="puls" || op=="pshs")
        return getPushPullParams(ol);


    if (op.startsWith("tfr"))
        return getRegisterCodeFromParams(expr);

    if (expr.contains(",")) {
        if (ol.m_instruction.m_opCode.contains("lea"))
            return getLeaParams(ol,size);
        return getLdaParams(ol,size);

    }

    if (m_symbolsList.contains(expr))
        return m_symbols[expr];

    if (m_passType == OrgasmData::PASS_SYMBOLS)
        for (auto& l:m_symbolsList) {
            if (expr.contains(l)) {
                expr = expr.replace(l,QString::number(m_symbols[l]));
                expr = expr.replace("#"+l,QString::number(m_symbols[l]));
            }
        }
    expr = Util::BinopString(expr);
    return Util::NumberFromStringHex(expr);

}

void MOrgasm::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
{
    //    QByteArray d = ol.m_instruction.Assemble(ol.m_expr, m_opCodes, pd, m_symbols, m_pCounter,m_constants, m_regs, m_symbolsList);
//    if (ol.m_orgLine.contains("ldb"))
  //      qDebug() <<ol.m_orgLine;
    QString expr = ol.m_expr;
    QByteArray data;
    ol.m_instruction.m_opCode = ol.m_instruction.m_opCode.toLower();
    QString op = ol.m_instruction.m_opCode.simplified().toLower();
    //    qDebug() << op;
    if (!m_instructions.contains(op))
        throw OrgasmError("Unknown instruction (opcode missing or not implemented yet. Bug Leuat!) : "+ol.m_orgLine.simplified(),ol);

    ol.m_expr.replace("*",Util::numToHex(m_pCounter));

    if (ol.m_instruction.m_opCode=="processor")
        return;
    //    qDebug() << line;

    if (ol.m_instruction.m_opCode=="org")
        return;

    int type = getTypeFromParams(ol);
    //    qDebug() << op<<type;
    //    if (type==Op6809::imm || type==Op6809::ext || type==Op6809::inh || type==Op6809::rel)
    Write(data,ol,type);
    //  else
    //    throw OrgasmError("Unknown instruction / opcode : "+ol.m_orgLine.simplified(),ol);



    m_data.append(data);
    m_pCounter+=data.length();

}

uchar MOrgasm::getRegisterCodeFromParams(QString s)
{
    auto lst = s.simplified().toLower().split(",");
    uchar c = 0;
    for (auto& s : lst)
        s = s.simplified();
    if (lst.size()>0 && m_codeReg.contains(lst[0]))
        c |= m_codeReg[lst[0]]<<4;
    if (lst.size()>1 && m_codeReg.contains(lst[1]))
        c |= (m_codeReg[lst[1]]);

    return c;
}

uchar MOrgasm::getPushPullParams(OrgasmLine&ol)
{
    QString op = ol.m_instruction.m_opCode.toLower().simplified();
    if (op!="puls" && op!="pshs")
        return 0;
    auto lst = ol.m_expr.simplified().toLower().split(",");
    uchar c = 0;
    for (QString s: lst) {
        s = s.simplified();
        if (!m_pushPullReg.contains(s))
            throw OrgasmError("Unknown pshs/puls register: "+s +" in expression ",ol);
        c |= m_pushPullReg[s];

    }
    return c;
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
    int ds = 1;
    if (code>=256) {
        data.append((code>>8)&0xFF);
        ds+=1;
    }
    data.append(code&0xFF);
    if (type==Op6809::inh)
        return;
    int val = 0;
    //if (m_passType==OrgasmData::PASS_SYMBOLS)
    {
        val = getParsedValue(l,size, type);
        if (type==Op6809::rel) {

            int add = ds+1;
            // accomodate for lbra opcode size
            if (l.m_instruction.m_opCode.startsWith("l"))
                add+=1;
            //                if (val<m_pCounter) add-=1;
            val-=m_pCounter+add;
        }
        //           qDebug() << Util::numToHex(val) << l.m_expr << code;
    }


    // tfr, puls,
    //    qDebug() << l.m_orgLine << size;
    WriteNumber(data, val, size);
}

void MOrgasm::WriteNumber(QByteArray &data, int val, int size)
{
    if (size>=3)
        data.append((val>>16)&0xFF);
    if (size>=2)
        data.append((val>>8)&0xFF);

    data.append((val)&0xFF);



}


