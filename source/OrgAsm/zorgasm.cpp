#include "zorgasm.h"

ZOrgasm::ZOrgasm()
{

}

void ZOrgasm::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
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
//        qDebug() << "ZORG " <<l2;
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
                if (tst==("<"+sym)) {
                    l2[0] = l2[0].replace("<"+sym,"$"+QString::number(i&0xFF,16));
                    break;
                }
                if (tst==(">"+sym)) {
                    l2[0] = l2[0].replace(">"+sym,"$"+QString::number((i>>8)&0xFF,16));
                    break;
                }

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

        qDebug() << "ORGASM " <<expr << org << ol.m_expr;

        if (expr!="" && !isRegister(org)) {
            long val = 0;
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




    OrgasmInstruction::Type type = OrgasmInstruction::imp;
    MOSOperandCycle cyc;

    //    if (opCode=="org")
    //      return false;

    if (expr.contains(",")) {
        QStringList lst = expr.split(",");
        QString a1 = lst[0].toLower().simplified().trimmed();
        QString a2 = lst[1].toLower().simplified().trimmed();
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


    }
    if (isRegister(expr)) {
        m_opCode +="<"+expr;
        expr = "";

    }
    expr = expr.remove("[");
    expr = expr.remove("]");
    qDebug() << "OPCODE  " <<m_opCode << expr;



    if (m_opCodes.m_opCycles.contains(m_opCode))
        cyc = m_opCodes.m_opCycles[m_opCode];
    else {
        if (m_opCode =="cpu")
            return; // Ignore
        throw OrgasmError("Unknown opcode: " + m_opCode,ol);
    }

    if (cyc.m_opcodes.count()==0)
        throw OrgasmError("Opcode illegal or not implemented yet: '" + m_opCode + "' on line '" + orgexpr+"'",ol);


    if (expr!="")
        type =  ol.m_instruction.getTypeFromParams(expr);
    else
        type = OrgasmInstruction::none;



    // Override types or pass symbol
    //    if (pass==OrgasmData::PASS_LABELS) {
    /*        if (m_opCode == "jmp" || m_opCode=="jsr")
                type = OrgasmInstruction::abs;
    */
    if ((int)type>cyc.m_opcodes.length())
        throw OrgasmError("Unknown or non-implemented opcode: " + m_opCode,ol);

    int code = cyc.m_opcodes[(int)type];


    if (code==0 && pd==OrgasmData::PASS_SYMBOLS) {
        qDebug() << "ERROR on line : " << m_opCode + " " +expr;
        throw OrgasmError("Opcode type not implemented or illegal: " + m_opCode + "  type " +QString::number(type) + "        on line " + ol.m_expr,ol );
    }



    int val=0;
    if (type!=OrgasmInstruction::none) {
        QString num = expr.split(",")[0].replace("#","").replace("(","").replace(")","");
        val = Util::NumberFromStringHex(num);
    }


    data.append(code);


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




    if (data.count()>0) {
        m_data.append(data);
        m_pCounter+=data.count();
    }
}
