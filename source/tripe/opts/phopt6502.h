#ifndef PHOPT6502_P
#define PHOPT6502_P

#include "phopt.h"

using namespace std;

namespace tripe {

// 6973, lda lzombie,x
class Phopt6502 : public Phopt {
  public:
    vector<string> m_bops = {"adc", "sbc", "eor", "and", "or"};

    vector<string> m_aChangingOps = {"ora", "and", "eor", "beq", "bcc", "bcs", "bne", "adc", "sbc",
                                     "asl", "ror", "lsr", "rol", "jsr", "inc", "dec", "tya", "txa"};

    vector<string> m_aChangingOpsForCmp = {"ora", "and", "eor", "beq", "bcc", "bcs", "bne", "adc", "sbc", "asl", "ror", "lsr", "rol", "jsr", "tya", "txa", "lda"};
//    vector<string> m_xChangingOps = {"inx", "dex"};

    enum Type { BOP1, BOP2, LDASTA, LDALDXLDA, LDASTA2, LDA, INCDEC, LDXSTA, CMP2 };

    bool is16bit(string v) {
        vector<string> lst;
        lst = Util::split(v, ',', lst);
        auto s = m_symTab[lst[0]];
        return (s == "uint16" || s.starts_with("ptr") || s == "address16");
    }

    vector<string> optimize(vector<std::string> in) override;
    /*
        void ldxsta(vector<vector<string>> &line, vector<string> &l, int &cur,
                    vector<string> &src);*/
    void incdec(vector<vector<string>> &line, vector<string> &l, int &cur, vector<string> &src);
    void Bop1(vector<vector<string>> &line, vector<string> &l, int &cur, vector<string> &src);
    void Bop2(vector<vector<string>> &line, vector<string> &l, int &cur, vector<string> &src);
    void ldasta(vector<vector<string>> &line, vector<string> &l, int &cur, vector<string> &src);
    void ldasta2(vector<vector<string>> &line, vector<string> &l, int &cur, vector<string> &src);
    void ldaldxlda(vector<vector<string>> &line, vector<string> &l, int &cur, vector<string> &src);
    void cmp2(vector<vector<string>> &line, vector<string> &l, int &cur, vector<string> &src);
    void ldX(string cmd);
    void ldA();
    void cmp();
    void ldAtaX(string cmd);
    void ldxAlone(string cmd);

    void Opt(Type type, int noLinesToCheck);
};
} // namespace tripe

#endif
