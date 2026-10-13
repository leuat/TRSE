#include "tripe.h"
#include "error.h"
#include "lorgasm.h"
#include "tripeparser.h"
#include "tripeutil.h"
#include <chrono>
#include <filesystem>
#include "guts/guts6502.h"
#include <map>

using namespace tripe;
namespace tripe {

void Tripe::setInternal(bool b) { Data::d.isInternal = b; }

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
    try {
    RequireParameter("i", "input file required (-i)");
    string inFile = m_args["i"];

    if (m_args.contains("guts")) {
        Guts6502 guts;
        guts.injectBin(inFile);
        guts.Run(m_args.contains("guts_output"));
        if (m_args.contains("exit_mem"))
            guts.Output(m_args["exit_mem"]);
        exit(0);
    }

    RequireParameter("o", "output file required (-o)");
    RequireParameter("arch", "architecture required (-arch)");
    auto start = chrono::system_clock::now();
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

        if (m_args.contains("ptr_zp"))
            Data::d.ptrZp = Util::fromNumber(m_args["ptr_zp"]);
        if (m_args.contains("reg_zp"))
            Data::d.regZp = Util::fromNumber(m_args["reg_zp"]);
        if (m_args.contains("wh_zp"))
            Data::d.whZp = Util::fromNumber(m_args["wh_zp"]);

        string binEnding = "bin";
        if (sys == "c64" || sys == "vic20")
            binEnding = "prg";

        if (m_args.contains("c")) {
            // Do all in a row
            auto optTripe = Util::insertInFilename(inFile, "_opt");
            auto binTripe = Util::getFilenameAlone(inFile) + ".trp";
            auto assembledFile = Util::getFilenameAlone(outFile) + "." + binEnding;
            Util::save_text(optTripe, p.TripeOptimise(inFile));
            Util::save_binary(binTripe, p.ParseText(optTripe));
            Util::save_text(outFile, p.ParseBinary(binTripe, arch, params));
            if (m_args.contains("a")) {
                OrgAsm orgasm(arch);
                orgasm.Assemble(outFile, assembledFile);
                if (m_args.contains("sym"))
                    Util::save_text(Util::getFilenameAlone(outFile)+".sym", orgasm.getSymbols());

            }

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
        if (!Data::d.isInternal)
            cout << "ok." << endl;
    } catch (string error) {
        Error::s_error += error;
        if (!Data::d.isInternal) {
            cout << error << endl;
            exit(1);
        }
    }
    auto stop = std::chrono::high_resolution_clock::now();

    m_timeMs = 0; // Crashes on windowsstd::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
}
} // namespace tripe