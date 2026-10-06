#ifndef DATA_H
#define DATA_H

#include <string>

namespace tripe {

class Data {
  public:
    static Data d;
    std::string opcodes = "";
    bool isInternal = false;
    int regZp = 0x80; // where to put temp values
    int ptrZp = 0x02; // where to put pointers
    int whZp = 0x40;  // where to put workhorse values
};
} // namespace tripe

#endif
