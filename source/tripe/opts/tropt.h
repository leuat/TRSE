#ifndef TROPT_H
#define TROPT_H

#include "cpu6502.h"
#include <string>
#include <vector>

namespace tripe {

class Tropt {
  private:
    CPU6502 m_cpu;
    vector<string> m_org, m_cur;
    string t = "\t";
    bool isTemp(string &s) { return s.starts_with("_r"); }

  public:
    std::vector<std::string> optimise(vector<string> input);

    std::vector<std::string> getLine(int i);

    void bops();
    void mov1();
    void load1();
    void load2();
    void muldiv();
    void cleanupAsm();
    void constIndex();

    int m_noLines = 0;
};
} // namespace tripe
#endif
