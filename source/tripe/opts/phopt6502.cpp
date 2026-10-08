#include "opts/phopt6502.h"

using namespace tripe;
namespace tripe {

vector<string> Phopt6502::optimize(vector<string> in) {

    m_src = in;
    // return m_src;
    Opt(LDASTA, 2);
    Opt(LDASTA2, 3);
    Opt(LDALDXLDA, 3);
    Opt(BOP1, 6);
    Opt(BOP2, 3);
    Opt(INCDEC, 4);
    ldX("y");
    ldX("x");
    ldA();
    return m_src;
}

void Phopt6502::ldX(string cmd) {
    string curVal = "";
    bool isDone = false;
    vector<string> src{};
    string tax = "ta" + cmd;
    string inx = "in" + cmd;
    string dex = "de" + cmd;
    string xy = cmd;
    cmd = "ld" + cmd;
    m_curLine = 0;
    for (int i = 0; i < m_src.size(); i++) {

        bool remove = false;
        string l1 = "";
        auto line1 = getLine(i);

        if (line1.size() != 0) {
            auto op1 = Util::toLower(line1[0]);
            if (line1[0] == ".gcode")
                remove = true;

            if (cmd == "ldx") {
                //                cout << line1[0] << " : " << curVal << endl;
            }

            if (curVal != "")
                if (op1.find(":") != std::string::npos || op1 == tax ||
                    /*op1 == "jmp" || */ op1 == "bne" || op1 == "beq" ||
                    op1 == dex || op1 == inx || op1 == "jsr" ||
                    op1 == "is_label") {
                    //		cout << "Reset "<<curVal<< " "
                    //<< op1<<endl;
                    curVal = "";
                    // cout << "RESET " << endl;
                    //				src.push_back(";
                    //  reset "+cmd + "  " + op1);
                }

            if (op1 == cmd) {
                if (curVal != line1[1]) {
                    curVal = line1[1];
                } else {
                    /*
                                        cout << " *** OPT  '" << m_src[i] << "'
                       l0: '" << op1
                                             << "' l1: '" << line1[1] << "'
                       curval:" << curVal
                                             << endl;
                                             */

                    src.push_back(" ; opt5 " + l1);
                    remove = true;
                    s_optLines += 1;

                    // src.push_back(" "+cmd+" "+curVal);
                }

            } else
                // replace "sta arr,x" with "sta arr+x" when x is a const
                if (line1.size() == 2)
                    if (curVal != "" && Util::isPureNumber(curVal))
                        if (op1 == "sta" || op1 == "lda" || op1 == "adc" ||
                            op1 == "sbc" || op1 == "ora" || op1 == "eor" ||
                            op1 == "asl" || op1 == "lsr" || op1 == "ror" ||
                            op1 == "rol") {

                            if (line1[1].find("(") == string::npos) {
                                vector<string> lst;
                                lst = Util::split(line1[1], ',', lst);
                                if (lst.size() == 2 && lst[1] == xy) {
                                    string add =
                                        Util::ReplaceString(curVal, "#", "");
                                    src.push_back(tab + op1 + tab + lst[0] +
                                                  "+" + add);
                                    src.push_back("; opt6 :" + line1[0] + " " +
                                                  line1[1]);
                                    remove = true;
                                    s_optLines += 1;
                                }
                            }
                        }
        }
        if (!remove)
            src.push_back(m_src[i]);
    }

    m_src = src;
}

void Phopt6502::ldA() {
    string curVal = "";
    bool isDone = false;
    vector<string> src{};

    m_curLine = 0;
    for (int i = 0; i < m_src.size(); i++) {

        bool remove = false;
        string l1 = "";
        auto line1 = getLine(i);
        if (line1.size() >= 3) {
            // oops, we have +x stuff
            /*            for (auto c : line1)
                            cout << "'" << c << "' ";
                        cout << endl;*/
            for (int i = 3; i < line1.size(); i++)
                line1[1] += " " + line1[i];
        }
        if (line1.size() != 0) {
            auto op1 = Util::toLower(line1[0]);
            if (curVal != "")

                if (op1 == "is_label" || op1.find(":") != std::string::npos ||
                    contains(m_aChangingOps, op1) || line1.size() == 1 ||
                    line1[1].find(",") != string::npos) {
                    curVal = "";
                }

            if (op1 == "lda" /* && (line1[1].find(",") == string::npos)*/) {
                //                cout << m_src[i] << ":" << line1[0] << " " <<
                //                curVal << ":"
                //                   << line1[1] << endl;
                if (curVal == "") {
                    curVal = line1[1];
                } else if (curVal != line1[1]) {
                    curVal = line1[1];
                } else {
                    if (m_src[i].find(";keep") == string::npos) {
                        src.push_back(" ; optlda1, remove:" + m_src[i]);
                        remove = true;
                        //  cout << "************************ LDA REMOVAL " <<
                        //  endl;
                        s_optLines += 1;
                    }

                    // src.push_back(" "+cmd+" "+curVal);
                }
            }
        }
        if (!remove)
            src.push_back(m_src[i]);
    }

    m_src = src;
}

void Phopt6502::ldaldxlda(vector<vector<string>> &line, vector<string> &l,
                          int &cur, vector<string> &src) {
    /*
    sta varPrefixed_c
    ldy i
    lda varPrefixed_c
*/
    if (line.size() != 3)
        return;
    if (l.size() != 3)
        return;
    auto line3 = line[2];
    auto line2 = line[1];
    auto line1 = line[0];
    auto l1 = l[0];
    auto l2 = l[1];
    auto l3 = l[2];
    if (line1.size() == 2 && line2.size() == 2 && line3.size() == 2) {
        auto op1 = Util::toLower(line1[0]);
        auto op2 = Util::toLower(line2[0]);
        auto op3 = Util::toLower(line3[0]);

        if (line1[0] == "sta" && line3[0] == "lda" && line1[1] == line3[1] &&
            !line1[1].ends_with(",x") && !line1[1].ends_with(",y")) {
            if (line2[0] == "ldy" || line2[0] == "ldx") {
                src.push_back(";opt4");
                if (!line1[1].starts_with("_"))
                    src.push_back(l1 + " ; opt ldxldy");
                else
                    s_optLines += 1;

                s_optLines += 1;
                src.push_back(l2);
                cur = m_curLine;
            }
        }
    }
}
void Phopt6502::incdec(vector<vector<string>> &line, vector<string> &l,
                       int &cur, vector<string> &src) {

    /*
        lda sfx_sfxType_sfxType_timer,x
        sec
        sbc #$1
        sta sfx_sfxType_sfxType_timer,x
    */
    if (line[0].size() == 2 && line[2].size() == 2 && line[3].size() == 2)
        if (line[0][0] == "lda" && line[3][0] == "sta" &&
            line[0][1] == line[3][1]) {
            if (line[2][0] == "sbc" &&
                (line[2][1] == "#1" ||
                 line[2][1] == "#$1" && line[0][1].find("(") == string::npos) &&
                !is16bit(line[0][1])) {
                cur = m_curLine;
                src.push_back(tab + "dec" + tab + line[0][1] + ";incdec opt");
                s_optLines += 4;
            }
            if (line[2][0] == "adc" &&
                (line[2][1] == "#1" ||
                 line[2][1] == "#$1" && line[0][1].find("(") == string::npos) &&
                !is16bit(line[0][1])) {
                cur = m_curLine;
                src.push_back(tab + "inc" + tab + line[0][1] + ";incdec opt");
                s_optLines += 4;
            }
        }
}

void Phopt6502::ldasta2(vector<vector<string>> &line, vector<string> &l,
                        int &cur, vector<string> &src) {

    /*
        sta _r8_1
        ldy #$0
        lda _r8_1
    */
    if (line[0].size() == 2 && line[1].size() == 2 && line[2].size() == 2)
        if (line[0][0] == "sta" && line[2][0] == "lda" &&
            line[0][1] == line[2][1] && isTemp(line[0][1])) {
            if (line[1][0] == "ldy" || line[1][0] == "ldx") {
                cur = m_curLine;
                src.push_back(l[1] + " ; opt ldasta2");
                s_optLines += 2;
            }
        }
}

void Phopt6502::ldasta(vector<vector<string>> &line, vector<string> &l,
                       int &cur, vector<string> &src) {
    /*
    sta t_uint8_3
    lda t_uint8_3

bug:

    lda (src),y
    sta t_uint8_load1
    lda t_uint8_load1
    sta div8x8_d


becomes

    lda (src),y
    lda t_uint8_load1
    sta div8x8_d


    */

    auto line2 = line[1];
    auto line1 = line[0];
    auto l1 = l[0];
    auto l2 = l[1];

    if (line2.size() != 0 && line1.size() != 0) {

        auto op1 = Util::toLower(line1[0]);
        auto op2 = Util::toLower(line2[0]);
        //      std::cout <<Util::toLower(line1[0])  << " "
        //<<Util::toLower(line2[0]) << endl;
        if (op1 == "sta" && (op2.starts_with("ld"))) {
            char cmd = op2.at(2);
            //              cout
            //<< cmd << endl;
            if (line2[1] != "")
                if ((line2[1] == line1[1]) && !line2[1].starts_with("$")) {
                    if (cmd == 'a' &&
                        m_src[m_curLine].find(";keep") == string::npos) {
                        if (isTemp(line1[1])) {
                            cur = m_curLine;
                            src.push_back("; stalda opt 1");
                            s_optLines += 2;

                        } else {
                            // cout << line1[1] << ": " << line2[1] << endl;
                            src.push_back(l1);
                            src.push_back("; stalda opt 2");
                            cur = m_curLine;
                            s_optLines += 1;
                        }
                    } else {
                        src.push_back("; stalda opt3 ");
                        if (cmd == 'x')
                            src.push_back("\ttax");
                        else
                            src.push_back("\ttay");
                        s_optLines += 1;

                        cur = m_curLine;
                        if (!isTemp(line1[1])) {
                            src.push_back(l1);
                        } else
                            s_optLines += 1;
                    }
                }
        }
    }
}

void Phopt6502::Bop2(vector<vector<string>> &line, vector<string> &l, int &cur,
                     vector<string> &src) {

    /*
    sta _r8_4
    lda _r8_3
    clc
    adc _r8_4

    should just be
    clc
    adc _r8_3


    sta _r8_4
    lda _r8_3
    adc _r8_4

    */

    if (!(line[0].size() == 2 && line[1].size() == 2))
        return;

    string sec = "";
    if (line[2][0] == "sec" || line[2][0] == "clc") {
        sec = l[2];
        string ll;
        line.push_back(getNextLine(ll));
        l.push_back(ll);
        line.erase(line.begin() + 2);
    }

    if (!(line[0][0] == "sta" && line[1][0] == "lda" && isTemp8(line[0][1]) &&
          isTemp8(line[1][1])))
        return;

    if (!(line[0][1] == line[2][1]))
        return;

    // perform opt
    string cmd = line[2][0];

    if (!(/*cmd == "sbc" || */ cmd == "adc" || cmd == "or" || cmd == "and" ||
          cmd == "xor"))
        return;
    /*
        cout << "HERE " << endl;
        int i = 0;
        for (auto s : l) {
            cout << s << endl;
        }
    */
    cur = m_curLine;
    s_optLines += 2;
    // do the opt:
    if (sec != "")
        src.push_back(sec);
    src.push_back(tab + cmd + " " + line[1][1] + " ; bop2 opt ");
}

void Phopt6502::Bop1(vector<vector<string>> &line, vector<string> &l, int &cur,
                     vector<string> &src) {

    /*
     sta t_uint8_load1
  ; opt5
     lda stars_star_star_ddx,x
     sta t_uint8_load2
     lda t_uint8_load1
     sec
     sbc t_uint8_load2


    sta _r8_4
    lda _r8_3
    clc
    adc _r8_4


    sta t_uint8_load1
    ldx c2y
    lda sin,x
    sta t_uint8_load2
    lda t_uint8_load1
    clc
    adc t_uint8_load2



    */
    // first line stores to temp

    if (!(line[0].size() == 2 && line[1].size() == 2 && line[2].size() == 2 &&
          line[3].size() == 2))
        return;

    if (!(line[0][0] == "sta" && isTemp8(line[0][1])))
        return;

    bool hasIndex = false;
    int type = 1;
    string indexLine = "";

    if (line[1][0] == "ldx" || line[1][0] == "ldy") {
        hasIndex = true;
        indexLine = l[1];
        line.erase(line.begin() + 1);
        string s;
        line.push_back(getNextLine(s));
        l.push_back(s);
        type = 2;
    }

    if (!(line[1][0] == "lda" && line[2][0] == "sta" && isTemp8(line[2][1])))
        return;
    if (!(line[3][0] == "lda" && line[3][1] == line[0][1]))
        return;

    int p = 4;
    // perform opt
    if (line[4][0] == "sec" || line[4][0] == "clc")
        p = 5;
    string cmd = line[p][0];
    if (!(cmd == "sbc" || cmd == "adc" || cmd == "or" || cmd == "and" ||
          cmd == "xor"))
        return;

    cur = m_curLine;
    s_optLines += 5;
    // do the opt:
    if (hasIndex)
        src.push_back(indexLine);
    if (cmd == "adc") {
        src.push_back(tab + "clc");
        s_optLines -= 1;
    }
    if (cmd == "sbc") {
        src.push_back(tab + "sec");
        s_optLines -= 1;
    }
    //    cout << "BOP1 Optimized *************** " << type << endl;
    src.push_back(tab + cmd + " " + line[1][1] + " ; bop opt " +
                  to_string(type));
}

void Phopt6502::Opt(Type type, int noLinesToCheck) {
    bool isDone = false;
    vector<string> src;
    m_curLine = 0;

    while (!isDone) {
        int cur = m_curLine;
        vector<vector<string>> line;
        vector<string> l;
        for (int i = 0; i < noLinesToCheck; i++) {
            string s;
            auto tl = getNextLine(s);
            if (tl.size() != 0)
                tl[0] = Util::toLower(tl[0]);
            l.push_back(s);
            line.push_back(tl);
        }

        if (type == LDASTA)
            ldasta(line, l, cur, src);
        else if (type == LDASTA2)
            ldasta2(line, l, cur, src);
        else if (type == LDALDXLDA)
            ldaldxlda(line, l, cur, src);
        else if (type == INCDEC)
            incdec(line, l, cur, src);
        else if (type == BOP1)
            Bop1(line, l, cur, src);
        else if (type == BOP2)
            Bop2(line, l, cur, src);
        else {
            cout << "Phopt6502 " << endl;
            exit(1);
        }

        m_curLine = cur;
        if (m_curLine < m_src.size())
            src.push_back(m_src[m_curLine]);
        m_curLine++;
        if (m_curLine >= m_src.size()) {
            isDone = true;
            continue;
        }
    }
    m_src = src;
}
} // namespace tripe