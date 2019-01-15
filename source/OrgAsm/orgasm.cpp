#include "orgasm.h"
#include <QRegularExpression>

bool OrgasmLine::m_inIFDEF = false;

void Orgasm::LoadFile(QString filename) {
    LoadCodes();
    QFile f(filename);
    f.open(QFile::ReadOnly);
    m_source=f.readAll();
    f.close();
    m_lines = m_source.split("\n");
    m_pCounter = 0;
    m_done = false;
    m_constantPassLines = -1;
}

void Orgasm::LoadCodes()
{
    QFile f(":/resources/text/opcodes.txt");
    f.open(QFile::ReadOnly);
    QString all = f.readAll();
    f.close();
    for (QString s: all.split("\n")) {
        s = s.trimmed().simplified().toLower();
        if (s=="") continue;
        if (s.startsWith("#")) continue;
        QStringList lst = s.split(",");
        QVector<uchar> opCodes;

        for (int i=0;i<11;i++) {
            bool ok;
            int code = lst[i+1].toInt(&ok, 16);
            //            qDebug() << "Adding : " << lst[0] << " " << Util::numToHex(code);
            opCodes.append( code );
        }
        MOSOperandCycle c = m_opCodes[lst[0]];
        c.m_opcodes = opCodes;
        m_opCodes.m_opCycles[lst[0]] = c;
    }


}

OrgasmLine Orgasm::LexLine(int i) {
    OrgasmLine l;
    QString line = m_lines[i];
    // First: Do we have alabel?
//    line = line.replace("//",";");
    line = line.replace("\t", " ");
    line = line.replace("dc.b", ".byte");
    line = line.replace("!by", ".byte");
    line = line.replace("!fi", ".byte");
    line = line.replace("dc.w", ".word");
    //line = line.replace("ds.w", ".word");
 //   line = line.replace("equ", "=");
//    line = line.replace("EQU", "=");
    //line = line.replace("ds.b", ".byte");
    line = line.replace(".dc ", ".byte ");
    line = line.split(";")[0];
    if (!line.toLower().contains("incbin"))
        line = line.split("//")[0];

    l.m_orgLine = line;

//    if (line.toLower().contains("else"))
  //      qDebug() << line;

    if (line.simplified().toLower().startsWith("ifconst")) {
        OrgasmLine::m_inIFDEF= true;
        l.m_ignore = true;
        return l;

    }

    if (line.simplified().toLower().startsWith("ifnconst")) {
        OrgasmLine::m_inIFDEF= false;
        l.m_ignore = true;
        return l;

    }


    if (line.simplified().toLower().startsWith("endif")) {
        OrgasmLine::m_inIFDEF= false;
        l.m_ignore = true;
        return l;

    }

    if (line.simplified().toLower() == ("else")) {
        OrgasmLine::m_inIFDEF= !OrgasmLine::m_inIFDEF;
        l.m_ignore = true;
        return l;

    }

    if (OrgasmLine::m_inIFDEF) {
        l.m_ignore = true;
        return l;

    }

    if (!(line[0]==" ") && !line.contains("=")) {
        QString lbl = line.replace(":", " ").trimmed().split(" ")[0];
        l.m_label = lbl;
//        qDebug() << line;

        line.remove(0, lbl.length()).replace(":", " ");
  //      qDebug() << line;

    }


    if (line.simplified().startsWith(";") || line.simplified()=="" || line.simplified().startsWith("//")) {
        if (l.m_label=="")
            l.m_ignore = true;
        else l.m_type = OrgasmLine::LABELONLY;
        return l;
    }


    QStringList lst = line.simplified().split(" ");


    if (lst[0].toLower()=="org") {
        l.m_type = OrgasmLine::ORG;
        l.m_expr = "";
        for (int i=0;i<lst.count()-1;i++)
           l.m_expr += lst[i+1] + " ";


        return l;
    }
    if (lst[0].toLower()=="incbin") {
        l.m_type = OrgasmLine::INCBIN;
        l.m_expr = lst[1];
        return l;
    }
    if (line.contains("=")) {
        l.m_type = OrgasmLine::CONSTANT;
        QStringList cl = line.split("=");
        l.m_label = cl[0].trimmed();
        l.m_expr = cl[1].trimmed();
        //qDebug() << l.m_expr;
        return l;
    }
    if (lst[0].toLower()==".byte") {
        l.m_type = OrgasmLine::BYTE;
        l.m_expr = line.replace(".byte", "").trimmed();//.simplified();
        return l;
    }
    if (lst[0].toLower()=="ds.b") {
        l.m_type = OrgasmLine::PADBYTE;
        l.m_expr = line.replace("ds.b", "").trimmed();//.simplified();
        return l;
    }
    if (lst[0].toLower()==".word") {
        l.m_type = OrgasmLine::WORD;
        l.m_expr = line.replace(".word", "").trimmed();//.simplified();
        return l;
    }


    l.m_type = OrgasmLine::INSTRUCTION;
    l.m_instruction.Init(lst);
/*    if (lst.count()>1)
        l.m_expr = lst[1];*/
    l.m_expr="";
    for (int i=0;i<lst.count()-1;i++)
        l.m_expr +=lst[i+1] + " ";

    //        l.m.l.m_instruction.getTypeFromParams(lst[1]);
    //      l.m_op



    return l;
}
/*
bool stringSort(const QString &s1, const QString &s2)
{
    return s1.count() < s2.count();
}
*/
bool Orgasm::Assemble(QString filename, QString outFile)
{
    m_success = false;
    LoadFile(filename);
    m_olines.clear();

    try {


    for (int i=0;i<m_lines.count();i++) {
        OrgasmLine ol = LexLine(i);
        if (!ol.m_ignore)
            m_olines.append(ol);
    }


    PassFindConstants();
    PassReplaceConstants();
//       for (QString s: m_constants.keys())
  //         qDebug() << s << " " << m_constants[s];
   //     exit(1);
/*    while (m_constantPassLines!=0) {
        PassConstants();
        qDebug() << "PAssing" << m_constantPassLines;
    }
*/
    QTime myTimer;
    myTimer.start();
//    qDebug() << "LABELS  " << QString::number((myTimer.elapsed()/100.0));
    Compile(OrgasmData::PASS_LABELS);
  //  qDebug() << "DONE " << QString::number((myTimer.elapsed()/100.0));



    //qDebug() << "SYMBOLS: " << QString::number((myTimer.elapsed()/100.0));
    Compile(OrgasmData::PASS_SYMBOLS);
    //qDebug() << "DONE " << QString::number((myTimer.elapsed()/100.0));


/*    for (OrgasmLine& ol:m_olines) {
        qDebug() << "                                         org: "<<ol.m_orgLine;
        QString pos =Util::numToHex(ol.m_pos);
        if (ol.m_type==OrgasmLine::INSTRUCTION)
            qDebug() <<pos << ol.m_instruction.m_opCode << " " << ol.m_expr;
        if (ol.m_type==OrgasmLine::ORG)
            qDebug() <<pos << "org" << ol.m_expr;
        if (ol.m_type==OrgasmLine::CONSTANT)
            qDebug() <<pos << ol.m_label<< " = " << ol.m_expr;
        if (ol.m_type==OrgasmLine::INCBIN)
            qDebug() <<pos << "incbin " << ol.m_expr;
        if (ol.m_type==OrgasmLine::BYTE)
            qDebug() <<pos << ".byte " << ol.m_expr;
    }
*/
    m_success = true;
    if (QFile::exists(outFile))
        QFile::remove(outFile);
    QFile out(outFile);
    out.open(QFile::WriteOnly);
    out.write(m_data);
    out.close();
    m_output = "Complete.";


    }
    catch (QString e) {
        //qDebug() << "Error compiling: " << e;
        m_output = "Error during OrgAsm assembly: "+e;
    }
    return m_success;
}

void Orgasm::PassReplaceConstants()
{
    //    if (m_constantPassLines==0)
    for (OrgasmLine& ol : m_olines) {
        if (ol.m_type != OrgasmLine::CONSTANT) {
            for (QString k : m_constList) {
                if (ol.m_expr.contains(k))
                ol.m_expr =  OrgasmData::ReplaceWord(ol.m_expr, k, m_constants[k]);
            }

        }
    }

}



void Orgasm::PassFindConstants()
{
//    if (m_constantPassLines==0)
  //      return;
    m_constantPassLines = 0;
    for (OrgasmLine& ol : m_olines) {
        if (ol.m_type == OrgasmLine::CONSTANT) {
//            qDebug() << ol.m_label;
            if (m_constants.contains(ol.m_label))
                continue;
            m_constants[ol.m_label] = ol.m_expr;
            m_constList.append(ol.m_label);

            for (QString key : m_constants.keys()) {
                ol.m_expr.replace(key, m_constants[key]);
            }

            int var = 0;
            //qDebug() << ol.m_expr;
            OrgasmData::BinopExpr(ol.m_expr,var,"");


            /*bool ok = Util::NumberFromStringHex(ol.m_expr, var);
            if (ok) {
                m_symbols[ol.m_label] = var;
            }
            else {
                m_constantPassLines++;
            }
            */
/*            qDebug() << ol.m_label << " = " << ol.m_expr;
            if (m_symbols.contains(ol.m_label))
                qDebug() << "NOW DEFINED : " << Util::numToHex(m_symbols[ol.m_label]);
*/
            //m_symbols[ol.m_label.simplified()] = ol.m_expr.simplified();
            //m_symbols[ol.m_label.simplified()] = ol.m_expr.simplified();
        }
    }

}

void Orgasm::Compile(OrgasmData::PassType pt)
{
    m_pCounter = 0;
//    return;

    m_data = QByteArray();
    for (OrgasmLine& ol : m_olines) {
        if (ol.m_type == OrgasmLine::CONSTANT)
            continue;

        if (ol.m_label!="" && pt == OrgasmData::PASS_LABELS) {
//            qDebug() << "Writing to label: " << ol.m_label << Util::numToHex(m_pCounter);
             m_symbols[ol.m_label] = m_pCounter;
             m_symbolsList.append(ol.m_label);
        }
        ol.m_pos = m_pCounter;


//        qDm_pCounter

        if (ol.m_type==OrgasmLine::BYTE)
            ProcessByteData(ol,pt);

        if (ol.m_type==OrgasmLine::PADBYTE)
            ProcessBytePad(ol);

        if (ol.m_type==OrgasmLine::WORD)
            ProcessWordData(ol);

        if (ol.m_type==OrgasmLine::ORG)
            ProcessOrgData(ol);

        if (ol.m_type==OrgasmLine::INSTRUCTION)
            ProcessInstructionData(ol, pt);

        if (ol.m_type==OrgasmLine::INCBIN)
            ProcessIncBin(ol);


//        qDebug() << "End: "<< ol.m_instruction.m_opCode << ol.m_expr  << Util::numToHex(m_pCounter);

    }


}

void Orgasm::ProcessByteData(OrgasmLine &ol,OrgasmData::PassType pt)
{
    if (ol.m_expr.trimmed()=="") {
        m_pCounter++;
        m_data.append((char)0x00);
        return;
    }
//    qDebug() << ol.m_expr;



    QStringList lst = ol.m_expr.split(",");
//    qDebug() << lst;
    for (QString s: lst) {

        if (s.trimmed()=="") continue;
  //      qDebug() << Util::NumberFromStringHex(s);
        if (!s.contains("\"")) {

            if (pt==OrgasmData::PASS_SYMBOLS) {
    //            qDebug() << "Before "<< s;
            for (QString& c: m_symbolsList)
                if (s.contains(c)) {
                    //qDebug() << c;
                    //exit(1);
                    s = OrgasmData::ReplaceWord(s,c,Util::numToHex(m_symbols[c]));

                }
  //              qDebug() << "After: "<<s;

                s = Util::numToHex(Util::NumberFromStringHex(s));
//                qDebug() << "After2: "<<s;

            }


//            if (lst.count()>1)  qDebug() << "Adding: " <<  Util::NumberFromStringHex(s);
            m_data.append(Util::NumberFromStringHex(s));

//            qDebug() << Util::NumberFromStringHex(s);
            //qDebug() << s << Util::NumberFromStringHex(s);
            m_pCounter++;

        }
        else {
            QString str = s;
            str = str.remove("\"");
            for (int i=0;i<str.length();i++) {
                int c = str.at(i).toLatin1();
//                if (c>)
//                qDebug() << c << QChar(c);
                m_data.append((uchar)c);
                m_pCounter++;

            }
  //          exit(1);

        }
    }
    //    qDebug() << "Final val added: " << QString::number((uchar)m_data[m_data.count()-1]);
}

void Orgasm::ProcessBytePad(OrgasmLine &ol)
{
    QStringList lst = ol.m_expr.simplified().split(" ");

//    qDebug() << lst;
  //


//    qDebug() << lst;
    //qDebug() << ol.m_expr;
    int v = 0;
    if (lst.count()==2)
        v = Util::NumberFromStringHex(lst[1]);

    int cnt = Util::NumberFromStringHex(lst[0]);
    for (int i=0;i<cnt;i++) {
        m_pCounter++;
        m_data.append((char)v);

    }

}

void Orgasm::ProcessWordData(OrgasmLine &ol)
{
    if (ol.m_expr=="") {
        m_pCounter+=2;
        m_data.append((char)0x00);
        m_data.append((char)0x00);
        return;
    }
    QStringList lst = ol.m_expr.split(",");
    for (QString s: lst) {
        if (s.trimmed()=="") continue;
        if (m_symbolsList.contains(s)) {
            m_data.append(m_symbols[s]&0xFF);
            m_data.append((m_symbols[s]>>8)&0xFF);
        }
        else {
            m_data.append(Util::NumberFromStringHex(s)&0xFF);
            m_data.append(Util::NumberFromStringHex(s)>>8);
        }
        m_pCounter++;
        m_pCounter++;
    }
}

void Orgasm::ProcessOrgData(OrgasmLine &ol)
{
    if (m_pCounter == 0) {
        int val = Util::NumberFromStringHex(ol.m_expr);
        m_data.append(val&0xFF);
        m_data.append((val>>8)&0xFF);
        m_pCounter = val;
        return;
    }
    else {


        for (QString s: m_symbols.keys())
                ol.m_expr = OrgasmData::ReplaceWord(ol.m_expr,s,Util::numToHex(m_symbols[s]));

        ol.m_expr =ol.m_expr.replace("#","");

        int val = Util::NumberFromStringHex(Util::BinopString(ol.m_expr));
        if (val<m_pCounter) {
            QString e = "Origin reversed index. Trying to move program counter backwards to '"+Util::numToHex(val)+"' from current counter " + Util::numToHex(m_pCounter)+". ";
            e += "Please make sure that your included data does not overlap, and make use of the memory analyzer tool.";
            throw QString(e);
        }

        while (m_pCounter<val) {
            m_pCounter++;
            m_data.append((uchar)0xff);
        }
    }
}

void Orgasm::ProcessIncBin(OrgasmLine &ol)
{
    //qDebug() << ol.m_expr;
    QFile f(ol.m_expr.replace("\"",""));
    f.open(QFile::ReadOnly);
    QByteArray data = f.readAll();
    f.close();
    m_data.append(data);
    m_pCounter+=data.count();


}

void Orgasm::ProcessInstructionData(OrgasmLine &ol, OrgasmData::PassType pd)
{
//    QByteArray d = ol.m_instruction.Assemble(ol.m_expr, m_opCodes, pd, m_symbols, m_pCounter,m_constants, m_regs, m_symbolsList);

    QString expr = ol.m_expr;
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
//        qDebug() << expr;

        QStringList l2 = expr.simplified().split(",");

        for (QString& c : m_constList) {
            l2[0] = OrgasmData::ReplaceWord(l2[0],c,m_constants[c]);

        }

//        qDebug() << "***" << l2;

        QString tst = l2[0];
        QString org = l2[0];
        tst = tst.replace("#", "").simplified();
//        qDebug() << symbols.keys().count();
        //for (QString& sym : symbols.keys())
        int cur = 0;
        for (QString& sym : m_symbolsList)
        {
//            qDebug() << sym;
            if (!tst.contains(sym))
                continue;
            int i = m_symbols[sym];
            if (tst==("<"+sym)) {
                l2[0] = l2[0].replace("<"+sym,"$"+QString::number(i&0xFF,16));
                break;
            }
            else
                if (tst==(">"+sym)) {
                    l2[0] = l2[0].replace(">"+sym,"$"+QString::number((i>>8)&0xFF,16));
                    break;
                }

//            if (tst.contains(sym)) {

/*                std::regex e ("\\b" +sym.toStdString()+ "\\b");   // matches words beginning by "sub"
                QString l = "$"+QString::number(i,16);
                 l2[0] = QString::fromStdString(std::regex_replace (l2[0].toStdString(),e,l.toStdString()));
*/
 //               l2[0] = l2[0].replace(*exp[sym],"$"+QString::number(i,16));
//}
                l2[0] = OrgasmData::ReplaceWord(l2[0],sym,"$"+QString::number(i,16));
                //l2[0] = l2[0].replace(sym,"$"+QString::number(i,16));
                if (org!=l2[0]) {
                    m_symbolsList.move(cur,0);
                    break;
                }
                cur++;
        }

        expr = l2[0];
        if (l2.count()>1)
            expr+=","+l2[1];


        if (expr!="") {
            int val = 0;
            QString repl = "$1000";
            if (expr.contains("<") || expr.contains(">"))
                repl= "#$10";

            expr = expr.replace("<","").replace(">","");
//            qDebug() << "A" <expr;
            if (!(expr.startsWith("(") && expr.endsWith(")"))) {
 //               expr = OrgasmData::BinopExpr(expr, val, repl);
                bool hasHash = false;
                if (expr.simplified().startsWith("#"))
                    hasHash = true;
                expr = OrgasmData::BinopExpr(expr, val, repl);
                if (hasHash)
                    expr = "#"+expr;
            }

/*            if (orgExpr.toUpper().contains("5")) {
                qDebug() << "org: " << orgExpr;
                qDebug() << "l2: " << l2;
                qDebug() << "expr: " << expr;
    //            exit(1);
            }
*/

  //          qDebug() << "B" <<expr;
            if (val==-1 && pd==OrgasmData::PASS_SYMBOLS) {
                //qDebug() << l2;
                //qDebug() << ol.m_instruction.m_opCode<<  orgExpr;
                throw QString("Unknown operation: " +orgExpr);
            }

        }
    }





//    qDebug() << " ** " << line;

    QString m_opCode = ol.m_instruction.m_opCode;




    OrgasmInstruction::Type type = OrgasmInstruction::imp;
    MOSOperandCycle cyc;

//    if (opCode=="org")
  //      return false;

    if (m_opCodes.m_opCycles.contains(m_opCode))
        cyc = m_opCodes.m_opCycles[m_opCode];
    else {
//        qDebug() << expr;
        throw QString("Uknown opcode: " + m_opCode);
    }

    if (cyc.m_opcodes.count()==0)
        throw QString("Opcode not implemented yet: " + m_opCode);


//    qDebug() << "BEFORE: " << expr;
    if (expr!="")
        type =  ol.m_instruction.getTypeFromParams(expr);
    else
        type = OrgasmInstruction::none;

//    qDebug() << type;


    // Override types or pass symbol
//    if (pass==OrgasmData::PASS_LABELS) {
        if (m_opCode == "jmp" || m_opCode=="jsr")
            type = OrgasmInstruction::abs;

//        if (opCode == "ldy" )
//    }

    int code = cyc.m_opcodes[(int)type];
    if (code==0 && pd==OrgasmData::PASS_SYMBOLS) {
        qDebug() << "ERROR on line : " << m_opCode + " " +expr;
        throw QString("Opcode type not implemented yet: " + m_opCode + "  type " +type );
    }



    int val=0;
    if (type!=OrgasmInstruction::none) {
        QString num = expr.split(",")[0].replace("#","").replace("(","").replace(")","");
        //qDebug() << " NUM " << num << "  with type " << type;
        val = Util::NumberFromStringHex(num);
//        qDebug() << "EXPR: " << expr << val << " with code "<< Util::numToHex(code);
    }
//    qDebug() << cyc.m_name;
  //  qDebug() << cyc.m_opcodes;
//    m_debug << (" I : " + opCode + " type: " + (int)type  + ":" +Util::numToHex(code) + "    value : " + Util::numToHex(val)) ;


    data.append(code);


    if (m_opCode=="bpl" || m_opCode=="bne" || m_opCode=="beq" || m_opCode=="bcc" || m_opCode=="bcs" || m_opCode=="bvc" || m_opCode=="bmi" || m_opCode=="bvc" || m_opCode=="bvs") {
        int diff = (val)-m_pCounter-2;
  //      qDebug() << QString::number(diff);
        if (abs(diff)>=128 && pd==OrgasmData::PASS_SYMBOLS) {
            throw QString("Branch out of range : " +QString::number(diff));
        }
//        qDebug() << "Diff: " << Util::numToHex((uchar)diff);
        data.append((uchar)diff);
    }
    else

    if (type==OrgasmInstruction::zp || type==OrgasmInstruction::zpx || type==OrgasmInstruction::zpy || type == OrgasmInstruction::izx || type == OrgasmInstruction::izy || type==OrgasmInstruction::imm) {
        data.append(val);
    }
    else
    if (type==OrgasmInstruction::abs || type==OrgasmInstruction::abx || type == OrgasmInstruction::aby || type == OrgasmInstruction::ind) {
        data.append(val&0xFF);
        data.append((val>>8)&0xFF);
    }
    if (pd != OrgasmData::PASS_SYMBOLS)
        expr = orgExpr;

//    return data;



    if (data.count()>0) {
        m_data.append(data);
        m_pCounter+=data.count();
    }
}











QString OrgasmData::ReplaceWord(QString& line, QString& word, QString replacement)
{
    if (line==word)
        return replacement;
    if (!line.contains(word))
        return line;

    QRegularExpression rg("\\b" + word + "\\b");
    return line.replace(rg,replacement);
}

QString OrgasmData::BinopExpr(QString& expr, int& val, QString rep)
{
    bool hasHash = false;
/*    if (expr.contains("#")) {
//        qDebug() << expr;
        hasHash = true;
        expr = expr.replace("#","");
    }*/
    QStringList l = expr.simplified().split(",");
    l[0] = l[0].replace("(","").replace(")","");
    l[0] = l[0].replace(" ","");
    //sym = sym.toLower();
//                qDebug() << "bf: " << l2[0] << "          expr: " << expr;
    l[0] =Util::BinopString(l[0]);


    if(!Util::NumberFromStringHex(l[0],val)) {
        val=-1;
        if (rep!="")
            l[0] = rep;
    }

    QString pa="";
    QString pb="";
    if (expr.contains("(")) {
        pa="(";
        pb=")";
    }
    expr = pa+l[0]+pb;
    if (l.count()>1)
        expr+="," + l[1];


/*    if (hasHash)
        expr = "#"+expr;*/
    return expr;
}
