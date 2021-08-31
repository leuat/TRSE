#ifndef LIMAGEVZ200_H
#define LIMAGEVZ200_H

#include "limageqimage.h"

class LImageVZ200 : public LImageQImage
{
public:
    LImageVZ200(LColorList::Type t);


    void ExportBin(QFile &file) override;


};

#endif // LIMAGEVZ200_H
