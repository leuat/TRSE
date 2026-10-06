#include "parser.h"
#include "cpu6502.h"
#include "error.h"
#include "opcodes.h"
#include "opts/phopt6502.h"
#include "opts/tropt.h"
#include "util.h"
#include <fstream>

using namespace tripe;
namespace tripe {

vector<uint8_t> TripeParser::ParseText(string inFile) {
    m_data.clear();
    m_src = Util::read_text_code_file(inFile, true);
    m_src_org = Util::read_text_code_file(inFile, false);

    ParseTextToBinary();

    return m_data;
}

vector<string> TripeParser::TripeOptimise(string inFile) {
    m_data.clear();
    m_src = Util::read_text_code_file(inFile, false);
    Tropt t;
    for (int i = 0; i < 4; i++)
        m_src = t.optimise(m_src);

    std::cout << "Optimized " << t.m_noLines << " lines of tripe" << endl;
    return m_src;
}

void TripeParser::LoadBinary(string inFile) {
    m_data = Util::load_binary(inFile);

    string id = "";
    id += m_data[0];
    id += m_data[1];
    id += m_data[2];
    if (id != m_id) {
        Error::RaiseError("'" + inFile + "' not a binary Tripe file.");
    }
    m_data.erase(m_data.begin(), m_data.begin() + 3);
    m_src.clear();
}

void TripeParser::AppendExtraCode(AbstractCPU *cpu) {
    int lineNumber = -1;
    for (int i = 0; i < m_src.size(); i++) {
        if (Util::trim(m_src[i]) == ".gcode") {
            lineNumber = i;
        }
    }
    if (lineNumber != -1) {
        m_src[lineNumber] = "; tripe cpu specific code";

        for (auto code : cpu->m_usedCode) {
            vector<string> d;
            d = Util::split(cpu->m_code[code], '\n', d);
            int i = lineNumber + 1;
            for (auto &s : d)
                m_src.insert(m_src.begin() + i++, s);
        }
        cpu->InsertTempValues(m_src, lineNumber);
    }
}

vector<string> TripeParser::ParseBinary(string inFile, string arch,
                                        map<string, string> params) {
    LoadBinary(inFile);

    AbstractCPU *cpu = NULL;
    if (arch == "tripe2trasm")
        cpu = new Opcodes();
    if (arch == "mos6502")
        cpu = new CPU6502();

    if (cpu == NULL)
        Error::RaiseError("ParseBinary error: unrecognized architecture " +
                          arch);

    cpu->m_src.clear();
    cpu->m_curBranch = 0;
    // Pass 0
    ParseBinary(cpu, 0);
    // estimated source code, used for branching
    cpu->m_src = m_src;
    // pass 1
    m_src = cpu->stub(params);
    ParseBinary(cpu, 1);

    AppendExtraCode(cpu);

    Phopt *phOpt = NULL;
    if (arch == "mos6502")
        phOpt = new Phopt6502();

    Phopt::s_optLines = 0;
    for (int i = 0; i < 4; i++)
        m_src = phOpt->optimize(m_src);

    std::cout << "optimized " << Phopt::s_optLines << " native lines of asm"
              << endl;

    return m_src;
}

void TripeParser::ParseTextToBinary() {

    Opcodes op;
    m_data.push_back(m_id[0]);
    m_data.push_back(m_id[1]);
    m_data.push_back(m_id[2]);
    int ln = 0;
    int cnt = 0;
    for (auto s : m_src) {
        if (op.m_inRawAsm) {
            //                std::cout << m_src_org[cnt] << endl;
            if (s.find(".endasm") != std::string::npos) {
                m_data.push_back(op.m_asmToOpcode[".endasm"]);
                op.m_inRawAsm = false;
                cnt++;
                continue;
            }
            for (auto c : m_src_org[cnt])
                m_data.push_back(c);

            m_data.push_back(10); // newline
            //            m_data.push_back('\t'); // newline
            cnt++;
            continue;
        }
        s = Util::ReplaceString(s, "$", "0x"); // replace all 'x' to 'y'
        s = Util::ReplaceString(s, "\t", " "); // replace all 'x' to 'y'
        s = Util::ReplaceString(s, "  ", " "); // replace all 'x' to 'y'
        vector<string> str;
        Util::split(s, ';', str);
        s = str[0];
        Error::s_curLine = s;
        Error::s_lineNumber = ln++;
        vector<string> v;
        // cout << s << endl;
        Util::split(s, ' ', v);
        op.ParseToBinary(v, m_data);
        cnt++;
    }
}

void TripeParser::ParseBinary(AbstractCPU *op, int pass) {
    int ln = 0;
    int pos = 0;
    op->m_data = m_data;
    op->m_pass = pass;
    op->m_curBranch = 0;
    while (pos < m_data.size()) {
        string s = op->ParseFromBinary(pos);
        vector<string> sp;
        sp = Util::split(s, '\n', sp);
        for (auto p : sp)
            //            if (p != "" && !p.ends_with("nada"))
            m_src.push_back(p);

        if (m_src.size() != 0)
            if (m_src.back().find(".", 0) == 0) {
                m_src.insert(m_src.end() - 1, "");
            }
        op->m_curLine = m_src.size() - 1;
    }
}
} // namespace tripe