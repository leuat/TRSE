#include "error.h"
#include "tripedata.h"
using namespace tripe;
namespace tripe {

int Error::s_lineNumber = 0;
string Error::s_curLine = "";
string Error::s_error = "";

void Error::RaiseError(string s) {
    s_error = "";
    s_error += "Tripe fatal error on line " + std::to_string(s_lineNumber) + "\n";
    s_error += "Error message: \n";
    s_error += s + "\n";
    throw(s_error);
}
} // namespace tripe