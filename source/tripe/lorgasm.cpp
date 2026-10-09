#include "lorgasm.h"
#include "resources/p6502.h"
#include "tinyexpr.h"

namespace tripe {

OrgAsm::OrgAsm(string defs) {
    if (defs == "mos6502")
        LoadDefs(string((char *)resources_orgasm_p6502_txt));
}

void OrgAsm::LoadDefs(string s) {
    vector<string> l;
    l = Util::split(s, '\n', l);
    for (auto &s : l) {
        s = Util::trim(s);
        if (s == "")
            continue;
        if (s.starts_with("#"))
            continue;

        auto ops = Util::clean_split(s, ':');
        // instructions
        if (ops[0] == "i") {
            bool isLocal = false;
            if (ops.size() >= 6)
                isLocal = ops[5] == "local";
            Opcode op(ops[1], Util::fromNumber("0x" + ops[2]),
                      +Util::fromNumber(ops[3]), +Util::fromNumber(ops[4]),
                      isLocal);

            m_opcodes[op.m_ins].push_back(op);
        }
        // commands
        if (ops[0] == "c") {
            m_cmd[ops[1]] = ops[2];
            if (ops[1] == "hex")
                m_hex = ops[2];
            if (ops[1] == "endian")
                m_isLittleEndian = ops[2] == "0";
        }
    }
}

void OrgAsm::Assemble(string in, string out) {
    string s = Util::load_text_file(in);
    m_curFile = in;
    m_src.clear();
    m_src = Util::split(s, '\n', m_src);

    //   cout << " *** pass 0: consts" << endl;
    Pass(0);

    //   cout << " *** pass 1: labels and syms" << endl;
    Pass(1);

    //    cout << " *** pass 2: assemble" << endl;
    Pass(2);
    Pass(2);

    cout << "OrgAsm done." << endl;

    // Util::save_text("test.asm", m_src);
    Util::save_binary(out, m_data);
}
void OrgAsm::LoadData(string type, vector<string> &l, string org) {

    int itype = 1; // byte default
    if (type == m_cmd["i16"])
        itype = 2;
    if (type == m_cmd["i32"])
        itype = 4;

    int scount = 1;
    for (int i = 1; i < l.size(); i++) {
        auto lst = Util::split(l[i], ',');
        for (auto d : lst) {
            // String handling
            if (d.starts_with("\"")) {
                if ((scount & 0x1) == 1) {
                    vector<string> slst;
                    slst = Util::split(org, '\"', slst);
                    for (auto c : slst[scount])
                        m_data.push_back((uint8_t)c);
                    m_pc += slst[scount].size();
                }
                scount += 1;

            } else {

                uint64_t ival = Util::fromNumber(d);
                //            cout << ival << endl;
                if (m_isLittleEndian) {
                    if (itype >= 1)
                        m_data.push_back((uint8_t)(ival & 0xff));
                    if (itype >= 2) {
                        m_data.push_back((uint8_t)((ival >> 8) & 0xff));
                    }
                } else
                    throw string("Big endian not supported yet");

                m_pc += itype;
            }
            // cout << "Done" << endl;
        }
    }
    //    cout << Util::toHex(m_pc) << " : " <<
    //    (Util::toHex((int)m_data.back()))
    //         << endl;
}
void OrgAsm::IncBin(string s) {
    s.erase(0, 1);
    s.erase(s.size() - 1, 1);
    auto data = Util::load_binary(s);
    m_data.insert(m_data.end(), data.begin(), data.end());
    m_pc += data.size();
}

bool OrgAsm::Consts(string s) {
    if (s.find("=") == string::npos)
        return false;

    auto l = Util::clean_split(Util::trim(s), '=');
    if (m_pass == 0) {
        auto var = Util::trim(l[0]);
        m_symtab[var] = Util::fromNumber(l[1]);
    }
    return true;
}

bool OrgAsm::Label(string s) {
    if (!(!s.starts_with(" ") && !s.starts_with("\t")))
        return false;

    string lbl = "";
    string cl = Util::trim(s);
    // replace ":" with " "
    if (cl.find(":") != string::npos)
        cl = Util::ReplaceString(cl, ":", " ");

    // split with ' '
    auto lst = Util::clean_split(cl, ' ');
    if (m_pass >= 1) {

        auto var = Util::trim(lst[0]);
        m_symtab[var] = m_pc;
        // Load definitions afterwards
        if (lst.size() >= 2) {
            lst.erase(lst.begin());
            LoadData(lst[0], lst, s);
        }
    }
    return true;
}
bool OrgAsm::ProgramCounter(vector<string> &l) {
    if (l[0] != m_cmd["pc"])
        return false;
    uint64_t org = m_pc;
    m_pc = Util::fromNumber(l[1]);

    if (!m_firstOrg) {
        for (int i = 0; i < m_pc - org; i++)
            m_data.push_back((uint8_t)0xff);
    }

    if (m_firstOrg) {
        m_firstOrg = false;
        m_data.push_back((uint8_t)(m_pc & 0xff));
        m_data.push_back((uint8_t)((m_pc >> 8) & 0xff));
    }

    return true;
}
void OrgAsm::Parse(string s) {
    // Remove all comments
    s = Util::split(s, ';')[0];

    if (Consts(s))
        return;

    if (Label(s))
        return;

    auto l = Util::clean_split(s, ' ');

    if (l.size() == 0)
        return;

    if (ProgramCounter(l))
        return;

    if (l[0] == m_cmd["i8"] || l[0] == m_cmd["i16"] ||
        l[0] == m_cmd["string"]) {
        LoadData(l[0], l, s);
        return;
    }
    if (l[0] == m_cmd["incbin"]) {
        IncBin(l[1]);
        return;
    }

    if (m_pass != 0)
        HandleInstruction(l, s);
}

void OrgAsm::HandleInstruction(vector<string> &l, string s) {
    if (m_opcodes.contains(l[0])) {
        string var = "";  // variable p
        string varg = ""; // arg like +1 + someConst
        if (l.size() == 1) {
            // nop, brk, clc etc
            Opcode opcode;
            // Find the empty opcode
            for (auto &o : m_opcodes[l[0]])
                if (o.m_arg == "")
                    opcode = o;
            addInstructionData(opcode, 0, varg);
        } else if (l.size() > 1) {

            string args = "";
            for (int i = 1; i < l.size(); i++) {
                if (l[i] != "")
                    args += l[i];
                if (i != l.size() - 1)
                    args += " ";
            }
            int ival = 0x1000;
            auto opcode = matchPattern(l[0], args, var, varg, ival);
            args = Util::trim(var);

            addInstructionData(opcode, ival, varg);
        }
        //        cout << "Found var: " << var << endl;
    } else {
        throw string(err() + "Unknown or non-impmented instruction :" + s);
    }
}

void OrgAsm::addInstructionData(const Opcode &op, int ival, string varg) {
    if (addData())
        m_data.push_back((uint8_t)op.m_opcode);

    // Increase pc
    m_pc += op.m_size;

    if (!addData())
        return;

    if (op.m_arg == "")
        return;

    // We have arguments like p+1+3*20 etc
    if (varg != "") {
        varg = Util::ReplaceString(varg, "$", "0x");
        int error = 0;
        //        cout << varg << " :  $" << Util::toHex(ival) << endl;
        ival += te_interp(varg.data(), &error);
        //      cout << "after" << " :  $" << Util::toHex(ival) << endl;
        if (error != 0) {
            throw string("Error in expression : " + std::to_string(ival) + " " +
                         varg);
        }
    }
    // Local branch or whatever
    if (op.m_isLocal) {
        ival -= m_pc;
        //        cout << "Branch size: " << Util::toHex(ival) << endl;
        if (ival >= 128 || ival <= -127)
            throw string("Local branch out of range");
    }

    if (m_isLittleEndian)
        for (int i = 0; i < op.m_size - 1; i++) {
            m_data.push_back((uint8_t)(ival & 0xff));
            ival >>= 8;
        }
    else
        throw string(err() + "Big endian not supported yet!");
}

Opcode OrgAsm::matchPattern(string op, string s, string &var, string &varg,
                            int &ival) {
    //   cout << "Pattern: " << s << endl;
    bool found = false;
    if (m_opcodes.contains("op"))
        throw string("Unknown opcode: " + op);
    ival = 0x1000;
    for (auto opcode : m_opcodes[op]) {
        //        cout << "compare : " << op << "   - " << s << " to " <<
        //        opcode.m_arg
        //           << endl;
        int posInData = 0;
        int posInArg = 0;
        string arg = opcode.m_arg;

        bool found = true;

        while (posInData < s.size() || posInArg < arg.size()) {
            if (arg[posInArg] == '%') {
                posInArg++;
                char first = s[posInData];
                // Get stuff like #$10, i, ptr etc

                var = getVariable(alNum, s, posInData);
                if (var.starts_with("<"))
                    var = "#" + var;
                if (var.starts_with(">"))
                    var = "#" + var;

                // Get symbol value of var
                ival = getValue(var);

                // Reject if wrong type
                if (ival <= 0xff && opcode.m_type == "i16")
                    found = false;
                if (ival > 0xff && opcode.m_type == "i08")
                    found = false;

                // Get arguments like +1 or +2*SOMETHING (not implemented yet)
                varg = getVariable(alNumOrExpr, Util::trim(s), posInData);

                posInArg += 3;
                continue;
            }
            if (posInData >= s.size() || posInArg >= arg.size() ||
                (s[posInData] != arg[posInArg]))
                found = false;

            posInArg++;
            posInData++;
        }
        if (found) {
            return opcode;
        }
    }

    throw string(
        err() +
        "OrgAsm::MatchPattern - Unknown or non-impmented instruction pattern " +
        op + " " + s);
    return Opcode("NONE", 0, 0, 0, false);
}

int OrgAsm::getValue(string var) {
    int ival = 0x1000;

    if (var.find("*") != string::npos) {
        return m_pc;
    }

    if (var.starts_with("#<") || var.starts_with("#>")) {
        bool isLo = var.starts_with("#<");
        var = Util::ReplaceString(var, "#<", "");
        var = Util::ReplaceString(var, "#>", "");
        if (m_symtab.contains(var)) {
            if (isLo)
                return (m_symtab[var]) & 0xff;
            else
                return (m_symtab[var] >> 8) & 0xff;

            ival = Util::fromNumber(var);
            if (isLo)
                return ival & 0xff;
            else
                return (ival >> 8) & 0xff;
        }
    }

    if (Util::isPureNumber(var))
        ival = Util::fromNumber(var);

    if (m_symtab.contains(var))
        ival = m_symtab[var];

    return ival;
}

void OrgAsm::Pass(int pass) {
    m_pass = pass;
    m_pc = 0;
    m_data.clear();
    m_firstOrg = true;
    m_curLine = 0;
    for (auto s : m_src) {
        string cl = Util::trim(s);
        if (cl == "" || cl.starts_with(";")) {
            m_curLine++;

            continue;
        }
        Parse(s);
        m_curLine++;
    }
}

string OrgAsm::getVariable(const string &tst, string s, int &pos) {
    string ret = "";
    int start = pos;
    while ((tst.find(s[pos]) != string::npos) && pos < s.size()) {
        ret += s[pos++];
    }
    return ret;
}

} // namespace tripe