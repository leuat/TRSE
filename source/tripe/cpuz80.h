#ifndef CPUZ80_H
#define CPUZ80_H

#include "abstractcpu.h"
#include "tripeutil.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

namespace tripe {

class CPUZ80 : public AbstractCPU {
  public:
    int m_ptrZp = 10;
    int m_regZp = 0x80;
    int m_whZp = 0x40;
    CPUZ80();

    //    string ParseFromBinary(vector<uint8_t>& m_data, int& pos) override;
    void InsertTempValues(vector<string> &lst, int pos) override;
    string loadIndex(string &s, Param &idx, string type, bool &indexIsConst);
    vector<string> stub(map<string, string> params) override;
    map<string, string> m_zpUsed;

    void LoadStore(int &pos, int opcode) override;
    void Declare(int &pos) override;
    void Mulu(int &pos) override;
    void Divu(int &pos) override;
    void Const(int &pos) override;
    void Binop(int &pos, int opcode) override;
    void Mov(int &pos) override;
    void Branch(int &pos, int opcode) override;

    void verifyZp(string name, string val);

    int estimateCodeSize(const string &s) override;

    string insertWhZp(string s);
    bool printCmp(const string &val);

    //    void Beq(int &pos, string cmd) override;

    //    void triplet(vector<uint8_t>& data, int& pos, bool isPtr, bool
    //    isLoad);
};
} // namespace tripe
#endif