#ifndef TRIPE_H
#define TRIPE_H

#include "tripedata.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace tripe {

class Tripe {
  public:
    vector<string> m_requireNextParam = {"i", "o", "arch", "sys"};
    vector<string> m_supportedArchitectures = {"mos6502", "tripe2trasm",
                                               "trasm2tripe", "amd64", "tropt"};
    vector<string> m_supportedSystems = {"c64", "vic20"};

    Tripe(int argc, char *argv[]);
    void setInternal(bool b);
    void Execute();
    int m_optAsm = 0;
    int m_optTripe = 0;

  private:
    map<string, string> m_args;
    void RequireParameter(string p, string error);
};

} // namespace tripe
#endif