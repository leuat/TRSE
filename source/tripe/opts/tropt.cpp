#include "opts/tropt.h"
#include <iostream>
#include <map>
using namespace std;

using namespace tripe;
namespace tripe {

void Tropt::bops() {

    /*
    Doesn't work:
    add     _r16_1  uint16:0x400    _r16_2
    mov     ptr     _r16_1
*/

    vector<string> bp = {"and", "add", "or", "xor", "sub", "shl", "shr", "divu", "mulu"};

    vector<string> n;
    // add 	t_uint8_2	j	uint8:0x01
    // mov j t_uint8_2

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto l1 = getLine(i + 1);
        auto cur = m_cur[i];
        if (find(bp.begin(), bp.end(), l0[0]) != bp.end()) {
            if (l0.size() == 4 && l1.size() == 3) {

                if (l1.size() >= 2 && l0.size() >= 1 && l1[0] == "mov") {
                    if (l0[1] == l1[2] && isTemp(l0[1])) {
                        // Perform replace
                        cur = t + l0[0] + t + l1[1] + t + l0[2] + t + l0[3];
                        /*
                        cout << m_cur[i] << endl;
                        cout << m_cur[i+1] << endl;
                        cout << "replace with : " << cur << endl << endl;
                        */
                        m_noLines++;
                        i += 1;
                    }
                }
            }
        }
        n.push_back(cur);
    }

    m_cur = n;
}

void Tropt::muldiv() {

    vector<string> n;
    // add  t_uint8_2   j   uint8:0x01
    // mov j t_uint8_2
    map<int, int> p2;
    int start = 2;
    for (int i = 0; i < 16; i++) {
        p2[start] = i + 1;
        start *= 2;
    }

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto cur = m_cur[i];
        l0[0] = Util::toLower(l0[0]);
        if (l0.size() == 4 && (l0[0] == "mulu" || l0[0] == "divu")) {
            vector<string> lst;
            lst = Util::split(l0[3], ':', lst);
            if (lst.size() == 2) {
                auto type = lst[0];
                int ival = Util::fromNumber(lst[1]);
                if (p2.contains(ival)) {

                    cur = t + ((l0[0] == "mulu") ? "shl" : "shr") + t + l0[1] + t + l0[2] + t + type + ":0x" + Util::toHex(p2[ival]);
                    //                    cout << m_cur[i] << " -> " << cur <<
                    //                    endl;
                }
            }
        }
        n.push_back(cur);
    }

    m_cur = n;
}

void Tropt::mov1() {

    //	vector<string> bp = {"and","add","or","xor","sub"};

    vector<string> n;
    // mov t_uint8_ld1 i
    // mov t_uint8_idx1 t_uint8_ld1

    //;load_p ptr t_uint8_idx1 t_uint8_ret1
    //;mov	 k t_uint8_ret1

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto l1 = getLine(i + 1);
        auto cur = m_cur[i];
        if (l0.size() == 3 && l1.size() == 3)
            if (l0[0] == "mov" && l1[0] == "mov") {
                if (l0[1] == l1[2] && isTemp(l0[1])) {
                    // Perform replace
                    cur = t + l0[0] + t + l1[1] + t + l0[2];
                    //					cout
                    //<< "replace with : " << cur << endl <<endl;
                    i += 1;
                    m_noLines++;
                }
            }
        n.push_back(cur);
    }

    m_cur = n;
}

void Tropt::load1() {

    vector<string> n;
    //	load_p t t_uint8_idx1 t_uint8_ret1
    //	mov	 k t_uint8_ret1

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto l1 = getLine(i + 1);
        auto cur = m_cur[i];
        if (l0.size() == 4 && l1.size() == 3)
            if ((l0[0] == "load") && l1[0] == "mov") {
                if (l0[3] == l1[2] && isTemp(l0[3])) {
                    // Perform replace
                    cur = t + l0[0] + t + l0[1] + t + l0[2] + t + l1[1];
                    //					cout
                    //<< "replace with : " << cur << endl <<endl;
                    m_noLines++;
                    i += 1;
                }
            }
        n.push_back(cur);
    }

    m_cur = n;
}

// Typical index loading
void Tropt::load2() {

    vector<string> n;
    //	mov	t_uint8_idx2	uint8:0x00
    //	load_p Screen_p1 t_uint8_idx2 t_uint8_ret1

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto l1 = getLine(i + 1);
        auto cur = m_cur[i];
        if (l0.size() == 3 && l1.size() == 4)
            if ((l1[0] == "store" || l1[0] == "load") && l0[0] == "mov") {
                if (l0[1] == l1[2] && isTemp(l0[1])) {

                    // Perform replace
                    cout << "YE" << endl;
                    cur = t + l1[0] + t + l1[1] + t + l0[2] + t + l1[3];
                    //					cout
                    //<< "replace with : " << cur << endl <<endl;
                    m_noLines++;
                    i += 1;
                }
            }
        n.push_back(cur);
    }

    m_cur = n;
    n.clear();
    //    mov     _r8_1   uint8:0x14
    //   store   barr    i   _r8_1

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto l1 = getLine(i + 1);
        auto cur = m_cur[i];
        if (l0.size() == 3 && l1.size() == 4)
            if ((l1[0] == "store") && l0[0] == "mov") {
                if (l0[1] == l1[3] && isTemp(l0[1])) {

                    // Perform replace
                    // cout << "YE" << endl;
                    cur = t + l1[0] + t + l1[1] + t + l1[2] + t + l0[2] + " ; opt15";
                    //                  cout
                    //<< "replace with : " << cur << endl <<endl;
                    m_noLines++;
                    i += 1;
                }
            }
        n.push_back(cur);
    }

    m_cur = n;
}

// Typical index loading
void Tropt::cleanupAsm() {

    vector<string> n;

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto l1 = getLine(i + 1);
        auto cur = m_cur[i];
        if (l0.size() == 1 && l1.size() == 1) {

            if ((l0[0] == ".asm" && l1[0] == ".endasm")) {
                m_noLines += 2;
                i += 1;
                continue;
            }
        }

        n.push_back(cur);
    }

    m_cur = n;
}

void Tropt::constIndex() {
    /*
        mov     _r8_1   uint8:0x1a
        add     _r8_2   i   uint8:0x12
        store   p1  _r8_2   _r8_1
    */

    vector<string> n;

    for (int i = 0; i < m_cur.size(); i++) {
        auto l0 = getLine(i);
        auto l1 = getLine(i + 1);
        auto l2 = getLine(i + 2);
        auto cur = m_cur[i];
        //        cout << i << " : " << m_cur.size() << " " << cur << "  " <<
        //        l1.size()
        //           << " " << l2.size() << endl;
        if (l0.size() == 3 && l1.size() == 4 && l2.size() == 4) {
            if ((l0[0] == "mov" && l2[0] == "store") && l0[1] == l2[3]) {

                m_noLines += 1;
                i += 2;
                //                n.push_back(t + "; constopt1");
                n.push_back(t + l1[0] + t + l1[1] + t + l1[2] + t + l1[3]);
                n.push_back(t + l2[0] + t + l2[1] + t + l2[2] + t + l0[2]);
                //                cout << "QHERE" << endl;

                continue;
            }
        }

        n.push_back(cur);
    }
    m_cur = n;
}

vector<string> Tropt::optimise(vector<string> input) {
    m_cpu.Init("");
    m_org = input;
    m_cur = m_org;

    bops();
    mov1();
    load1();
    load2();
    muldiv();
    cleanupAsm();
    constIndex();
    return m_cur;
}

vector<string> Tropt::getLine(int i) {
    vector<string> ret, ret2;
    if (i >= m_cur.size())
        return ret;
    auto s = Util::trim(m_cur[i]);
    s = Util::ReplaceString(s, "\t", " "); // replace all 'x' to 'y'
    s = Util::ReplaceString(s, "  ", " "); // replace all 'x' to 'y'

    //    s = Util::split(s, ';', ret)[0];
    ret = Util::split(s, ' ', ret);
    // Lowercase operation
    if (ret.size() > 0)
        ret[0] = Util::toLower(ret[0]);

    for (auto c : ret)
        if (Util::trim(c) != "")
            ret2.push_back(c);

    return ret2;
}
} // namespace tripe