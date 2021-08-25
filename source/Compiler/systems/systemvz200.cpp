#include "systemvz200.h"

#include "source/LeLib/util/lz4/lz4.h"
#include "source/Compiler/errorhandler.h"

SystemVZ200::SystemVZ200(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): SystemZ80(settings, proj) {
    m_processor = Z80;
    m_system = VZ200;

    m_startAddress = 0x7B00;
    m_programStartAddress = 0x7B00;
    m_supportsExomizer = false;


    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0x7000,0x7800));

    m_systemColor = QColor(30,60,90);

    m_requireEmulatorWorkingDirectory = true;


}

void SystemVZ200::Assemble(QString &text, QString filename, QString currentDir, QSharedPointer<SymbolTable> symTab)
{
    QString output;
    int time = timer.elapsed();

    output+="<br>";

    PerformAssembling(filename,text,currentDir,symTab);


    if (!QFile::exists(filename+".bin")) {
        text  += "<br><font color=\"#FFFF00\">Error during assembly : please check source assembly for errors.</font>";
        text+=output;
        m_buildSuccess = false;
        return;
    }





/*
    if (m_projectIni->getdouble("exomizer_toggle")==1) {

        QString fn = filename+".bin";
        int maxx = 0xF8FF;
        int start = 0x4000;
        int actualStart  = 0;
        int size = QFileInfo(fn).size();
        if (size>maxx) {
            text += "<br><font color=\"red\">Error compressing CPC file: file is too large : </font>"+Util::numToHex(size)+" > " + Util::numToHex(maxx);
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
        if (size>(maxx-start)) {
                    text += "<br><font color=\"red\">Error compressing CPC file: compressed file is too large : </font>"+Util::numToHex(size)+" > " + Util::numToHex(maxx-start);
                    m_buildSuccess = false;
                    if (QFile::exists(fn+"_c"))
                        QFile::remove(fn+"_c");

                    return;

                }

//        Util::SaveByteArray(ba,fn);
        // Make sure there are padding before
        actualStart = 0xB780 - size;
        text += "<br><font color=\"yellow\">Start address : " +Util::numToHex(actualStart) + " to " + Util::numToHex(actualStart+size) + ".</font>";
        if (actualStart<0x4100) {
            text += "<br><font color=\"red\">Error compressing CPC file: compressed file is too large : </font>"+Util::numToHex(size)+" > " + Util::numToHex(0xb780-0x4100);
            m_buildSuccess = false;
            if (QFile::exists(fn+"_c"))
                QFile::remove(fn+"_c");

            return;


        }
        QString code = Util::loadTextFile(":resources/code/amstrad/unpack.asm");
        code = code.replace("@START", Util::numToHex(actualStart));
        code = code.replace("@FILE", fn);
//        code = code.replace("@UNPACKCODE", "$F000");
        QString codeFile = QFileInfo(fn).dir().path()+QDir::separator() + "_unpack";
        Util::SaveTextFile(codeFile+".asm", code);
        QFile::remove(filename+".bin");
//        QString temp;
        PerformAssembling(codeFile,text,currentDir,symTab);
        Util::CopyFile(codeFile+".bin",filename+".bin");
        QFile::remove(codeFile+".bin");
        QFile::remove(codeFile+".asm");

//        StartProcess(assembler, QStringList() << codeFile <<filename+".bin", output);


    }
    */
    if (m_buildSuccess) {
        text +="<br>Assembled file size: <b>" + QString::number(QFileInfo(filename+".bin").size()) + "</b> bytes";
    }

    output+="<br>";

    time = timer.elapsed();


    text+=output;

}

void SystemVZ200::PostProcess(QString &text, QString file, QString currentDir)
{
    // Add vz header
    auto d = Util::loadBinaryFile(file+".bin");
    QByteArray h;
    /*
     * defb 'VZF0'
    defb 'AGDGAME',0,0,0,0,0,0,0,0,0,0
    defb $f1
    defb $00    ; lb $7b00
    defb $7b    ; hb $7b00

    org $7b00
    */
    h.append("VZF0");
    h.append("AGDGAME");
    for (int i=0;i<10;i++)
        h.append((char)0x0);
    h.append(0xf1);
    h.append((char)0x0);
    h.append(0x7b);
    h.append(d);
    Util::SaveByteArray(h,file+".vz");


}
