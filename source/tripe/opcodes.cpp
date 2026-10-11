#include "opcodes.h"
#include "error.h"
#include "tripeutil.h"
#include <sstream>

using namespace tripe;
namespace tripe {

void Opcodes::ParseToBinary(vector<string> &line, vector<uint8_t> &data) {
    if (!m_asmToOpcode.contains(line[0]))
        Error::RaiseError("Opcodes::ParseToBinary : Error parsing line with opcode: " + line[0]);

    uint8_t opcode = m_asmToOpcode[line[0]];
    if (opcode == 0) {
        Error::RaiseError("Opcodes::ParseToBinary : Incorrect opcode on line : " + line[0]);
    }
    vector<string> &p = m_opcodeToParams[opcode];
    bool isFirstParam = true;

    data.push_back(opcode);

    if (line[0] == ".asm") {
        m_inRawAsm = true;
        return;
    }

    int opCodePos = data.size() - 1;
    int i = 0;
    for (auto s : p) {
        if (s == "0")
            break;
        vector<uint8_t> d;
        i++;
        while (line[i] == "")
            i++;

        if (i >= line.size())
            Error::RaiseError("Incorrect number of parameters");
        if (s == "datastream") {
            string type = line[0];
            uint16_t cnt = 0;
            vector<uint8_t> d;
            stringstream ss;
            while (i < line.size()) {
                getIntOrString(line[i], type, d);
                cnt++;
                i++;
            }
            if (cnt >= 256)
                Error::RaiseError("Error: cannot have more than 255 elements per line");

            d.insert(d.begin(), (uint16_t)(cnt >> 8) & 0xff);
            d.insert(d.begin(), (uint16_t)(cnt & 0xff));

            for (auto b : d) {
                data.push_back(b);
            }
        }

        if (s == "ival" || s == "p") {
            vector<string> v;

            if (s == "p" && line[i].find(":", 0) == std::string::npos && line[i].find(":", 0)) {
                // Is a variable: Print out the full name
                Util::append_string(Util::trim(line[i]), d);
            } else {

                Util::split(line[i], ':', v);

                auto a = v[0];
                a = Util::trim(a);
                if (v.size() == 1)
                    v.push_back("0");
                if (v.size() != 2)
                    Error::RaiseError("ival type must be of format uint8:[number]");
                if (!m_asmToOpcode.contains(a))
                    Error::RaiseError("Unknown type: " + a);

                //   cout << line[i] << " v :'" << v[1] << "'" << endl;
                /*                d = Util::ival2int8(v[1], v[0]);
                                d.insert(d.begin(), 0);*/
                getIntOrString(v[1], v[0], d);
                d.insert(d.begin(), m_asmToOpcode[a]);
            }
        }
        for (auto b : d) {
            data.push_back(b);
        }
    }
}

string Opcodes::ParseFromBinary(int &pos) {
    return "";
    /*
    cout <<" A AARGH * "<<endl;
    string s = "";
    uint8_t opcode = data[pos];
    s = m_opcodeToAsm[opcode];
    pos+=1;
    cout << " CRASH" << endl;
    exit(1);
    std::cout << "ORG opcode : " << s <<  " " << (int)opcode <<std::endl;
    if (s==".asm") {
        s+="\n";
        while (data[pos]!=m_asmToOpcode[".endasm"]) {
            s+=data[pos++];
        }
        s+="\n.endasm\n";
        pos++;
        std::cout << "Found ASM : "<<s << std::endl;
        return s;
    }


    vector<string>& p = m_opcodeToParams[opcode];
  //  cout << "*** CUR "<<s << endl;
    for (auto str:p) {
        if (str=="0") break;
        uint8_t v = data[pos];

        if (str=="datastream") {
            pos++;
            int cnt = (data[pos]) | (data[pos+1]<<8);
            pos+=2;
//            cout << "Recreate: "<<s <<to_string(cnt);
            for (int i=0;i<cnt;i++) {
                s+=m_hexprefix+Util::toHex(data[pos++]) + " ";
            }
        }
        std::cout << s << endl;

        if (m_opcodeToAsm.contains(v) && v>0xF0) {
            // We have a const type int64 etc
            pos+=1;
            s+=" " +
m_opcodeToAsm[v]+":0x"+Util::ival2string(data,pos,m_opcodeToAsm[v]);
            pos+=Util::getIntLen(m_opcodeToAsm[v]);
        }
        else // Some text
        {
            s+=" ";
            while (data[pos]!=0) {
                s+=data[pos++];
            }
            pos++;
        }

    }
    if (s.find(".",0)!=0)
        s = "\t"+s;
    return s;*/
}
} // namespace tripe