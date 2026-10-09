#pragma once

#include "tripeutil.h"
#include <map>
#include <string>

using namespace std;

namespace tripe {

class Opcode {
  public:
    uint16_t m_opcode, m_size, m_cycles;
    string m_ins, m_org, m_arg;

    Opcode(string instruction, uint16_t opcode, uint16_t size, uint16_t cycles)
        : m_org(instruction), m_opcode(opcode), m_size(size), m_cycles(cycles) {
        auto l = Util::clean_split(m_org, ' ');
        m_ins = l[0];
        m_arg = l[1];
    }
    Opcode() {}
};

class OrgAsm {
  public:
    OrgAsm(string defs);
    void Assemble(string in, string out);
    map<string, map<string, Opcode>> m_opcodes;
    map<string, string> m_cmd;
    int m_pass = 0;
    uint64_t m_pc = 0;
    map<string, int> m_symbols;
    vector<string> m_src;
    vector<uint8_t> m_data;
    string m_hex = "";

  private:
    void LoadDefs(string s);
    void Pass(int type);
    void Parse(string s);
};

} // namespace tripe