#ifndef ABSTRACTCPU_H
#define ABSTRACTCPU_H

#include <cstdint>
#include <map>
#include <sstream>
#include <string>
#include <vector>

#include "error.h"
#include "opts/phopt.h"
#include "tripeutil.h"

using namespace std;

namespace tripe {

class Param {
  public:
    static const int VAR = 1;
    string str;
    uint64_t ival;
    int type;
    string typeName = "";
    Param() {
        str = "";
        type = 0;
        ival = 0;
        typeName = "";
    }
    Param(string s, int t, string tn) {
        str = s;
        type = t;
        typeName = tn;
        std::istringstream(str) >> hex >> ival;
        if (str.starts_with("<")) {
            type = 1;
            typeName = "uint8";
            str = clean();
            //           cout << str << endl;
        }
        if (str.starts_with(">")) {
            type = 1;
            typeName = "uint8";
            str = clean();
            str += "+1";
        }
    }
    string lo() {
        if (type == 1) {
            if (isRef())
                return "#<" + clean();
            else
                return str;
        }
        return "#$" + Util::toHex(ival & 255);
    }
    string lhi() {
        if (type == 1) {
            if (isRef())
                return "#>" + clean();
            else if (typeName == "uint16" || typeName.starts_with("ptr"))
                //                   str.starts_with("screen"))
                return str + "+1";
            else {
                //                cout << str << " " << typeName << endl;
                return "#0"; // byte cast
            }
        }
        return "#$" + Util::toHex((ival >> 8) & 255);
    }
    string hi() {
        if (type == 1) {
            if (typeName == "uint8")
                return "#0";
            if (isRef())
                return "#>" + clean();
            else // if (typeName == "uint16" || typeName.starts_with("ptr"))
                return str + "+1";
        }
        return "#$" + Util::toHex((ival >> 8) & 255);
    }

    string prefix() {
        if (type == 1)
            return str;

        return "#$" + str;
    }

    bool isRef() { return str.starts_with("#"); }

    string clean() {
        string s = str;
        return s.erase(0, 1);
    }
};

class AbstractCPU {
  public:
    bool m_initialized = false;
    int m_curPos = 0;
    int m_foundStartPos = -1;
    int m_tempLabel = 1;
    vector<uint8_t> m_data;
    Phopt *m_phopt = 0;
    string m_line = "", m_comment = "";
    void Init(string opcodes);
    bool m_prevCmpWas16bit = false;
    string m_nextCompare = "";

    map<string, string> m_code;
    vector<string> m_usedCode;

    AbstractCPU();

    bool isRegister(string &s) { return s.starts_with("_r"); }

    void addCode(string s) {
        int cnt = count(m_usedCode.begin(), m_usedCode.end(), s);
        if (cnt == 0)
            m_usedCode.push_back(s);
    }

    string getTempLabel() { return "temp_label_" + to_string(m_tempLabel++); }

    virtual void InsertTempValues(vector<string> &lst, int pos) {}

    string ParseFromBinary(int &pos);

    virtual vector<string> stub(map<string, string> params) {
        return vector<string>();
    }

    std::string ParseInlineAsm(vector<uint8_t> &data, int &pos);

    map<string, uint8_t> m_asmToOpcode;
    map<uint8_t, string> m_opcodeToAsm;

    Param getNextParam(vector<uint8_t> &data, int &pos);
    vector<string> m_registersUsed;
    vector<string> m_src;

    vector<int> m_branches;

    int m_curBranch = 0;
    int m_pass = 0;
    int m_curLine;

  protected:
    string m_opcodeFile = "";
    string m_hexprefix = "0x";

    void Asm(string t) { m_line += "\t" + t + "\n"; }
    void Label(string t) { m_line += t + ":\n"; }
    void Label(string t, string v) { m_line += t + ":\t" + v + "\n"; }

    virtual void LoadStore(int &pos, int opcode) {}
    virtual void Declare(int &pos) {}
    virtual void Mulu(int &pos) {}
    virtual void Divu(int &pos) {}
    virtual void Binop(int &pos, int opcode) {}
    virtual void Mov(int &pos) {}
    virtual void Branch(int &pos, int cmd) {}
    virtual void Const(int &pos) {}

    virtual int estimateCodeSize(const string &s) { return 0; }
    int branchSizeEstimator(const string &lbl, int pos);
    //    virtual void Beq(int &pos, string cmd) {}

    bool isBinaryOpOpcode(int code);
    bool isBranchOpcode(int code);
    bool isSingleParamOpcode(int code);

    bool is16bit(const Param &val);

    int m_currentRegister;

    map<uint8_t, vector<string>> m_opcodeToParams;
    map<string, string> m_typeTripeToNative;
    vector<string> m_similarBinops, m_branchOpcodes;
    vector<string> m_singleParamOpcodes;
    vector<string> m_registers;
    map<string, string> m_symtab;

    string m_nada = "_nada";

    string pushReg() { return m_registers[m_currentRegister++]; }
    void popReg() {
        m_currentRegister--;
        if (m_currentRegister < 0)
            Error::RaiseError("Cannot pop register from 0");
    }
};
} // namespace tripe
#endif
