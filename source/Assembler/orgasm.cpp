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
    line=line.toLower();
    if (! (line.startsWith("\t") || (line.startsWith(" ")) )) {
       // a : dc.b 0
       line.replace(":"," ");

       QStringList lblSplit = line.simplified().split(" ");
       QString lbl = lblSplit[0];
       if (lbl!="") {
            m_symbols[lbl] = m_pCounter;

            Debug("New symbol '" + lbl+"' at " + Util::numToHex(m_pCounter) );

            line = line.replace(lbl, " ");
       }
    }

    return line.simplified().trimmed().toLower();

}

void OrgAsm::TreatOrg(QStringList l)
{
    int val = Util::NumberFromStringHex(l[1]);

    if (m_data.count()==0) {
        m_data.append(val&0xFF);
        m_data.append((val>>8)&0xFF);
        m_pCounter = val;
        Debug("First org: " + Util::numToHex(val));


    }
    else {
        int cnt =0;
        while (m_pCounter<val) {
            m_data.append((uchar)0);
            m_pCounter++;
            cnt++;

        }
        Debug("Org padding : " + Util::numToHex(cnt) +" bytes to " + Util::numToHex(val));


    }

}

void OrgAsm::TreatData(QString s)
{
    QStringList lst = s.replace(",", " ").simplified().split(" ");


    if (lst.count()==1) {
        m_data.append((uchar)0);
        Debug(" data: 1 byte (single");
        m_pCounter++;
        return;
    }
    for (int i=1;i<lst.count();i++) {
//        qDebug() <<lst[i];
        m_data.append((uchar)Util::NumberFromStringHex(lst[i]));
        m_pCounter++;

    }

    Debug(" data: "+Util::numToHex(lst.count()-1)+" bytes");

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

    for (QString s: lst) {
        QString o = s;
        if (s.simplified()=="")
            continue;
        if (s.simplified().startsWith(";"))
            continue;
        s = TreatLabel(o);

        if (s =="") // label only
            continue;

        // Test for org
        QStringList l = s.split(" ");
        if (l.count()==2 && l[0]=="org") {
            TreatOrg(l);
            continue;
        }
        if (l[0]==".byte" || l[0]=="dc.b") {

            TreatData(s);
            continue;
        }


        Instruction i;
        i.Assemble(s, m_opCodes, m_pCounter);
        m_data.append(i.m_data);
        m_pCounter+=i.m_data.count();
        m_debug << i.m_debug;

    }
    } catch (QString error) {
        qDebug() << "ERROR: " << error;
        return;
    }
    if (QFile::exists(outFile))
        QFile::remove(outFile);
    QFile out(outFile);
    out.open(QFile::WriteOnly);
    out.write(m_data);
    out.close();
}

bool Instruction::Assemble(QString line, Opcodes6502 &m_opCodes, int pCounter) {

    QStringList ll = line.split(";");
    line = ll[0].simplified();

    if (line.contains("*"))
        line = line.replace("*", Util::numToHex(pCounter));


    QStringList lst = line.split(" ");
    QString opCode = lst[0];

    if (opCode=="processor")
        return true;


    Type type = imp;
    MOSOperandCycle cyc;

    if (m_opCodes.m_opCycles.contains(opCode))
        cyc = m_opCodes.m_opCycles[opCode];
    else
        throw QString("Uknown opcode: " + opCode);



    if (lst.count()>=1)
        type = getType(lst[1]);

    int code = cyc.m_opcodes[(int)type];

    int val = Util::NumberFromStringHex(lst[1]);

    qDebug() << cyc.m_name;
    qDebug() << cyc.m_opcodes;
    m_debug << (" I : " + opCode + " type: " + (int)type  + ":" +Util::numToHex(code) + "    value : " + Util::numToHex(val)) ;

    qDebug() << m_debug;
    if (code==0)
        throw QString("Unknown operation code: " + line);

    m_data.append(code);


    if (type==zp || type==zpx || type==zpy || type == izx || type == izy || type==imm) {
        m_data.append(val);
    }
    if (type==abs || type==abx || type == aby || type == ind) {
        m_data.append(val&0xFF);
        m_data.append((val>>8)&0xFF);
    }

    return true;

}
