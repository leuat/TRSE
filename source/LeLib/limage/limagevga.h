#ifndef LIMAGEVGA_H
#define LIMAGEVGA_H


#include "source/LeLib/util/util.h"
#include "source/LeLib/limage/limageok64.h"
class LImageVGA : public LImageOK64
{
public:

    LImageVGA(LColorList::Type t);

//    void ExportBin(QFile &file) override;



};

#endif // LIMAGEVGA_H
