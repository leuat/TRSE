#ifndef PHOPT_P
#define PHOPT_P

#include "tripeutil.h"
#include <map>
#include <string>
#include <vector>

namespace tripe {

class Phopt {
  public:
    vector<string> m_src;
    virtual std::vector<std::string> optimize(std::vector<std::string> in) = 0;
    bool isEnd = false;
    int m_curLine = 0;
    string tab = "\t";
    static int s_optLines;
    map<string, string> m_symTab;
    /*
static string trim(const std::string &s);
static std::vector<std::string> &split(const std::string &s, char delim,
std::vector<std::string> &elems);
*/
    bool isTemp(string s) { return s.starts_with("_r"); }
    bool isTemp8(string s) { return s.starts_with("_r8"); }
    bool isTemp16(string s) { return s.starts_with("_r16"); }

    vector<string> getNextLine(string &l) {
        bool ok = false;
        string s = "";
        while (!ok) {
            if (m_curLine >= m_src.size()) {
                return vector<string>();
            }
            s = m_src[m_curLine];
            //			if (s=="") continue;

            l = s;
            m_curLine++;

            s = Util::trim(s);
            s = Util::ReplaceString(s, "\t",
                                    " "); // replace all 'x' to 'y'
                                          //  			s =
            //  Util::ReplaceString(s, "  ", " "); // replace all 'x' to 'y'

            //			s = Util::trim(s);
            if (s.starts_with(";") || s == "") {
                continue;
            }
            ok = true;
        }
        //	cout << s <<  "  : " <<(char)(s[0])<<endl;
        vector<string> lst;
        lst = Util::split(s, ';', lst);
        s = lst[0];
        lst.clear();
        return Util::split(s, ' ', lst);
    }

    vector<string> getLine(int l) {
        bool ok = false;
        string s = "";
        if (l >= m_src.size()) {
            return vector<string>();
        }
        s = m_src[l];
        if (s.starts_with(";") || s == "") {
            return vector<string>();
        }
        if (!s.starts_with("\t") && !s.starts_with(" "))
            return vector<string>{"is_label"};

        //			if (s=="") continue;

        s = Util::trim(s);
        s = Util::ReplaceString(s, "\t", " "); // replace all 'x' to 'y'
        s = Util::ReplaceString(s, "  ", " "); // replace all 'x' to 'y'
        //  			s = Util::ReplaceString(s, "  ",
        //  " "); // replace all 'x' to 'y'

        //			s = Util::trim(s);

        vector<string> lst;
        return Util::split(s, ' ', lst);
    }
};
} // namespace tripe
#endif
