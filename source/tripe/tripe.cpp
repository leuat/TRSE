#include "tripe.h"
#include "error.h"
#include "tripeparser.h"
#include "tripeutil.h"
#include <filesystem>
#include <map>

using namespace tripe;
namespace tripe {

void Tripe::setInternal(bool b) { Data::s_isInternal = b; }

Tripe::Tripe(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        string arg1 = argv[i];
        if (arg1.find("-", 0) == 0) {
            arg1.erase(0, 1);
            auto val = "";
            if (contains(m_requireNextParam, arg1)) {
                if (i >= argc)
                    Error::RaiseError("-" + arg1 + " requires a parameter");
                val = argv[i + 1];
                i++;
            }
            m_args[Util::toLower(arg1)] = val;
        }
    }
}

void Tripe::RequireParameter(string p, string error) {
    if (!m_args.contains(p))
        Error::RaiseError(error);
}

void Tripe::Execute() {
    RequireParameter("i", "input file required (-i)");
    RequireParameter("o", "output file required (-o)");
    RequireParameter("arch", "architecture required (-arch)");
    string inFile = m_args["i"];
    string outFile = m_args["o"];
    string arch = Util::toLower(m_args["arch"]);
    string sys = m_args["sys"];
    Error::s_error = "";
    //    cout << inFile << endl;

    if (!std::filesystem::exists(inFile))
        Error::RaiseError("Could not find input file: " + inFile);

    if (!contains(m_supportedArchitectures, arch))
        Error::RaiseError("Architecture '" + arch + "' not supported. ");

    if (sys != "")
        if (!contains(m_supportedSystems, sys))
            Error::RaiseError("System '" + sys + "' not supported. ");

    TripeParser p;
    map<string, string> params;
    if (sys != "")
        params["sys"] = sys;

    try {

        if (m_args.contains("c")) {
            // Do all in a row
            auto optTripe = Util::insertInFilename(inFile, "_opt");
            auto binTripe = Util::getFilenameAlone(inFile) + ".trp";
            cout << optTripe << endl;
            Util::save_text(optTripe, p.TripeOptimise(inFile));
            Util::save_binary(binTripe, p.ParseText(optTripe));
            Util::save_text(outFile, p.ParseBinary(binTripe, arch, params));

        } else {
            if (arch == "trasm2tripe") {
                Util::save_binary(outFile, p.ParseText(inFile));
            } else if (arch == "tropt")
                Util::save_text(outFile, p.TripeOptimise(inFile));
            else
                Util::save_text(outFile, p.ParseBinary(inFile, arch, params));
        }
        m_optAsm = p.m_noAsmLinesOpt;
        m_optTripe = p.m_noTripeLinesOpt;
        if (!Data::s_isInternal)
            cout << "ok." << endl;
    } catch (string error) {
        if (!Data::s_isInternal) {
            cout << error << endl;
            exit(1);
        }
    }
}
} // namespace tripe