#ifndef CC1541_H
#define CC1541_H
#include <ctype.h>
#include <locale.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <wchar.h>

extern "C" int cc1541(int argc, char* argv[]);

class CC1541
{
public:
    CC1541();
};

#endif // CC1541_H
