#include "abstractcpu.h"

#include "error.h"
#include "opcodes.h"
#include "resources/opcodes_data.h"

using namespace tripe;
namespace tripe {

AbstractCPU::AbstractCPU() {
    m_similarBinops = {"add", "sub", "or", "and", "xor", "mulu", "shl", "shr"};
    m_singleParamOpcodes = {"call", "jump"};
    m_branchOpcodes = {"jeq", "jneq", "jgt", "jlt", "jgte", "jlte", "jgts", "jlts", "jgtes", "jltes"};
}

bool AbstractCPU::isPointer(const Param &val) {
    return ( m_symtab[val.str].starts_with("ptr")); 
}

bool AbstractCPU::is16bit(const Param &val) {
    return (m_symtab[val.str] == "int16" || m_symtab[val.str] == "uint16" || m_symtab[val.str].starts_with("ptr") || m_symtab[val.str].starts_with("address16")) ||
           m_opcodeToAsm[val.type] == "uint16" || val.str.starts_with("#");
}

bool AbstractCPU::isSigned(const Param &val) {
    return (m_symtab[val.str] == "int16" || m_symtab[val.str] == "int8");
}

bool AbstractCPU::is16bitArray(const Param &val) {
    return (m_symtab[val.str] == "int16" || m_symtab[val.str] == "uint16" || m_symtab[val.str] == ("ptr16") || m_symtab[val.str].starts_with("address16")) ||
           m_opcodeToAsm[val.type] == "uint16";
}

void AbstractCPU::Init(string opcodes) {
    if (m_initialized)
        return;
    vector<string> d;
    string oc = string((char *)resources_opcodes_txt);
    d = Util::split(oc, '\n', d);
    for (auto s : d) {
        s = Util::trim(s);
        if (s == "")
            continue;
        if (s.starts_with("#"))
            continue;
        vector<string> v;
        s = Util::ReplaceString(s, " ", "");
        Util::split(s, ',', v);
        if (v.size() <= 1)
            continue;
        int val;
        stringstream(v[1]) >> hex >> val;
        string str = Util::toLower(Util::trim(v[0]));
        if (m_opcodeToAsm.contains(val))
            Error::RaiseError("Error when reading the opcodes list: opcode $" + Util::toHex(val) + " is taken.");

        m_asmToOpcode[str] = val;
        m_opcodeToAsm[val] = str;
        vector<string> params;
        if (v.size() >= 3) {
            params = Util::split(v[2], ':', params);
            for (auto &sp : params) {
                sp = Util::toLower(Util::trim(sp));
            }
        }
        m_opcodeToParams[val] = params;
    }
    m_initialized = true;
}

Param AbstractCPU::getNextParam(vector<uint8_t> &data, int &pos) {
    string s = "";
    int type = 0;
    int flag = 0;
    uint8_t v = data[pos];
    string typeName = "";
    bool isVar = false;
    if (m_opcodeToAsm.contains(v) && v >= 0xE0) {
        // We have a const type int64 etc
        pos += 1;
        flag = data[pos++];
        if (flag == Opcodes::DATATYPE_STRING) {
            while (data[pos] != 0)
                s += data[pos++];

            pos++;

        } else {

            s += Util::ival2string(data, pos, m_opcodeToAsm[v]);
            pos += Util::getIntLen(m_opcodeToAsm[v]);
        }
        type = v;
        typeName = m_opcodeToAsm[type];
    } else // Some text
    {
        //                symType val = getSymbolValue(pos);

        symType val = getSymbolValue(pos);
        if (!m_binToSym.contains(val))
            throw string("Error parsing binary data, symbol id not found: $" + Util::toHex(val) + " in position $" + Util::toHex(pos));

        s = m_binToSym[val];
        //        cout << "Recovering symbol: "<< s << endl;
        type = Param::VAR;
        typeName = m_symtab[s];
        isVar = true;
    }

    if (s.starts_with("_r"))
        if (!contains(m_registersUsed, s))
            m_registersUsed.push_back(s);

    return Param(s, type, typeName, flag, isVar);
}

bool AbstractCPU::isBinaryOpOpcode(int code) {
    for (string s : m_similarBinops) {
        if (m_asmToOpcode[s] == code)
            return true;
    }

    return false;
}

bool AbstractCPU::isBranchOpcode(int code) {
    for (string s : m_branchOpcodes) {
        if (m_asmToOpcode[s] == code)
            return true;
    }

    return false;
}
bool AbstractCPU::isSingleParamOpcode(int code) {
    for (string s : m_singleParamOpcodes) {
        if (m_asmToOpcode[s] == code)
            return true;
    }

    return false;
}





std::string AbstractCPU::ParseInlineAsm(vector<uint8_t> &data, int &pos) {
    string s = "";
    uint8_t opcode = data[pos];
    s = m_opcodeToAsm[opcode];

    if (s == ".asm") {
        std::string as = "";
        pos++;
        while (data[pos] != m_asmToOpcode[".endasm"]) {
            as += data[pos++];
        }
        pos++;
        return as;
    }
    return "";
}

// void HandleDataStream(int& pos, string )

AbstractCPU::symType AbstractCPU::getSymbolValue(int &pos) {
    symType val = 0;
    for (int i = 0; i < sizeof(symType); i++)
        val |= m_data[pos + i] << (i * 8);
    pos += sizeof(symType);
    return val;
}

string AbstractCPU::ParseFromBinary(int &pos) {
    m_line = "";
    auto data = m_data;
    uint8_t opcode = data[pos];
    if (opcode == 0)
        throw string("AbstractCPU::ParseFromBinary: Error parsing binary tripe, illegal opcode 0 at pos " + Util::toHex(pos));

    int type = 0;
    auto as = ParseInlineAsm(data, pos);
    if (as != "") {
        return as;
    }
    pos++;
    if (!m_opcodeToAsm[opcode].starts_with("."))
        Asm("; " + m_opcodeToAsm[opcode]);

    if (opcode == m_asmToOpcode[".symbol"]) {
        // Add bin symbol
        symType val = getSymbolValue(pos);
        string sym = "";
        while (m_data[pos] != 0)
            sym += (char)m_data[pos++];
        // get symbol value
        pos++;
        m_binToSym[val] = sym;
        //        cout << "Symbol value: " <<val<< " " <<sym  << "   pos:" << (int)m_data[pos]<< endl;
        return m_line;
    }

    if (opcode == m_asmToOpcode[".uint8"] || opcode == m_asmToOpcode[".uint16"] || opcode == m_asmToOpcode[".int8"] || opcode == m_asmToOpcode[".int16"]) {
        string stype = m_opcodeToAsm[opcode];
        stype.erase(stype.begin(), stype.begin() + 1);
        m_line = "\t" + m_typeTripeToNative[stype] + "\t";
        int cnt = (uint8_t)data[pos] | (((uint8_t)data[pos + 1]) << 8);
        pos += 2;
        for (int i = 0; i < cnt; i++) {

            uint8_t flag = data[pos++];
            if (flag == Opcodes::DATATYPE_NUMBER) {
                int val = 0;
                if (opcode == m_asmToOpcode[".uint16"] || opcode == m_asmToOpcode[".int16"]) {
                    val = data[pos] << 8;
                    //                cout << val;
                    val |= data[++pos];
                } else
                    val = data[pos];

                pos++;
                m_line += m_hexprefix + Util::toHex(val);
            }
            if (flag == Opcodes::DATATYPE_STRING) {
                string s = "";
                while (data[pos] != 0) {
                    s += char(data[pos++]);
                }
                pos++;
                //                cout << " line data " << s << endl;
                m_line += s;
            }

            if (i != cnt - 1)
                m_line += ", ";
        }
        return m_line;
    }

    if (opcode == m_asmToOpcode[".code"] || opcode == m_asmToOpcode[".data"]) {
        auto p1 = getNextParam(data, pos);
        string name = "";
        if (opcode == m_asmToOpcode[".code"]) {
            auto na = getNextParam(data, pos);
            name = "; " + na.str;
        }
        if (p1.ival != 0) {
            m_curPos = p1.ival;
            if (m_foundStartPos == -1)
                m_foundStartPos = m_curPos;
            if (m_curPos >= 0x200) {
                Asm("org " + p1.prefix() + name);
            }
        }
    }

    if (isSingleParamOpcode(opcode)) {
        auto lbl = getNextParam(data, pos);
        Asm(m_typeTripeToNative[m_opcodeToAsm[opcode]] + "\t" + lbl.str);
    }

    if (opcode == m_asmToOpcode[".label"])
        Label(getNextParam(data, pos).str);
    if (opcode == m_asmToOpcode[".gcode"]) {
        Asm(".gcode");
        return m_line;
    }
    if (isBranchOpcode(opcode)) {
        Branch(pos, opcode);
        return m_line;
    }

    if (opcode == m_asmToOpcode[".incbin"])
        Asm("incbin " + getNextParam(data, pos).prefix());
    if (opcode == m_asmToOpcode["mulu"]) {
        Mulu(pos);
        return m_line;
    }
    if (opcode == m_asmToOpcode["divu"]) {
        Divu(pos);
        return m_line;
    }
    if (opcode == m_asmToOpcode["decl"])
        Declare(pos);
    if (opcode == m_asmToOpcode["const"])
        Const(pos);
    if (isBinaryOpOpcode(opcode))
        Binop(pos, opcode);
    if (opcode == m_asmToOpcode["mov"])
        Mov(pos);
    if (opcode == m_asmToOpcode["store"] || opcode == m_asmToOpcode["load"])
        LoadStore(pos, opcode);
    if (opcode == m_asmToOpcode["return"])
        Asm(m_typeTripeToNative["return"]);
    if (opcode == m_asmToOpcode["rti"])
        Asm(m_typeTripeToNative["rti"]);

    return m_line;
}

int AbstractCPU::branchSizeEstimator(const string &lbl, int pos) {

    int size = 0;
    int curLine = m_branches[pos];
    int endLine = curLine - 1;
    // Backward
    // cout << " START LINE " << m_src[curLine] << endl;
    while (!m_src[endLine].starts_with(lbl) && endLine > 0) {
        endLine--;
    }
    if (endLine == 0) {
        endLine = curLine + 1;
        while (!m_src[endLine].starts_with(lbl) && endLine < m_src.size()) {
            endLine++;
        }
    }
    if (endLine == 0 || endLine == m_src.size() - 1)
        Error::RaiseError("Branching error: could not find label " + lbl);
    if (curLine > endLine)
        swap(curLine, endLine);

    // cout << "From lines: " << curLine << "  to " << endLine << endl;
    for (int i = curLine; i < endLine; i++)
        size += estimateCodeSize(m_src[i]);

    //    cout << "End : " << m_src[curLine] << endl;

    return size;
}
} // namespace tripe