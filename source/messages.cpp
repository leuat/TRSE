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

    m_messages[NO_DASM] = Message(NO_DASM, Message::Error, "You need to download the Dasm assembler (http://dasm-dillon.sourceforge.net/) and specify the location of dasm.exe in the TRSE settings panel.");
    m_messages[NO_EXOMIZER] = Message(NO_EXOMIZER, Message::Error, "In order to use Exomizer, you need to download the exe file and  specify the location of exomizer.exe in the TRSE settings panel.");
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
                                      "Ah. Hi there! <br><br>Welcome to this pre-pre-alpha release of Turbo Rascal Syntax Error. However, be warned: "
                                      "this version is probably riddled with bugs and will occasionally crash. Not really meant for production. "
                                      "For now, open the tutorial 1-10 project located in the parent directory and play around. <br> "
                                      "<br> "
                                      "<b>A fair warning:</b><br>"
                                      "In the following version, the tutorials will probably be overhauled. So, if you do create your own project, please remember "
                                      "to place it in a completely separate directory from the TRSE directory, or everything will be overwritten with subsequent updates.<br>"
                                      "<br>"
                                      "If there are bugs with the updater, you can ignore it by just running <b>bin&#92;trse.exe</b><br><br>"
                                      "Thank you so much for trying out this pre-pre-alpha release of the TRSE suite! Remember to report any bugs to <b>generalgrapefruit@lemonspawn.com</b>.");


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
