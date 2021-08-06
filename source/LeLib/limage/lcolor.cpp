#include "lcolor.h"


unsigned short LColor::get12BitValue() {
    return color.blue()/16  | (color.green()/16)<<4  | (color.red()/16)<<8;

}

unsigned short LColor::get15BitValue() {
    return color.red()/8  | (color.green()/8)<<5  | (color.blue()/8)<<10;

}

unsigned short LColor::get9BitValue() {
    return color.blue()/32  | (color.green()/32)<<4  | (color.red()/32)<<8;

}
