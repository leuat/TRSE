#ifndef TRIPEPARSER_H
#define TRIPEPARSER_H

#include "abstractcpu.h"
#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <vector>

using namespace std;

namespace tripe {

class TripeParser {
  public:
    const char *m_id = "TRP";
    vector<uint8_t> ParseText(string inFile);
    vector<string> TripeOptimise(string inFile);
    vector<string> ParseBinary(string inFile, string arch,
                               map<string, string> params);

  private:
    void AppendExtraCode(AbstractCPU *cpu);

    void ParseTextToBinary();
    void ParseBinary(AbstractCPU *op, int pass);

    void LoadBinary(string inFile);

    vector<string> m_src, m_src_org;
    vector<uint8_t> m_data;
};
} // namespace tripe
#endif
