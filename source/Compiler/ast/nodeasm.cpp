#include "nodeasm.h"

QString NodeAsm::Build(Assembler *as) {
    QStringList txt = m_asm.split("\n");
    as->Comment("");
    as->Comment("****** Inline assembler section");
    for (QString t: txt) {
        as->Write(t,0);
    }
    as->Asm("");
    return "";
}
