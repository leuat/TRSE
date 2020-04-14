#ifndef LIMAGEGAMBOY_H
#define LIMAGEGAMBOY_H

#include "limagenes.h"


class LImageGamboy : public LImageNES
{
public:
    LImageGamboy(LColorList::Type t);

    void ExportBin(QFile &file);


};

#endif // LIMAGEGAMBOY_H
