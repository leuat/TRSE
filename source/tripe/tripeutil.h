#ifndef TRIPEUTIL_H
#define TRIPEUTIL_H

#include <algorithm>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

namespace tripe {

template <typename T> bool contains(vector<T> vec, const T &elem) {
    bool result = false;
    if (find(vec.begin(), vec.end(), elem) != vec.end()) {
        result = true;
    }
    return result;
}

class Util {
  public:
    static string trim(const std::string &s);
    static std::vector<std::string> &split(const std::string &s, char delim,
                                           std::vector<std::string> &elems);

    static std::vector<std::string> split(const std::string &s, char delim);

    static std::vector<std::string> clean_split(string s, char delim);

    static vector<string> read_text_code_file(string f, bool trim);
    static string load_text_file(string f);

    static string insertInFilename(string fn, string val);
    static string getFilenameAlone(string fn);

    static bool isPureNumber(string str) {
        if (str.starts_with("#"))
            str = str.erase(0, 1);
        if (str.starts_with("0x") || str.starts_with("$") ||
            str.starts_with("%"))
            return true;
        if (std::isdigit(str[0]))
            return true;
        return false;
    }

    static string toLower(string str) {
        transform(str.begin(), str.end(), str.begin(), ::tolower);
        return str;
    }

    static string toHex(uint64_t);
    static string toDec(uint64_t);

    static int fromNumber(string s);

    static void save_binary(string file, const vector<uint8_t> data);
    static void save_text(string file, const vector<string> data);
    static vector<uint8_t> load_binary(string file);
    static int getIntLen(string type);
    static vector<uint8_t> ival2int8(string ival, string type);
    static string ival2string(vector<uint8_t> &data, int pos, string type);

    static void append_string(string s, vector<uint8_t> &data);

    static string ReplaceString(std::string str, const std::string &from,
                                const std::string &to);
};

} // namespace tripe
#endif
