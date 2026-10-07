#ifndef OPCODES_H
#define OPCODES_H

#include "abstractcpu.h"
#include "tripedata.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>
using namespace std;

namespace tripe {
class Opcodes : public AbstractCPU {
  public:
    void ParseToBinary(vector<string> &line, vector<uint8_t> &m_data);
    string ParseFromBinary(int &pos);
    bool m_inRawAsm = false;
    static const int DATATYPE_STRING = 1;
    static const int DATATYPE_NUMBER = 0;

    void getIntOrString(string s, const string &type, vector<uint8_t> &d) {
        auto val = Util::trim(s);
        bool isHex = false;
        bool is16bit = (type.find("uint16") != string::npos) ||
                       (type.find("address") != string::npos);

        //        cout << "getintorstring :  " << type << " " << is16bit <<
        //        endl;
        if (val.starts_with("0x")) {
            isHex = true;
        }
        stringstream ss;
        uint8_t flag = DATATYPE_NUMBER;
        if (val != "") {
            int ival = 0;
            ss.clear();
            if (isHex)
                ss << std::hex << val;
            else
                ss << std::dec << val;
            ss >> ival;
            if (ss.fail()) {
                flag = DATATYPE_STRING;
                //                        cout << " FAIL '" << val << "'
                //                        " << ival << endl;
            }
            // else
            //     cout << "OK '" << val << "'  " << ival << endl;
            d.push_back(flag);
            if (flag == DATATYPE_NUMBER) {
                if (is16bit)
                    d.push_back((ival >> 8) & 0xFF);
                d.push_back(ival & 0xFF);
            } else {
                // String
                for (auto c : val) {
                    d.push_back((uint8_t)c);
                }
                d.push_back((uint8_t)0);
            }
        }
    }

    Opcodes() { Init(Data::d.opcodes); }
};
} // namespace tripe

#endif