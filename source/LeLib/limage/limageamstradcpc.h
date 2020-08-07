#ifndef LIMAGEAMSTRADCPC_H
#define LIMAGEAMSTRADCPC_H

#include "limageqimage.h"

class LImageAmstradCPC : public LImageQImage
{
public:
    LImageAmstradCPC(LColorList::Type t);

    QVector<int> m_data;

    void SetMode(int mode);
    int m_colors;
    int m_bpp;

};

#endif // LIMAGEAMSTRADCPC_H
