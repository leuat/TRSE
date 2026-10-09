#pragma once

#include "tripeutil.h"
#include <map>
#include <string>

using namespace std;

namespace tripe {

class Opcode {
  public:

    /*
        m_opcode is the opcode itself
        m_size is the size of the opcode, ie 1 for "clc" or 2 for "lda #10"
        m_cycles is the number of cycles this opcode uses
        m_ins is the instruction, ie "lda"
        m_org is the full org definition from the cpu opcode file, ie i:lda #%i08:a9:2:3
        m_arg is the argument type, ie "#%i08" or "(%i08),x" etc
        m_type is either i08 or i16

    */

    uint16_t m_opcode, m_size, m_cycles;
    string m_ins, m_org, m_arg, m_type = "";
    bool m_isLocal = false;

    Opcode(string instruction, uint16_t opcode, uint16_t size, uint16_t cycles,
           bool isLocal)
        : m_org(instruction), m_opcode(opcode), m_size(size), m_cycles(cycles),
          m_isLocal(isLocal) {
        auto l = Util::clean_split(m_org, ' ');
        m_ins = l[0];
        if (l.size() >= 2)
            m_arg = l[1];
        else
            m_arg = "";

        // extract %i08 etc
        if (m_arg.size() > 0) {
            int cnt = 0;
            while (cnt < m_arg.size() && m_arg[cnt] != '%')
                cnt++;

            if (cnt < m_arg.size())
                m_type = m_arg.substr(cnt + 1,
                                      3); // + m_arg[cnt + 1] + m_arg[cnt + 2];
            if (m_type != "i08" && m_type != "i16")
                throw string(
                    "Orgasm internal error in opcode definitions for " + m_ins +
                    ", incorrect type :" + m_type);
        }
    }
    Opcode() {}
};

class OrgAsm {
  public:
    OrgAsm(string defs);
    // Assembles an .asm file
    void Assemble(string in, string out);

    // Opcodes and commands
    map<string, vector<Opcode>> m_opcodes;
    map<string, string> m_cmd;

    // Current file (for error output)
    string m_curFile = "";

    bool m_isLittleEndian = true;
    // Current pass
    int m_pass = 0;
    // program counter
    uint64_t m_pc = 0;
    // Symbol table
    map<string, int> m_symtab;
    // Source file
    vector<string> m_src;
    // Output binary data
    vector<uint8_t> m_data;
    // Hex string
    string m_hex = "$";
    // Current line
    int m_curLine = 0;
    // First org will write to a .prg
    bool m_firstOrg = true;
    const string alNum =
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVXYZ$0123456789_$<>%*";
    const string alNumOrExpr = alNum + "#+-*/ ";

  private:
    // Loads the opcode table for the CPU in question
    void LoadDefs(string s);
    // Converts a value (symbol or constant, reference lo/hi etc) to int
    int getValue(string s);
    // Parse a single line
    void Parse(string s);

    // Parse calls the following helper function to evaluate each line

    bool ProgramCounter(vector<string> &l);
    bool Consts(string s);
    bool Label(string s);
    void Pass(int type);
    void HandleInstruction(vector<string> &l, string s);
    void LoadData(string s, vector<string> &l, string org);
    void IncBin(string s);

    // Matches a pattern with the opcode def, ie if
    // lda #$10
    // matches
    // lda #%i08(true) or lda %i08 or lda (%i08),y   or lda %i16etc
    Opcode matchPattern(string op, string s, string &var, string &varArg,
                        int &ival);

    // Only adds data on pass 2
    bool addData() { return m_pass >= 2; }

    // Writes the finished instruction opcodes + data to m_data
    // Also evalutes potential additional paramters, like "sta p+1"
    void addInstructionData(const Opcode &op, int ival, string varg);

    // Gets a almin substring 
    string getVariable(const string &tst, string s, int &pos);

    // Handy error message
    string err() {
        return "\nOrgAsm error in " + m_curFile + " on line " +
               std::to_string(m_curLine) + ":\n";
    }
};

} // namespace tripe