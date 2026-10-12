#include "guts/guts6502.h"
#include "resources/p6502_2.h"


vector<uint8_t> Guts6502::m_memory;

void Guts6502::LoadDefs() {
	cout << "HERE"<<endl;
	string s = string((char *)resources_orgasm_p6502_txt_2);
	cout << "HERE2"<<endl;
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
        	int op = Util::fromNumber("0x"+ops[2]);
        	m_ins[op] = Util::split(ops[1],' ')[0];
        	m_size[op] = Util::fromNumber(ops[3]);
        }
    }
}






