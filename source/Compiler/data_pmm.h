#ifndef DATAPMM_H
#define DATAPMM_H

#include <QString>

namespace Pmm {
class Data
{
public:
    static Data d;
    QString currentLineText;
    int lineNumber = 0;
    void Init() {
        lineNumber = 0;
        currentLineText ="";
    }

    void Set(int ln, QString txt) {
        lineNumber = ln;
        currentLineText = txt;
    }
};
}
#endif // DATA_H
