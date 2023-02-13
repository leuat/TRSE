#include "source/LeLib/util/util.h"
#include "source/LeLib/util/cinifile.h"
#include "systemthomson.h"


SystemThomson::SystemThomson(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System6809(settings,proj)
{
    m_processor = M6809;
    m_system = THOMSON;

    m_startAddress = 0x2600;
    m_programStartAddress = 0x2600;
    m_supportsExomizer = false;


 //   m_labels.append(SystemLabel(SystemLabel::ZEROPAGE,"System values",0,0x00FF));
 /* m_labels.append(SystemLabel(SystemLabel::STACK,"Stack",0xBF00,0xBFFF));
    m_labels.append(SystemLabel(SystemLabel::BASIC,"Basic",0x0200,0x3FFF));
    m_labels.append(SystemLabel(SystemLabel::FREE,"Free",0x4000,0xAFFF));
    m_labels.append(SystemLabel(SystemLabel::VIC,"Video memory",0xC000,0xFFFF));
    m_labels.append(SystemLabel(SystemLabel::SID,"Firmware",0xB000,0xBEFF));
*/
    m_systemColor = QColor(40,90,40);
    m_requireEmulatorWorkingDirectory = true;

}


void SystemThomson::PostProcess(QString &text, QString file, QString currentDir)
{
    if (!m_buildSuccess)
        return;

    Createk5Tape(file);
/*    auto in = Util::loadBinaryFile(file+".bin");
    QByteArray b;
    b.resize(m_startAddress);
    b.fill(0);
    b.append(in);
    Util::SaveByteArray(b,file+".sna");*/
}

void SystemThomson::applyEmulatorParameters(QStringList &params, QString debugFile, QString filename, CIniFile *pini) {
    params << "mo5";
    filename = "/Users/leuat/code/TRSE/Publish/tutorials/THOMSON/tutorials/agenda";
    //params <<"-dump"<<filename+".sna";
    params <<"-cass"<<filename+".k7";
    params <<  "-resolution0" << "640x480@60" <<"-window";
    params <<"-nothrottle";
    params << "-skip_gameinfo" << "-autoboot_delay"<< "1" <<"-autoboot_command"<< "loadm \"\",,R\\n";

    m_requireEmulatorWorkingDirectory = true;

}


void SystemThomson::Createk5Tape(QString filename)
{
    char** argv = Util::StringListToChar(QStringList() << "" <<filename+".bin");
    char argc = 2;

        const uint8_t sync[] = {
            1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0x3C,0x5A
        };
        while(--argc > 0)
        {
            uint8_t l;
            char* path = argv[argc];
            FILE *in, *out;
            in = fopen(path, "rb");
            out = fopen(QString(filename+".k7").toStdString().c_str(), "wb");

            // HEADER BLOCK
            fwrite(sync, 1, sizeof(sync), out);

            uint8_t head[17];
            l = 0;
            head[l++] = 0; head[l++] = 16;
            for(int i = 0; l < 13;)
            {
                if(i >= strlen(path)) break;
                if(path[i] == '.') i++;
                if(path[i] == '/') {
                    l = 2;
                    i++;
                    continue;
                }
                head[l++] = path[i++];
            }

            while(l < 13) head[l++] = ' ';

            head[l++] = 2;
            head[l++] = 0;
            head[l++] = 0;
            head[l] = 0;

            for(int i = 2; i < l; i++)
                head[l] += head[i];
            head[l] = -head[l];
            l++;
            fwrite(head, 1, 17, out);

            // DATA BLOCKS
            for(;;)
            {
                uint8_t buf[254];
                l = fread(buf, 1, 254, in);
                if(l <= 0) break;

                fwrite(sync, 1, sizeof(sync), out);
                head[0] = 1;
                head[1] = l + 2;
                fwrite(head, 1, 2, out);
                fwrite(buf, 1, l, out);

                head[0] = 0;
                while(l > 0)
                    head[0] -= buf[--l];
                fwrite(head, 1, 1, out);
            }

            // FOOTER BLOCK
            fwrite(sync, 1, sizeof(sync), out);
            head[0] = 255;
            head[1] = 2;
            head[2] = 0;
            fwrite(head, 1, 3, out);

            fclose(in);
            fclose(out);

        }
}


