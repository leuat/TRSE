#include "messages.h"

Messages Messages::messages;

Messages::Messages()
{
    InitMessages();
}

void Messages::LoadFromCIni(CIniFile* file)
{
    m_iniFile = file;
    QStringList ls = file->getStringList("ignore_messages");
    for (QString s: ls) {
        int i = s.toInt();
        m_messages[i].m_ignore = true;
    }

}

void Messages::InitMessages()
{
    m_messages[DASM_COMPILER_ERROR] = Message(DASM_COMPILER_ERROR, Message::Error, "Oops! Seems like you encountered a DASM compiler error. This typically "
                                   "means that the author of TRSE did something wrong. You can either change some "
                                   "parts of your previous code, or nag the programmer about this bug. If you know any assember, "
                                   "you can always add a comment on the faulty line and try to debug for yourself.");


    m_messages[MEMORY_OVERLAP_ERROR] = Message(MEMORY_OVERLAP_ERROR, Message::Error, "Your program has overlapping memory configurations. A future version of TRSE will include a memory visualization "
                                   "tool, but for now you need to manually reorganize your code blocks and include files.");

    m_messages[NO_DASM] = Message(NO_DASM, Message::Error, "You need to download the Dasm assembler (http://dasm-dillon.sourceforge.net/) and specify the location of the dasm executable in the TRSE settings panel.");
    m_messages[NO_EXOMIZER] = Message(NO_EXOMIZER, Message::Error, "In order to use Exomizer, you need to download the exe file and  specify the location of the exomizer executablein the TRSE settings panel.");
    m_messages[NO_EMULATOR] = Message(NO_EMULATOR, Message::Error,
                                      "You need to download a c64 emulator (such as Vice) and specify the location in the TRSE settings panel.");

    m_messages[NO_PROJECT] = Message(NO_PROJECT, Message::Error,
                                      "You need to open a project in order to create a new file.");

    m_messages[CHARSET_WARNING] = Message(CHARSET_WARNING, Message::Msg,
                                      "In order to display anything in the level editor, you first need to load and set a (binary) charset from the <b>charset</b> tab.");

    m_messages[BRANCH_ERROR] = Message(BRANCH_ERROR, Message::Error,
                                      "Branching error!<br><br>In order to circument the +-127-byte branching limitations, the TRSE compiler tries to automatically decide whether to use short conditional jumps "
                                       "(bcs, bne etc) or long jumps (jmp). However, this does not always work, so you will sometimes need to explicitly specify whether "
                                       "the branch is an <b>offpage</b>(&gt;127 bytes) or <b>onpage</b> (&lt;127 bytes) branch. The syntax is as follows:<br>"
                                       "<br>"
                                       "if a>b onpage then ...  // force onpage &gt;127 bytes brach<br>"
                                       "if a>b offpage then ...  // force offpage &lt;127 bytes brach (slower)<br>"

                                       );

    m_messages[ALPHA_WARNING] = Message(ALPHA_WARNING, Message::Msg,
                                      "Ah. Hi there! <br><br>Welcome to this final alpha release of Turbo Rascal Syntax Error! "
                                      "A fair warning: This version might have some bugs and will occasionally crash. "
                                      "If this is your first time using the TRSE framework, please open the tutorials project located in the parent directory and play around. <br> "
                                      "<br> "
                                      "Thank you so much for trying out Turbo Rascal Syntax Error! Remember to report any bugs to <b>generalgrapefruit@lemonspawn.com</b>.");


}
void Messages::DisplayMessage(int message)
{
    Message m = m_messages[message];

    if (m.m_ignore)
        return;
    DialogMessage* dm = new DialogMessage();

    QImage img;
    if (m.m_severity==Message::Error)
        img.load(":resources/images/friday.png");
    if (m.m_severity==Message::Msg)
        img.load(":resources/images/creatures.png");

    dm->SetMessage(m.m_message, img);
    dm->exec();
    if (dm->ignoreIsChecked()) {
        m_iniFile->addStringList("ignore_messages", QString::number(message), true);
        m_messages[message].m_ignore = true;
    }


    delete dm;

}
