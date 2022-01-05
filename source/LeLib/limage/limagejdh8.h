#ifndef LIMAGEJDH8_H
#define LIMAGEJDH8_H


#include "source/LeLib/limage/limageqimage.h"
#include "source/LeLib/util/util.h"
class LImageJDH8 : public LImageQImage
{
public:

    LImageJDH8(LColorList::Type t);

    void ExportBin(QFile &file) override;

//    void LoadBin(QFile &file) override;



};

#endif // LIMAGEJDH8_H
