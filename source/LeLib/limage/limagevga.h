#ifndef LIMAGEVGA_H
#define LIMAGEVGA_H


#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limageqimage.h"
class LImageVGA : public LImageQImage
{
public:

    LImageVGA(LColorList::Type t);

//    void ExportBin(QFile &file) override;

    void ExportBin(QFile &file);


};

#endif // LIMAGEVGA_H
