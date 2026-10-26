#include "cpuz80.h"
#include "error.h"
#include "opcodes.h"
/*
#include "resources/div16_Z80.h"
#include "resources/div8_Z80.h"
#include "resources/mul16_Z80.h"
*/

#include "resources/mul8_z80.h"
#include "resources/mul16_z80.h"
#include "resources/div8_z80.h"
#include "resources/div16_z80.h"

#include "tinyexpr.h"
#include "tripedata.h"
#include <algorithm>
using namespace tripe;
namespace tripe {

CPUZ80::CPUZ80() : AbstractCPU() {
    //    AbstractCPU();
    Init(Data::d.opcodes);
    m_symtab.clear();
    m_hexprefix = "$";
    m_registeredProcessor ="z80";
    m_typeTripeToNative["uint8"] = "dc.b";
    m_typeTripeToNative["uint16"] = "dc.w";
    m_typeTripeToNative["int8"] = "dc.b";
    m_typeTripeToNative["int16"] = "dc.w";

    m_typeTripeToNative["add"] = "adc";
    m_typeTripeToNative["sub"] = "sbc";
    m_typeTripeToNative["or"] = "or";
    m_typeTripeToNative["and"] = "and";
    m_typeTripeToNative["xor"] = "xor";

    m_typeTripeToNative["shr"] = "shr";
    m_typeTripeToNative["shl"] = "shl";

    m_typeTripeToNative["jump"] = "jp";
    m_typeTripeToNative["call"] = "call";
    m_typeTripeToNative["fcall"] = "jsr";

    m_typeTripeToNative["return"] = "ret";
    m_typeTripeToNative["rti"] = "reti";

    m_code["mul8"] = string((char *)resources_z80_mul8_asm);
    m_code["mul16"] = string((char *)resources_z80_mul16_asm);
    m_code["div8"] = string((char *)resources_z80_div8_asm);
    m_code["div16"] = string((char *)resources_z80_div16_asm);

}


void CPUZ80::InsertTempValues(vector<string> &lst, int pos) {
    for (auto s : m_registersUsed) {
    }
}


vector<string> CPUZ80::stub(map<string, string> params) {
    vector<string> src;
    /*
    string startAddress = "";
    string printAddress = "";

    bool print = false;
    int istart = 0;
    if (params.contains("start_address"))
        startAddress = params["start_address"];


    if (params.contains("sys"))
        if (params["sys"] == "c64" || params["sys"] == "vic20") {
            if (startAddress == "")
                istart = m_foundStartPos;

        }
    */
    return src;
}



void CPUZ80::LoadStore(int &pos, int opcode) {
    string s = "";
    auto res = getNextParam(m_data, pos);
    auto idx = getNextParam(m_data, pos);
    auto val = getNextParam(m_data, pos);
    if (isPointer(res))
        Asm("ld hl,"+res.z80());
    else
        Asm("ld hl,"+res.clean());

    if (is16bit(idx)) {
        Asm("ld de,"+idx.z80());
    }
    else {
        Asm("ld a,"+idx.z80());
        Asm("ld e,a");
        Asm("ld d,0");

    }
    Asm("Add hl,de");
    if (is16bitArray(res))
        Asm("Add hl,de ; 16 bit array");

    if (opcode == m_asmToOpcode["store"]) {
        if (!is16bitArray(res)) {
            Asm("ld a,"+val.z80());
            Asm("ld [hl],a");
        }
        else {
            Asm("ld a,"+val.z80lo());
            Asm("ld [hl],a");
            Asm("inc hl");
            Asm("ld a,"+val.z80hi());
            Asm("ld e,a");
            Asm("ld [hl],a");
        }
    }
    if (opcode == m_asmToOpcode["load"]) {
        if (!is16bitArray(res)) {
            Asm("ld a,[hl]");
            Asm("ld "+val.z80()+",a");
        }
        else {
//            Asm("ld de,[hl]");
  //          Asm("ld "+val.z80()+",de");
            
            Asm("ld a,[hl]");
            Asm("ld "+val.z80lo()+",a");
            Asm("inc hl");
            Asm("ld a,[hl]");
            Asm("ld "+val.z80hi()+",a");
        }
    }
}

void CPUZ80::Const(int &pos) {
    auto name = getNextParam(m_data, pos);
    auto value = getNextParam(m_data, pos);
    Asm(name.str + " = " + m_hexprefix + Util::toHex(value.ival));
    m_symtab[name.str] = m_opcodeToAsm[value.type];
}

void CPUZ80::Declare(int &pos) {
    auto name = getNextParam(m_data, pos);
    auto value = getNextParam(m_data, pos);
    bool isString = value.flag == Opcodes::DATATYPE_STRING;
    string hex = m_hexprefix;
    if (isString)
        hex = "";

    if (m_opcodeToAsm[value.type].starts_with("address")) {
        Asm(name.str + "\t=\t" + hex + value.str);

    } else if (m_symtab[name.str].starts_with("ptr")) {
        Label(name.str, m_typeTripeToNative["uint16"] + "\t" + hex + value.str);
 
    } else {
        Label(name.str, m_typeTripeToNative[m_opcodeToAsm[value.type]] + "\t" + hex + value.str);
    }
    if (m_decltab.contains(name.str)) {
        Error::RaiseError("Z80 codegen: symbol '" + name.str + "' already defined");
    }
    m_decltab[name.str] = name.str;
    m_symtab[name.str] = m_opcodeToAsm[value.type];
}

void CPUZ80::Mulu(int &pos) {
    auto ret = getNextParam(m_data, pos);
    auto a = getNextParam(m_data, pos);
    auto b = getNextParam(m_data, pos);


    if (is16bit(a) || is16bit(b)) {
        addCode("mul16");
        if (is16bit(b) && !is16bit(a))
            swap(a,b);

        Asm("ld de,"+a.z80());
        Asm("ld a,"+b.z80());
        Asm("ld hl,0");
        Asm("ld c,0");
        Asm("call mul_16bit_");
        Asm("ld "+ret.z80()+",hl");

    }
    else {
        addCode("mul8");
        // 8x8 mul
        Asm("ld a,"+a.z80());
        Asm("ld e,a");
        Asm("ld d,0");
        Asm("ld a,"+b.z80());
        Asm("ld h,a");
        Asm("ld l,0");
        Asm("call mul_8bit_");
        Asm("ld a,l");
        Asm("ld "+ret.z80()+",a");
        if (is16bit(ret)) {
            Asm("ld a,h");
            Asm("ld "+ret.z80hi()+",a");
        }

    }


}

void CPUZ80::Divu(int &pos) {
    auto ret = getNextParam(m_data, pos);
    auto a = getNextParam(m_data, pos);
    auto b = getNextParam(m_data, pos);

    if (is16bit(a) && is16bit(b)) {
        addCode("div16");
       Asm("ld de,"+b.z80());
        Asm("ld hl,"+a.z80());
        Asm("ld a,h");
        Asm("ld c,l");
        Asm("call div_16bit_");
        Asm("ld h,a");
        Asm("ld l,c");
        if (is16bit(ret))
            Asm("ld "+ret.z80()+",hl");
        else {
            Asm("ld a,l");
            Asm("ld "+ret.z80lo()+",a");
        }


    }
    else {
        addCode("div8");
        // 8x8 mul
        Asm("ld a,"+b.z80());
        Asm("ld c,a");
        Asm("ld hl,"+a.z80());
        if (!is16bit(a))
            Asm("ld h,0");
        Asm("call div_8bit_");
        if (is16bit(ret)) 
            Asm("ld "+ret.z80()+",hl");
        else {
            Asm("ld a,l");
            Asm("ld "+ret.z80()+",a");
        }

    }


}



void CPUZ80::Binop(int &pos, int opcode) {
    auto res = getNextParam(m_data, pos);
    auto a = getNextParam(m_data, pos);
    auto b = getNextParam(m_data, pos);

   string op = m_typeTripeToNative[m_opcodeToAsm[opcode]];
    bool isAdc = op=="adc" || op=="sbc";    


    if (op=="shr" || op=="shl") {
        shrshl(op,res,a,b);
        return;
    }
 

    if (!is16bit(b) && !is16bit(res) && !(is16bit(a))) {
        if (isAdc) {
            Asm("xor a");
        }
        if (b.isPureNumber()) {
            Asm("ld a,"+a.z80());
            if (isAdc)
                Asm(op + " a,"+b.z80());
            else
                Asm(op + " "+b.z80());
        }
        else {
            Asm("ld a,"+b.z80());

            Asm("ld b,a");
            Asm("ld a,"+a.z80());
            if (isAdc)
               Asm(op + " a,b");
           else
               Asm(op + " b");
            
        }
        Asm("ld "+res.z80()+",a");
    }



    if (is16bit(res)) {
        Asm("ld hl,"+b.z80());
        if (!is16bit(b))
            Asm("ld h,0");
        Asm("push hl");
        Asm("ld hl,"+a.z80());
        if (!is16bit(a))
            Asm("ld h,0");
        Asm("pop de");
        if (op=="adc") {
            Asm("add hl,de");
        }
        else if (op=="sbc") {
            Asm("xor a ; clear carry");
            Asm("sbc hl,de");
        }
        else Error::RaiseError("z80 codegen: 16 bit operation not implemented yet :"+op);
//        Asm("ex de,hl");
        Asm("ld "+res.z80()+",hl");
    }

}
void CPUZ80::shrshl(const string& op, Param& res, Param& a, Param& b) {
   if (is16bit(a) || is16bit(res)) {
//            Error::RaiseError("z80 codegen: 16 bit shift not implemented yet");
        Asm("ld hl,"+a.z80());
        if (!is16bit(a))
            Asm("ld h,0"); // if a is 8 bit
        if (b.isPureNumber()) {
            for (int i=0;i<Util::fromNumber("$"+b.str);i++) {
                if (op=="shr") {
                    Asm("srl h");
                    Asm("rr l");
                }
                else
                    Asm("add hl,hl");

            }
        }
        else {
            string lblSkip = getTempLabel();
            Asm("ld a,"+b.z80());
            Asm("cp $0");
            Asm("jr z,"+lblSkip);
            Asm("ld c,a");
            string lbl = getTempLabel();
            Label(lbl);
            if (op=="shr") {
                Asm("srl h");
                Asm("rr l");
            }
            else
                Asm("add hl,hl");

            Asm("dec c");
            Asm("jr nz,"+lbl);
            Label(lblSkip);


        }
        if (is16bit(res)) 
            Asm("ld "+res.z80()+",hl");
        else {
            Asm("ld a,l");
            Asm("ld "+res.z80()+",a");
        }


    }
    else // 8 bit 
    {
        if (b.isPureNumber()) {
            Asm("ld a,"+a.z80());
            for (int i=0;i<Util::fromNumber("$"+b.str);i++) {
                if (op=="shr")
                    Asm("rrca");
                else
                    Asm("rlca");

            }
            Asm("ld "+res.z80()+",a");
        }
        else {
            string lblSkip = getTempLabel();
            Asm("ld a,"+b.z80());
            Asm("cp $0");
            Asm("jr z,"+lblSkip);
            Asm("ld c,a");
            Asm("ld a,"+a.z80());
            string lbl = getTempLabel();
            Label(lbl);
            if (op=="shr")
                Asm("rrca");
            else
                Asm("rlca");
            Asm("dec c");
            Asm("jr nz,"+lbl);
            Asm("ld "+res.z80()+",a");
            Label(lblSkip);

        }
    }
    return;

}


/*
void CPUZ80::LoadHL(const Param& p) {

}
*/
void CPUZ80::Mov(int &pos) {
    auto res = getNextParam(m_data, pos);
    auto val = getNextParam(m_data, pos);


//    cout << res.str << " " << is16bit(res)<< " " << res.typeName<<endl;
    if (!is16bit(res) && !is16bit(val)) {
        Asm("ld a,"+val.z80());
        Asm("ld "+res.z80()+",a");
    }
    if (is16bit(res) && is16bit(val)) {
        Asm("ld hl,"+val.z80()+"");
        Asm("ld "+res.z80()+",hl");
    }
    if (is16bit(res) && !is16bit(val)) {
        Asm("ld hl,"+val.z80());
        Asm("ld h,0");
        Asm("ld "+res.z80()+",hl");
    }
    if (!is16bit(res) && is16bit(val)) {
//        Asm("ld hl,"+val.z80());
//        Asm("ld a,l");
        Asm("ld a,"+val.z80lo());
        Asm("ld "+res.z80()+",a");
    }


}

bool CPUZ80::printCmp(const string &val) {
    if (!(val == "#0" || val == "#$0" || val == "#$00" || val == "#00"))
        return true;

    return true;
}

void CPUZ80::Branch(int &pos, int opcode) {
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

    string p = "r";
    if (isOffpage)
        p = "p";

    string bcs ="j"+p+" c,";
    string bcc ="j"+p+" nc,";

    if (!is16) {

        string cmp = b.z80();
        if (!b.isPureNumber()) {
            Asm("ld a,"+b.z80());
            Asm("ld b,a");
            cmp = "b";
        }

        if (a.str != m_nada)
            Asm("ld a," + a.z80());

        Asm("cp " + cmp);
    }
    else {
        Asm("ld hl,"+a.z80());        
        if (!is16bit(a))
            Asm("ld h,0");

        Asm("ld de,"+b.z80());        
        if (!is16bit(b))
            Asm("ld d,0");
        Asm("xor a");
        Asm("sbc hl,de");

    }


    string jneq = isOffpage?"j"+p+" z,":"j"+p+" nz,";
    string jeq = isOffpage?"j"+p+" nz,":"j"+p+" z,";

    if (opcode == m_asmToOpcode["jeq"])
        Asm(jeq + lbl.str);
    if (opcode == m_asmToOpcode["jneq"])
        Asm(jneq + lbl.str);

    if (opcode == m_asmToOpcode["jgt"] || opcode == m_asmToOpcode["jgts"]) {
        auto lblOK = getTempLabel();
        Asm(jeq + lblOK);
        Asm(bcc + lbl.str);
        Label(lblOK);
    }
    if (opcode == m_asmToOpcode["jgte"] || opcode == m_asmToOpcode["jgtes"]) {
        Asm(bcc + lbl.str);
    }
    if (opcode == m_asmToOpcode["jlt"] || opcode == m_asmToOpcode["jlts"]) {
        Asm(bcs + lbl.str);

    }
    if (opcode == m_asmToOpcode["jlte"] || opcode == m_asmToOpcode["jltes"]) {
        Asm(bcs + lbl.str);
        Asm(jeq + lbl.str);
    }



    if (m_pass == 0) {
        vector<string> lst;
        m_branches.push_back(m_curLine + Util::split(m_line, '\n', lst).size() + 1);
    }

    if (isOffpage) {
        if (opcode == m_asmToOpcode["jeq"] || opcode == m_asmToOpcode["jneq"]) {
            Asm("; branch is offpage optbranch1");
            Asm("jp " + lblKeep);
            Label(lbl.str);

        } else {
            string cont = getTempLabel();
            Asm("; branch is offpage");
            Asm("jp " + cont);
            Label(lbl.str);
            Asm("jp " + lblKeep);
            Label(cont);
        }
    }
    m_curBranch++;

}

int CPUZ80::estimateCodeSize(const string &s) {
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