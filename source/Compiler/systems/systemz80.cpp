#include "systemz80.h"
#include "source/LeLib/util/lz4/lz4.h"
#include "source/OrgAsm/zorgasm.h"
void SystemZ80::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    AssembleZOrgasm(output,text,filename,currentDir,symTab);

}

void SystemZ80::PostProcess(QString &text, QString file, QString currentDir)
{
}


void SystemZ80::PerformAssembling(QString filename, QString &text,QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    bool useOrgasm = false;
    if (m_settingsIni->contains("assembler_z80"))
            useOrgasm = m_settingsIni->getString("assembler_z80")!="Pasmo";


    QString assembler = m_settingsIni->getString("pasmo");
    if (!useOrgasm && !QFile::exists(assembler)) {
        text  += "<br><font color=\"#FF6040\">Please set up a link to the PASMO assembler directory in the TRSE settings panel.</font>";
        m_buildSuccess = false;
        return;
    }
    if (QFile::exists(filename+".bin"))
        QFile::remove(filename+".bin");



    if (useOrgasm) {
        QString output="";
        AssembleZOrgasm(output,text,filename,currentDir,symTab);
  //      QProcess process;
//        StartProcess(assembler, QStringList() << filename+".asm" <<filename+"_pasmo.bin", text);

    }
    else {
        QProcess process;
        StartProcess(assembler, QStringList() << filename+".asm" <<filename+".bin", text);
    }



}

void SystemZ80::PackProgram(QString filename, int start, int actualStarto, QString&text,  QString currentDir, QSharedPointer<SymbolTable> symTab, QString asmFile)
{
    QString fn = filename+".bin";
    int maxx = 0xF8FF;
    int actualStart  = 0;
    int size = QFileInfo(fn).size();
    if (size>maxx) {
        text += "<br><font color=\"red\">Error compressing file: file is too large : </font>"+Util::numToHex(size)+" > " + Util::numToHex(maxx);
        m_buildSuccess = false;
        if (QFile::exists(fn+"_c"))
            QFile::remove(fn+"_c");
        return;

    }
    QFile::remove(fn+".bin_c");

    CompressLZ4(fn);

    fn = filename+".bin_c";
//        QByteArray ba = Util::loadBinaryFile(fn);
    size = QFileInfo(fn).size();
    text += "<br>Compressed file : <b>" + QString::number(size) +" bytes ( "+Util::numToHex(size) + ") </b>";
/*        while (ba.size()<(maxx-start))
        ba.append((char)0);
*/
    if (size>(maxx-start)) {
                text += "<br><font color=\"red\">Error compressing file: compressed file is too large : </font>"+Util::numToHex(size)+" > " + Util::numToHex(maxx-start);
                m_buildSuccess = false;
                if (QFile::exists(fn+"_c"))
                    QFile::remove(fn+"_c");

                return;

            }

//        Util::SaveByteArray(ba,fn);
    // Make sure there are padding before
    actualStart = actualStarto - size;
    text += "<br><font color=\"yellow\">Start address : " +Util::numToHex(actualStart) + " to " + Util::numToHex(actualStart+size) + ".</font><br>";
    if (m_system == AMSTRADCPC)
    if (actualStart<0x4100) {
        text += "<br><font color=\"red\">Error compressingfile: compressed file is too large : </font>"+Util::numToHex(size)+" > " + Util::numToHex(0xb780-0x4100);
        m_buildSuccess = false;
        if (QFile::exists(fn+"_c"))
            QFile::remove(fn+"_c");

        return;


    }
    QString code = Util::loadTextFile(asmFile);
    code = code.replace("@START", Util::numToHex(actualStart));
    code = code.replace("@FILE", fn);
//        code = code.replace("@UNPACKCODE", "$F000");
    QString codeFile = QFileInfo(fn).dir().path()+QDir::separator() + "_unpack";
    Util::SaveTextFile(codeFile+".asm", code);
    QFile::remove(filename+".bin");
//        QString temp;
    auto keep = m_orgAsm;
    PerformAssembling(codeFile,text,currentDir,symTab);
    m_orgAsm = keep; // keep for memory analyzer
    Util::CopyFile(codeFile+".bin",filename+".bin");
    QFile::remove(codeFile+".bin");
//    QFile::remove(codeFile+".asm");

//        StartProcess(assembler, QStringList() << codeFile <<filename+".bin", output);


}

QString SystemZ80::CompressFile(QString fileName)
{
    return CompressLZ4(fileName);
}
