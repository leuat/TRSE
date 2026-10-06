#include "cpu6502.h"
#include "error.h"
#include "resources/div16_6502.h"
#include "resources/div8_6502.h"
#include "resources/mul16_6502.h"
#include "resources/mul8_6502.h"
#include "tripedata.h"
#include <algorithm>

using namespace tripe;
namespace tripe {

CPU6502::CPU6502() : AbstractCPU() {
    //    AbstractCPU();
    Init(Data::s_opcodes);
    m_symtab.clear();
    m_hexprefix = "$";
    m_typeTripeToNative["uint8"] = "dc.b";
    m_typeTripeToNative["uint16"] = "dc.w";

    m_typeTripeToNative["add"] = "adc";
    m_typeTripeToNative["sub"] = "sbc";
    m_typeTripeToNative["or"] = "ora";
    m_typeTripeToNative["and"] = "and";
    m_typeTripeToNative["xor"] = "eor";

    /*
        m_typeTripeToNative["bne"] = "bne";
        m_typeTripeToNative["beq"] = "beq";
        m_typeTripeToNative["bgtu"] = "bcc";
        m_typeTripeToNative["bltu"] = "bcs";
        */
    m_typeTripeToNative["shr"] = "lsr";
    m_typeTripeToNative["shl"] = "asl";

    m_typeTripeToNative["jump"] = "jmp";
    m_typeTripeToNative["call"] = "jsr";
    m_typeTripeToNative["fcall"] = "jsr";

    m_typeTripeToNative["return"] = "rts";
    m_typeTripeToNative["rti"] = "rti";

    //    m_typeTripeToNative["bcc"] = "bcc";
    //    m_typeTripeToNative["bcs"] = "bcs";

    m_code["mul8"] = string((char *)resources_6502_mul8_asm);
    m_code["div8"] = string((char *)resources_6502_div8_asm);
    m_code["mul16"] = string((char *)resources_6502_mul16_asm);
    m_code["div16"] = string((char *)resources_6502_div16_asm);
    //    addCode("mul8");
    //    addCode("mul16");
}

void CPU6502::InsertTempValues(vector<string> &lst, int pos) {
    for (auto s : m_registersUsed) {
        lst.insert(lst.begin() + pos, s + " = $" + Util::toHex(m_tmpZp));
        pos += 1;
        m_tmpZp += m_symtab[s] == "uint8" ? 1 : 2;
    }
}

string CPU6502::loadIndex(string &s, string idx, string type) {
    string xy = "x";
    if (type.starts_with("ptr"))
        xy = "y";
    if (type.starts_with("address"))
        xy = "x";

    if (type == "ptr16" || type == "uint16") {
        Asm("lda " + idx);
        Asm("asl");
        Asm("ta" + xy);
    } else
        Asm("ld" + xy + " " + idx);
    return xy;
}

/*
string CPU6502::ParseFromBinary(vector<uint8_t>& data, int& pos) {
}
*/

vector<string> CPU6502::stub(map<string, string> params) {
    vector<string> src;
    string startAddress = "";
    string printAddress = "";

    bool print = false;
    int istart = 0;
    if (params.contains("start_address"))
        startAddress = params["start_address"];

    string basicStart = "$801";

    if (params.contains("sys"))
        if (params["sys"] == "c64" || params["sys"] == "vic20") {
            if (startAddress == "")
                istart = m_foundStartPos;
            print = true;

            if (params["sys"] == "vic20") {
                basicStart = "$" + Util::toHex(m_foundStartPos - 15);
                if (m_foundStartPos == 0x2000)
                    basicStart = "$1201";
            }

            string s = Util::toDec(istart);
            for (auto c : s) {
                printAddress += "$" + Util::toHex(c) + ",";
            }
            // add missing spaces
            while (printAddress.size() < 4)
                printAddress += "$20, ";
        }

    if (print) {
        src.push_back("\torg " + basicStart);
        src.push_back("\tdc.b $b, $8, $a, $0, $9e, $20," + printAddress +
                      " $0, $0, $0");
    }

    return src;
}

void CPU6502::LoadStore(int &pos, int opcode) {
    string s = "";
    auto res = getNextParam(m_data, pos);
    auto idx = getNextParam(m_data, pos);
    auto val = getNextParam(m_data, pos);

    if (opcode == m_asmToOpcode["store"]) {
        // store_p ptr idx val
        auto type = m_symtab[res.str];
        string y = loadIndex(s, idx.prefix(), type);
        if (val.lo() != m_nada)
            Asm("lda " + val.lo());

        if (y == "y")
            Asm("sta (" + res.str + ")," + y);
        else
            Asm("sta " + res.str + "," + y);

        if (type == "uint16") {
            Asm("lda " + val.hi());
            if (y == "y") {
                Asm("iny");
                Asm("sta (" + res.str + ")," + y);
            } else
                Asm("sta " + res.str + "+1," + y);

            Asm("sta " + val.prefix() + "+1");
        }
    }
    if (opcode == m_asmToOpcode["load"]) {
        // store_p ptr idx val
        auto type = m_symtab[res.str];
        string y = loadIndex(s, idx.prefix(), type);
        //        Asm(" ; type : " + type);
        if (y == "y")
            Asm("lda (" + res.str + ")," + y);
        else
            Asm("lda " + res.str + "," + y);

        Asm("sta " + val.prefix());

        // If store type is 16 bit
        if (is16bit(val)) {
            // if load type is 16 bit
            if (is16bit(res)) {
                if (y == "y") {
                    Asm("iny");
                    Asm("lda (" + res.str + ")," + y);
                } else
                    Asm("lda " + res.str + "+1," + y);
            } else
                Asm("lda #0 ; loading uint8, storing uint16");

            Asm("sta " + val.prefix() + "+1");
        }
    }
}

void CPU6502::Const(int &pos) {
    auto name = getNextParam(m_data, pos);
    auto value = getNextParam(m_data, pos);
    Asm(name.str + " = " + Util::toHex(value.ival));
    m_symtab[name.str] = m_opcodeToAsm[value.type];
}

void CPU6502::Declare(int &pos) {
    auto name = getNextParam(m_data, pos);
    auto value = getNextParam(m_data, pos);
    if (isRegister(name.str)) {
        m_symtab[name.str] = m_opcodeToAsm[value.type];
        return;
    }
    if (m_opcodeToAsm[value.type].starts_with("address")) {
        Asm(name.str + "\t=\t" + "0x" + value.str);

    } else if (m_symtab[name.str].starts_with("ptr")) {
        Asm(name.str + "\t=\t" + to_string(m_curZp));
        m_curZp += 2;

    } else {
        Label(name.str, m_typeTripeToNative[m_opcodeToAsm[value.type]] + "\t" +
                            "$" + value.str);
    }
    m_symtab[name.str] = m_opcodeToAsm[value.type];
}

void CPU6502::Mulu(int &pos) {
    auto ret = getNextParam(m_data, pos);
    auto a = getNextParam(m_data, pos);
    auto b = getNextParam(m_data, pos);

    if (!(is16bit(a) || is16bit(b) || is16bit(ret))) {
        addCode("mul8");
        //        cout << m_opcodeToAsm[a.type] << "  " << b.str << endl;
        Asm("ldx " + b.prefix());
        Asm("lda " + a.prefix());
        Asm("jsr mul_8bit_");
        Asm("stx " + ret.lo());
        if (is16bit(ret)) {
            Asm("lda #0");
            Asm("sta " + ret.hi());
        }
        return;
    }
    addCode("mul16");
    if (is16bit(b) && !is16bit(a))
        swap(a, b);
    Asm("lda " + a.lo());
    Asm("ldy " + a.hi());
    Asm("sta mul16x8_num1");
    Asm("sty mul16x8_num1Hi");
    Asm("lda " + b.lo());
    Asm("sta mul16x8_num2");
    Asm("jsr mul_16bit");
    Asm("sta " + ret.lo());
    if (is16bit(ret))
        Asm("sty " + ret.hi());
    //        Asm( "ldy #0");
}
void CPU6502::Divu(int &pos) {
    auto ret = getNextParam(m_data, pos);
    auto a = getNextParam(m_data, pos);
    auto b = getNextParam(m_data, pos);

    if (!(is16bit(a) || is16bit(b))) {

        addCode("div8");
        Asm("lda " + a.prefix());
        Asm("sta div8x8_d");
        Asm("lda " + b.prefix());
        Asm("sta div8x8_c");
        Asm("jsr div_8bit_");
        Asm("sta " + ret.prefix());
        return;
    }
    addCode("div16");

    //    Asm("ldy #0");
    Asm("lda " + a.lo());
    Asm("ldy " + a.lhi());
    Asm("sta initdiv16x8_dividend");
    Asm("sty initdiv16x8_dividend+1");
    Asm("lda " + b.lo());
    Asm("ldy " + b.lhi());

    Asm("sta initdiv16x8_divisor");
    Asm("sty initdiv16x8_divisor+1");
    Asm("jsr div_16bit");
    Asm("lda initdiv16x8_dividend");
    Asm("ldy initdiv16x8_dividend+1");
    Asm("sta " + ret.lo());
    if (is16bit(ret))
        Asm("sty " + ret.hi());
}

void CPU6502::Binop(int &pos, int opcode) {
    auto res = getNextParam(m_data, pos);
    auto a = getNextParam(m_data, pos);
    auto b = getNextParam(m_data, pos);

    string op = m_typeTripeToNative[m_opcodeToAsm[opcode]];
    // Inc / dec
    if (op == "adc" && res.str == a.str && b.str == "1" && !is16bit(res)) {
        Asm("inc " + res.str);
        return;
    }
    if (op == "sbc" && res.str == a.str && b.str == "1" && !is16bit(res)) {
        Asm("dec " + res.str);
        return;
    }

    if (op == "asl" || op == "lsr") {
        if (is16bit(a) || is16bit(b)) {

            bool useTemp = !is16bit(res);
            auto resKeep = res;
            if (useTemp) {
                res.str = "$F0";
                res.typeName = "uint16";
            }

            Asm("lda " + a.lo());
            Asm("sta " + res.lo());
            Asm("lda " + a.lhi());
            Asm("sta " + res.hi());
            string one = "asl " + res.lo();
            string two = "rol " + res.hi();
            if (op == "lsr") {
                one = "lsr " + res.hi();
                two = "ror " + res.lo();
            }
            //            cout << b.ival << endl;
            if (b.ival != 0) // fixed number, unroll
                for (int i = 0; i < b.ival; i++) {
                    Asm(one);
                    Asm(two);
                }
            else {
                auto loop = getTempLabel();
                auto cancel = getTempLabel();
                Asm("ldx " + b.str);
                Asm("cpx #0");
                Asm("beq " + cancel);
                Label(loop);
                Asm(one);
                Asm(two);
                Asm("dex");
                Asm("cpx #0");
                Asm("bne " + loop);
                Label(cancel);
            }
            if (useTemp) {
                Asm("lda " + res.str);
                Asm("sta " + resKeep.lo());
            }

            return;
        }

        Asm("lda " + a.lo());
        if (b.ival != 0) {
            for (int i = 0; i < b.ival; i++)
                Asm(op);
        } else {
            auto loop = getTempLabel();
            auto cancel = getTempLabel();
            Asm("ldx " + b.str);
            Asm("cpx #0");
            Asm("beq " + cancel);
            Label(loop);
            Asm(op);
            Asm("dex");
            Asm("cpx #0");
            Asm("bne " + loop);
            Label(cancel);
        }

        Asm("sta " + res.prefix());
        if (is16bit(res)) {
            Asm("ldx #0 ; make sure hi bit is set");
            Asm("stx " + res.hi());
        }
        return;
    }
    Asm("lda " + a.lo());

    //        std::cout << " tst " << (int)opcode  <<  " " <<op<< " "
    //        <<m_opcodeToAsm[opcode] << " " << (int)a.ival << " " << b.prefix()
    //        <<std::endl;
    if (op == "lsr") {
        //          std::cout << "shlll  " << b.ival<<std::endl;
        for (int i = 0; i < b.ival; i++)
            Asm("lsr");
        Asm("sta " + res.prefix());
        return;
    }

    if (op == "adc")
        Asm("clc");
    if (op == "sbc")
        Asm("sec");

    Asm(op + " " + b.prefix());
    Asm("sta " + res.prefix());
    //        std::cout << "BINOP : " <<a.prefix() << " " <<b.prefix() << " " <<
    //        (int)(a.type==m_asmToOpcode["uint16"])<< " "
    //        <<(int)(b.type==m_asmToOpcode["uint16"]) << endl; std::cout << "
    //        Type : " << m_symtab[a.prefix()] << " "
    //        <<(int)(b.type==m_asmToOpcode["uint16"]) << endl;
    bool ab16bit =
        is16bit(a) || is16bit(b) || a.isRef() || b.isRef() || is16bit(res);

    if (ab16bit) {

        //          Error::RaiseError("Add / sub doesn't work with 16 bit yet");
        Asm("lda " + a.hi());
        Asm(op + " " + b.hi());
        //
        Asm("sta " + res.prefix() + "+1");
    }
}

void CPU6502::Mov(int &pos) {
    auto res = getNextParam(m_data, pos);
    auto val = getNextParam(m_data, pos);

    if (m_symtab.contains(res.str) && is16bit(res) || val.isRef() ||
        is16bit(val)) {

        if (is16bit(res)) {
            if (val.str != m_nada)
                Asm("ldy " + val.lhi());

            Asm("sty " + res.hi());
        }
        if (val.lo() != m_nada)
            Asm("lda " + val.lo());

        Asm("sta " + res.str);

    } else {

        if (val.str != m_nada)
            Asm("lda " + val.prefix());
        Asm("sta " + res.str);
    }
}

void CPU6502::Branch(int &pos, int opcode) {
    auto a = getNextParam(m_data, pos);
    auto b = getNextParam(m_data, pos);
    auto lbl = getNextParam(m_data, pos);

    int size = 0;
    bool isOffpage = false;
    if (m_pass == 1) {
        size = branchSizeEstimator(lbl.str, m_curBranch);
        isOffpage = size > 120;
    }
    string lblKeep = lbl.str;
    if (isOffpage)
        lbl.str = getTempLabel();

    bool is16 = is16bit(a) || is16bit(b);

    if (m_symtab.contains(a.str) && !is16bit(a))
        is16 = false;

    if (!is16) {
        if (a.str != m_nada)
            Asm("lda " + a.prefix());

        Asm("cmp " + b.prefix());

        if (opcode == m_asmToOpcode["jeq"])
            Asm("beq " + lbl.str);
        if (opcode == m_asmToOpcode["jneq"])
            Asm("bne " + lbl.str);

        if (opcode == m_asmToOpcode["jgt"]) {
            auto lblOK = getTempLabel();
            Asm("beq " + lblOK);
            Asm("bcs " + lbl.str);
            Label(lblOK);
        }
        if (opcode == m_asmToOpcode["jgte"]) {
            Asm("bcs " + lbl.str);
        }
        if (opcode == m_asmToOpcode["jlt"])
            Asm("bcc " + lbl.str);
        if (opcode == m_asmToOpcode["jlte"]) {
            Asm("bcc " + lbl.str);
            Asm("beq " + lbl.str);
        }

    } else {
        auto lblDone = getTempLabel();

        if (opcode == m_asmToOpcode["jneq"]) {
            Asm("ldx " + a.hi());
            Asm("lda " + a.lo());
            Asm("cmp " + b.lo());

            Asm("bne " + lbl.str);
            Asm("cpx " + b.hi());
            Asm("bne " + lbl.str);
        }
        if (opcode == m_asmToOpcode["jeq"]) {
            Asm("ldx " + a.hi());
            Asm("lda " + a.lo());
            Asm("cmp " + b.lo());
            Asm("bne " + lblDone);
            Asm("cpx " + b.hi());
            Asm("beq " + lbl.str);
            Label(lblDone);
        }

        if (opcode == m_asmToOpcode["jgt"] || opcode == m_asmToOpcode["jgte"]) {
            Asm("; Integer Greater");
            Asm("lda " + a.hi() + "   ; compare high bytes");
            Asm("cmp " + b.hi() + " ;keep");
            Asm("bcc  " + lblDone);
            Asm("bne " + lbl.str);
            Asm("lda " + a.lo());
            Asm("cmp " + b.lo() + " ;keep");
            if (opcode == m_asmToOpcode["jgt"])
                Asm("beq " + lblDone);
            Asm("bcs " + lbl.str);
            Label(lblDone);
        }

        if (opcode == m_asmToOpcode["jlt"] || opcode == m_asmToOpcode["jlte"]) {
            Asm("; Integer Less");
            Asm("lda " + a.hi() + "   ; compare high bytes");
            Asm("cmp " + b.hi() + " ;keep");
            Asm("bcc " + lbl.str);
            Asm("bne " + lblDone);
            Asm("lda " + a.lo());
            Asm("cmp " + b.lo() + " ;keep");
            Asm("bcc " + lbl.str);
            if (opcode == m_asmToOpcode["jlte"])
                Asm("beq " + lbl.str);
            Label(lblDone);
        }
    }

    if (m_pass == 0) {
        vector<string> lst;
        m_branches.push_back(m_curLine + Util::split(m_line, '\n', lst).size() +
                             1);
    }

    if (isOffpage) {
        string cont = getTempLabel();
        Asm("; branch is offpage");
        Asm("jmp " + cont);
        Label(lbl.str);
        Asm("jmp " + lblKeep);
        Label(cont);
    }
    m_curBranch++;

    /*
    if (m_pass == 1)
        cout << "isoffpage : " << isOffpage << "  : " << size
             << "   label : " << lblKeep << endl;
             */
}

int CPU6502::estimateCodeSize(const string &s) {
    //    std::cout << s << endl;
    //   std::cout << " 1 " << s << endl;
    if (Util::trim(s) == "")
        return 0; // nada
    if (!s.starts_with("\t"))
        return 0;
    if (Util::trim(s).starts_with(";"))
        return 0; // comment
    if (s.find("=") != string::npos)
        return 0; // const

    string v = Util::ReplaceString(s, "\t", " ");
    v = Util::ReplaceString(v, "  ", " ");
    vector<string> lst;
    lst = Util::split(v, ';', lst);
    v = lst[0]; // remove end comments
    lst.clear();
    lst = Util::split(v, ' ', lst);

    vector<string> lst2;
    for (auto &c : lst)
        if (c != "")
            lst2.push_back(c);
    lst = lst2;

    int size = 0;
    //    cout << " ****** OK " << lst.size() << " : " << endl;
    // for (auto s : lst)
    //   cout << "'" << s << "'" << endl;

    if (lst.size() == 1)
        size = 1; // single byte op

    if (lst.size() >= 2) {
        string p = lst[1];
        vector<string> ps;

        ps = Util::split(p, ',', ps);
        p = ps[0]; // pick first one, ignore ",x" etc
        if (p.starts_with("#")) {
            size = 2; // const
        } else
            size = 3; // address
    }
    //    cout << " - " << s << " : " << size << "  list size: " << lst.size()
    //       << endl;
    //   for (auto c : lst)
    //      cout << "  *** " << c << endl;
    //  std::cout << " 3 " << endl;
    return size;
}
} // namespace tripe