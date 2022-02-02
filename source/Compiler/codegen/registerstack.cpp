#include "registerstack.h"

GenRegisterStack::GenRegisterStack()
{

}

QString GenRegisterStack::Push() {
    if (m_lvl>=m_regs.count())
        ErrorHandler::e.Error("Error in registerstack push : trying to push regstack from max");

    m_lvl++;
    return m_regs[m_lvl];
}

void GenRegisterStack::Pop() {
    if (m_lvl==0)
        ErrorHandler::e.Error("Error in registerstack pop : trying to pop from zero");
    m_lvl--;
}

QString GenRegisterStack::Get(int dd) {
    if (m_lvl+dd>=m_regs.count()) {
        ErrorHandler::e.Error("Error in registerstack get : out of bounds");
    }
    return m_regs[m_lvl+dd];

}
