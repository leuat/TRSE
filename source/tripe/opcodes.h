#ifndef OPCODES_H
#define OPCODES_H

#include "abstractcpu.h"
#include "tripedata.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

namespace tripe {
class Opcodes : public AbstractCPU {
  public:
    void ParseToBinary(vector<string> &line, vector<uint8_t> &m_data);
    string ParseFromBinary(int &pos);
    bool m_inRawAsm = false;
    static const int DATATYPE_STRING = 1;
    static const int DATATYPE_NUMBER = 0;

    Opcodes() { Init(Data::s_opcodes); }
};
} // namespace tripe

#endif