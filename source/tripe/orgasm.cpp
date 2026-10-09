#include "orgasm.h"
#include "resources/p6502.h"

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
            Opcode op(ops[1], Util::fromNumber("0x" + ops[2]),
                      +Util::fromNumber(ops[3]), +Util::fromNumber(ops[4]));

            //        cout << op.m_ins << " " << op.m_arg << endl;
            m_opcodes[op.m_ins][op.m_arg] = op;
        }
        if (ops[0] == "c") {
            m_cmd[ops[1]] = ops[2];
            if (ops[1] == "hex")
                m_hex = ops[2];
        }
    }
}

void OrgAsm::Assemble(string in, string out) {
    string s = Util::load_text_file(in);
    m_src.clear();
    m_src = Util::split(s, '\n', m_src);

    Pass(0);
}

void OrgAsm::Parse(string s) {
    auto l = Util::clean_split(s, ' ');
    if (l.size() == 0)
        return;
    /*
        for (auto &c : l)
            cout << "'" << c << "'";
        cout << endl;
    */
    if (l[0] == m_cmd["pc"]) {
        m_pc = Util::fromNumber(l[1]);
        cout << m_pc << endl;
        cout << "setting org: " << l[1] << " " << Util::toHex(m_pc) << endl;
    }
}

void OrgAsm::Pass(int pass) {
    m_pass = pass;
    m_pc = 0;
    for (auto s : m_src) {
        s = Util::trim(s);
        if (s == "" || s.starts_with(";"))
            continue;
        Parse(s);
    }
}

} // namespace tripe