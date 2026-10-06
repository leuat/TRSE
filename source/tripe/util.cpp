#include "util.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace tripe;
namespace tripe {

string Util::trim(const std::string &s) {
    auto wsfront = std::find_if_not(s.begin(), s.end(),
                                    [](int c) { return std::isspace(c); });
    auto wsback = std::find_if_not(s.rbegin(), s.rend(), [](int c) {
                      return std::isspace(c);
                  }).base();
    return (wsback <= wsfront ? std::string() : std::string(wsfront, wsback));
}

std::vector<std::string> &Util::split(const std::string &s, char delim,
                                      std::vector<std::string> &elems) {
    std::stringstream ss(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        elems.push_back(item);
    }
    return elems;
}

vector<string> Util::read_text_code_file(string f, bool trim) {
    vector<string> m_src;
    ifstream inp(f);
    string s, o;
    while (getline(inp, o)) {
        s = Util::trim(o);
        if (trim)
            o = s;
        if (s != "" && (s.find(";", 0) != 0) && (s.find("#", 0) != 0))
            m_src.push_back(o);
    }

    return m_src;
}

string Util::load_text_file(string f) {
    vector<string> m_src;
    ifstream inp(f);
    string s;
    string ret;
    while (getline(inp, s)) {
        ret += s + "\n";
    }

    return ret;
}
int Util::getIntLen(string type) {
    int len = 1;
    if (type == "uint64")
        len = 8;
    if (type == "uint32")
        len = 4;
    if (type == "uint16")
        len = 2;
    if (type == "address")
        len = 2;
    if (type == "uint8")
        len = 1;
    return len;
}

void Util::save_binary(string file, vector<uint8_t> data) {
    std::ofstream outfile(file, std::ofstream::binary);
    outfile.write(reinterpret_cast<const char *>(data.data()),
                  sizeof(uint8_t) * data.size());
    outfile.close();
}

void Util::save_text(string file, vector<string> data) {
    std::ofstream outfile(file);
    for (auto &s : data) {
        outfile << s << endl;
    }
    outfile.close();
}

vector<uint8_t> Util::ival2int8(string ival, string type) {

    int len = getIntLen(type);
    stringstream st(ival);
    uint64_t val;
    if (ival.find("0x", 0) == 0)
        st >> hex >> val;
    else
        st >> val;

    vector<uint8_t> data;
    if (len == 1)
        data.push_back(val & 0xFF);
    if (len == 2) {
        data.push_back((val >> 8) & 0xFF);
        data.push_back(val & 0xFF);
    }
    if (len == 4) {
        data.push_back((val >> 24) & 0xFF);
        data.push_back((val >> 16) & 0xFF);
        data.push_back((val >> 8) & 0xFF);
        data.push_back(val & 0xFF);
    }
    if (len == 8) {
        data.push_back((val >> 56) & 0xFF);
        data.push_back((val >> 48) & 0xFF);
        data.push_back((val >> 40) & 0xFF);
        data.push_back((val >> 32) & 0xFF);
        data.push_back((val >> 24) & 0xFF);
        data.push_back((val >> 16) & 0xFF);
        data.push_back((val >> 8) & 0xFF);
        data.push_back(val & 0xFF);
    }
    return data;
}

string Util::ival2string(vector<uint8_t> &data, int pos, string type) {
    int len = getIntLen(type);
    uint64_t val = 0;
    if (len == 1)
        val += data[pos];
    if (len == 2) {
        val += data[pos] << 8;
        val += data[pos + 1];
    }
    if (len == 4) {
        val += (data[pos]) << 24;
        val += (data[pos + 1]) << 16;
        val += (data[pos + 2]) << 8;
        val += data[pos + 3];
    }
    if (len == 8) {
        val += (uint64_t)(data[pos]) << 56;
        val += (uint64_t)(data[pos + 1]) << 48;
        val += (uint64_t)(data[pos + 2]) << 40;
        val += (uint64_t)(data[pos + 3]) << 32;
        val += data[pos + 4] << 24;
        val += data[pos + 5] << 16;
        val += data[pos + 6] << 8;
        val += data[pos + 7];
    }
    return toHex(val);
}

string Util::toHex(uint64_t val) {
    std::stringstream stream;
    stream << std::hex << val;
    return stream.str();
}

string Util::toDec(uint64_t val) {
    std::stringstream stream;
    stream << std::dec << val;
    return stream.str();
}

void Util::append_string(string s, vector<uint8_t> &data) {
    int n = s.length();
    char char_array[n + 1];

    // copying the contents of the
    // string to char array
    strcpy(char_array, s.c_str());
    for (int i = 0; i < n; i++)
        data.push_back(char_array[i]);

    data.push_back(0);
}

vector<uint8_t> Util::load_binary(string file) {
    std::ifstream fint(file, ios::ate | ios::binary);
    uint size = fint.tellg();
    fint.close();
    std::ifstream fin(file, ios::in | ios::binary);
    char *buffer = new char[size];
    fin.read(buffer, size);
    vector<uint8_t> data;
    for (int i = 0; i < size; i++)
        data.push_back(buffer[i]);
    fin.close();
    delete[] buffer;
    return data;
}

string Util::ReplaceString(std::string str, const std::string &from,
                           const std::string &to) {
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos +=
            to.length(); // Handles case where 'to' is a substring of 'from'
    }
    return str;
}
// std::vector<std::string> &Util::split(const std::string &s, char delim,
// std::vector<std::string> &elems) {

string Util::insertInFilename(string fn, string val) {
    vector<string> lst;
    lst = Util::split(fn, '.', lst);
    return lst[0] + val + "." + lst[1];
}

string Util::getFilenameAlone(string fn) {
    vector<string> lst;
    lst = Util::split(fn, '.', lst);
    return lst[0];
}

int Util::fromNumber(string s) {
    stringstream st(s);
    uint64_t val;
    if (s.find("0x", 0) == 0)
        st >> hex >> val;
    else
        st >> val;
    return val;
}
} // namespace tripe