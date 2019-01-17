/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

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

    m_messages[NO_DASM] = Message(NO_DASM, Message::Error, "You need to download the Dasm assembler (http://dasm-dillon.sourceforge.net/) and specify the location of the dasm executable in the TRSE settings panel. Or stick to the built-in TRSE assembler namde 'OrgAsm'.");
    m_messages[NO_EXOMIZER] = Message(NO_EXOMIZER, Message::Error, "In order to use Exomizer, you need to download the exe file and  specify the location of the exomizer executablein the TRSE settings panel.");
    m_messages[NO_C1541] = Message(NO_C1541, Message::Error, "In order to create .d64 disk files, you need to install vice & specify the location of the c1541 disk tool in the TRSE settings dialog.");
    m_messages[NO_EMULATOR] = Message(NO_EMULATOR, Message::Error,
                                      "You need to download an emulator for your system (c64, nes, vic20, c128) (such as Vice) and specify the location in the TRSE settings panel.");

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
                                      "Hi there! <br><br>Welcome to this new release of Turbo Rascal Syntax Error 0.06! "
                                      "If this is your first time using the TRSE framework, please open the tutorials project located in the parent directory and play around. <br> "
                                      "<br> "
                                      "Thank you so much for trying out Turbo Rascal Syntax Error! Remember to report any bugs to <b>generalgrapefruit@lemonspawn.com</b>.");


    m_messages[OLD_SPRITE_FILE] = Message(OLD_SPRITE_FILE, Message::Msg,
                                      "You are now opening a deprecated and old version of the sprite system used in TRSE. While it is still possible "
                                       "to use this old system, we strongly recomend that you start using the new sprite file types. <br><br>"
                                       "You can export your old sprite data to a binary file, and then just import into the new .flf sprite type. "
                                          );


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
