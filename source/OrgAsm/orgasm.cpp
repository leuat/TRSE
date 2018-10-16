#include "orgasm.h"

OrgAsm::OrgAsm()
{
    m_opCodes.InitMosOpCycles();
//    LoadCodes();
   /// Connect the cycles

}

void OrgAsm::LoadCodes()
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

void OrgAsm::Debug(QString s)
{
    m_debug<<Util::numToHex(m_pCounter)+ " : " +s;
}

QString OrgAsm::TreatLabel(QString line)
{
//    line=line.toLower();
    if (! (line.startsWith("\t") || (line.startsWith(" ")) )) {
       // a : dc.b 0
       line.replace(":"," ");

       QStringList lblSplit = line.simplified().split(" ");
       QString lbl = lblSplit[0];
       if (lbl!="") {
            m_symbols[lbl] = m_pCounter;

            Debug("New symbol '" + lbl+"' at " + Util::numToHex(m_pCounter) );


            //line = line.replace(lbl, " ");
            line = " ";
            if (lblSplit.count()>1)
                for (int i=1;i<lblSplit.count();i++)
                    line+=lblSplit[i] + " ";

//            qDebug() << "New label: " << lbl<< " starts with line " << line;
       }
    }

    return line.simplified().trimmed();

}

void OrgAsm::TreatOrg(QStringList l)
{
    qDebug() << l[0] << " " << l[1];
//    if (l.count()>2)
  //      qDebug() << l[2];
    QString v = l[1];
    if (l.count()>2)
        v+=" "+l[2];
    for (QString s: m_symbols.keys()) {
        if (v.contains(s))
            v = v.replace(s, Util::numToHex(m_symbols[s]));
    }

    //qDebug() << v;

//    int val = Util::NumberFromStringHex(l[1]);
    int val = Util::NumberFromStringHex(Util::BinopString(v));
  //  qDebug() << Util::numToHex(val);

    if (m_data.count()==0) {
        m_data.append(val&0xFF);
        m_data.append((val>>8)&0xFF);
        m_pCounter = val;
        Debug("First org: " + Util::numToHex(val));


    }
    else {
        int cnt =0;
        while (m_pCounter<val) {
            m_data.append((uchar)0xFF);
            m_pCounter++;
            cnt++;

        }
        Debug("Org padding : " + Util::numToHex(cnt) +" bytes to " + Util::numToHex(val));
//        qDebug() << "Org padding : " + Util::numToHex(cnt) +" bytes to " + Util::numToHex(val);

    }

}

void OrgAsm::TreatData(QString s)
{
    qDebug() << s;
    QStringList lst = s.replace(",", " ").simplified().split(" ");
    bool isWord = false;
    if (s.simplified().split(" ")[0]==".word") {
        isWord = true;
  //      qDebug() << " ****** IS WORD ******* " << s;
    }

    if (lst.count()==1) {
        m_data.append((uchar)0);
        if (isWord) {
//            qDebug() << "APPENDING EXRA";
            m_data.append((uchar)0);
            m_pCounter++;
        }
        Debug(" data: 1 byte = 0 ");
        m_pCounter++;
        return;
    }
    for (int i=1;i<lst.count();i++) {
//        qDebug() <<lst[i];
        //if (lst[i].trimmed()=="")
         //   continue;
        m_data.append((uchar)Util::NumberFromStringHex(lst[i]));
        m_pCounter++;
        if (isWord) {
            m_data.append((uchar)Util::NumberFromStringHex(lst[i]));
            m_pCounter++;

        }

    }
//    qDebug() << "Data org: " << s;
   qDebug() << " data: "+Util::numToHex(lst.count()-1)+" bytes";
  //  Debug(" data: "+Util::numToHex(lst.count()-1)+" bytes");

}

void OrgAsm::TreatIncBin(QString s)
{
    s = s.replace("\"","");
    if (!QFile::exists(s)) {
        qDebug() << "File doesn't exist: " << s;
        exit(1);
    }
    QFile f(s);
    f.open(QFile::ReadOnly);
    QByteArray d = f.readAll();
    f.close();
    m_data.append(d);
    //qDebug() << " CUR : " << Util::numToHex(m_pCounter);
    m_pCounter+=d.count();
}

QStringList OrgAsm::Compile(Instruction::Pass pass, QStringList& lst)
{
    int cnt  = 0;
    QStringList newList;
    for (QString& s: lst) {
        QString o = s;
        if (s.simplified()=="")
            continue;
        if (s.simplified().startsWith(";"))
            continue;
        if (pass==Instruction::passSymbol && !o.toLower().contains("incbin"))
            s = TreatLabel(o).toLower();

        if (s =="") // label only
            continue;

        // Test for org

//        qDebug() << "L " << o <<  " " << s;
        QStringList l = s.simplified().split(" ");
//        qDebug() << s;
        if (l[0]=="org") {
            TreatOrg(l);
        }
        else
        if (l[0]==".byte" || l[0]=="dc.b" || l[0]=="d.b" || l[0] ==".word") {
            TreatData(s);
        }
        else
        if (l.count()==2 && l[0]=="incbin") {
            //qDebug() << o;
            TreatIncBin(o.simplified().split(" ")[1]);
        }

        else {


            Instruction i;
            i.Assemble(s, m_opCodes, m_pCounter, pass, m_symbols);
            m_data.append(i.m_data);
            m_pCounter+=i.m_data.count();
        }
//        m_debug << i.m_debug;


        if (pass==Instruction::passSymbol) {
//            lst[cnt] = s;
            newList<<s;
//            qDebug() << " PPPP " << lst[cnt];
//            s = TreatLabel(o);
        }
        cnt++;
    }

    return newList;
}




void OrgAsm::Assemble(QString inFile, QString outFile)
{
    QFile f(inFile);
    f.open(QFile::ReadOnly);
    QString allData = f.readAll();
    QStringList lst = allData.split("\n");
    f.close();

    LoadCodes();

    m_data = QByteArray();
    m_debug = QStringList();
    m_output = "";
    m_pCounter=0;

    try {

        qDebug() << " ******* PASS PREPROCESS";
        lst = Preprocess(lst);

        m_data.clear();
        lst = BinaryOps(lst);
        m_data.clear();

/*        for (QString l: lst)
           qDebug() << l.simplified();

        exit(1);
*/
        qDebug() << " ******* PASS SYMBOL";
        lst = Compile(Instruction::passSymbol, lst);
        m_data.clear();

        qDebug() << " ******* PASS BINARY OP";
        lst = BinaryOps(lst);
        m_data.clear();

//        exit(1);


        qDebug() << " ******* PASS COMPILE";
        Compile(Instruction::passCompile, lst);
        for (QString l: lst)
           qDebug() << l.simplified();


//        exit(1);



    } catch (QString error) {
        qDebug() << "ERROR: " << error;
        return;
    }
    m_success = true;
    if (QFile::exists(outFile))
        QFile::remove(outFile);
    QFile out(outFile);
    out.open(QFile::WriteOnly);
    out.write(m_data);
    out.close();
}

QStringList OrgAsm::Preprocess(QStringList lst)
{
    int cnt  = 0;
    QStringList newList;
    m_prep.clear();
    for (QString& s: lst) {

        if (s.simplified()=="")
            continue;
        if (s.simplified().startsWith(";"))
            continue;
        s = s.split(";")[0];
        //s = s.toLower().simplified();
        s = s.replace("dc.b", ".byte");
        s = s.replace("dc.w", ".word");

        if (s.contains("=")) {
            QStringList l = s.simplified().split("=");
            m_prep[l[0].trimmed()] = l[1].trimmed();
            qDebug() << l[0] <<  " wth  " << l[1];

//            m_symbols[l[0]] = Util::NumberFromStringHex( l[1]);
            continue;
        }
        // Afterwards, replace all
        s = s.replace("\t", " ");
        QStringList l = s.split(" ");
//        qDebug() << s;
        if (l.count()>1) {

            for (int i=1;i<l.count();i++) {

                QString v = l[i];
  //              qDebug() << v;

                QStringList l2 = v.split(",")[0].split("-")[0].split("+");
                QString cmp = l2[0].replace("(","").replace(")","").simplified();
                for (QString k : m_prep.keys())
                    if (k==cmp) {
    //                    qDebug() << "** Replace Before : " << s;
                        s = s.replace(k,m_prep[k]);
      //                  qDebug() << "** Replace after: " << s;
                    }
            }
        }

//        qDebug() << s;
       newList<<s;
    }

    return newList;
}

QStringList OrgAsm::BinaryOps(QStringList lst)
{
    int cnt  = 0;
    QStringList newList;
    m_prep.clear();
    for (QString& s: lst) {
        QString o = s;
        if (s.simplified()=="")
            continue;
        if (s.simplified().startsWith(";"))
            continue;
        if (s.simplified().startsWith(".byte") || s.simplified().startsWith("dc.b") ||
                s.simplified().startsWith("d.b") || s.simplified().startsWith(".word")) {

            newList<< s;
            continue;
        }
        if (!(s.startsWith(" ") || s.startsWith("\t"))) {
            newList<< s;
            continue;

        }
        if (s.simplified().startsWith("org") ) {
            newList<< s;
            continue;
        }

        //s = s.toLower().simplified();

        QString label = "";
        if (!(s.startsWith(" ") || s.startsWith("\t"))) {
            s=s.replace(":", "");
            label=s.simplified().split(" ")[0];
            s=s.replace(label, "");
        }


        QString ret = "";
        QStringList l = s.simplified().split(" ");
        QString main = l[0];
        QString tag = "";
        if (l.count()>1)
        for (int i=1;i<l.count();i++) {
            tag+=" " + l[i];
        }
        QStringList lTag = tag.split(",");
        lTag[0] = Util::BinopString(lTag[0]);
//        qDebug() << "Before: " <<s;
        s = main + " " + lTag[0];
        if (lTag.count()>1)
            s+= ","+lTag[1];
        s= label + " " + s;

//        if (s.contains("$80"))
  //          qDebug() << s;

       newList<<s;
    }

    return newList;

}

bool Instruction::Assemble(QString& line, Opcodes6502 &m_opCodes, int pCounter, Instruction::Pass pass, QMap<QString, int>& symbols) {


    QStringList ll = line.split(";");
    line = ll[0].simplified();

    if (line.contains("*")) {
//        line = line.replace("*", Util::numToHex(pCounter));
        QString add = line.simplified().split(" ")[1].replace(" ", "");
        add = add.replace("*", Util::numToHex(pCounter));
        add = Util::BinopString(add);
        line = line.split(" ")[0] + " " + add;
        qDebug() << add;

    }

    if (pass==passCompile) {
        for (QString sym : symbols.keys()) {
            // Replace symbols

            QStringList l = line.split(" ");
            if (l.count()>1) {
                QStringList l2 = l[1].split(",");
//                qDebug() << l2 << " vs " << sym;

                //sym = sym.toLower();
                QString cmp = l2[0].replace("(","").replace(")","").trimmed().toLower().split("+")[0].split("-")[0];
                if (cmp==(sym.toLower())) {
//                    line = line.replace(sym, Util::numToHex(symbols[sym]));
                    l2[0].replace(sym.toLower(),Util::numToHex(symbols[sym]));
                    l2[0] = Util::BinopString(l2[0]);
//                    qDebug() << line << " vs " << l2[0] <<  " with sym " << sym ;
                    line = l[0] + " " + l2[0];
                    if (l2.count()>1)
                        line+="," + l2[1];
  //                  qDebug() << " *** " << line;

                }

            }
        }
    }

//    qDebug() << " ** " << line;

    QStringList lst = line.split(" ");
    QString opCode = lst[0];

    if (opCode=="processor")
        return true;
//    qDebug() << line;

    if (opCode=="org")
        return true;
    Type type = imp;
    MOSOperandCycle cyc;

//    if (opCode=="org")
  //      return false;

    if (m_opCodes.m_opCycles.contains(opCode))
        cyc = m_opCodes.m_opCycles[opCode];
    else {
        qDebug() << line;
        throw QString("Uknown opcode: " + opCode);
    }

    if (cyc.m_opcodes.count()==0)
        throw QString("Opcode not implemented yet: " + opCode);



    if (lst.count()>1)
        type = getType(lst[1]);
    else
        type = none;

    // Override types or pass symbol
    if (pass==passSymbol) {
        if (opCode == "jmp" || opCode=="jsr")
            type = abs;

//        if (opCode == "ldy" )
    }

    int code = cyc.m_opcodes[(int)type];
    if (code==0 && pass==passCompile) {
        qDebug() << "ERROR on line : " << line;
        throw QString("Opcode type not implemented yet: " + opCode + "  type " +type );
    }


    int val=0;
    if (type!=none) {
        QString num = lst[1].split(",")[0].replace("#","").replace("(","").replace(")","");
        //qDebug() << " NUM " << num << "  with type " << type;
        val = Util::NumberFromStringHex(num);
    }
//    qDebug() << cyc.m_name;
  //  qDebug() << cyc.m_opcodes;
    m_debug << (" I : " + opCode + " type: " + (int)type  + ":" +Util::numToHex(code) + "    value : " + Util::numToHex(val)) ;


    m_data.append(code);


    if (opCode=="bpl" || opCode=="bne" || opCode=="beq" || opCode=="bcc" || opCode=="bcs") {
        int diff = (val)-pCounter-2;
//        qDebug() << "Diff: " << Util::numToHex((uchar)diff);
        m_data.append((uchar)diff);
    }
    else

    if (type==zp || type==zpx || type==zpy || type == izx || type == izy || type==imm) {
        m_data.append(val);
    }
    else
    if (type==abs || type==abx || type == aby || type == ind) {
        m_data.append(val&0xFF);
        m_data.append((val>>8)&0xFF);
    }
//    qDebug() << Util::numToHex(pCounter) << m_debug << " " <<QString::number(m_data.count());

    return true;

}
