#include "tripe.h"
#include <iostream>

using namespace std;

void print_usage() {
    cout << "Usage: " << endl;
    cout << " tripe -arch [ system ] -i [ in.trp ] -o [ out.asm ]  -params... "
         << endl;
    cout << endl;
    cout
        << " -arch mos6502 : compiles a tripe .trp binary file to a 6502 binary"
        << endl;
    cout << " -arch trasm2tripe : compiles a .trasm text assembly file to a "
            ".tripe binary file"
         << endl;
    cout << " -arch tropt : optimises a .trasm text assembly file" << endl;
    cout << endl;
    cout << "optional parameters:" << endl;
    cout << " -sys [c64, vic20] : adds loading stub for the current system"
         << endl;
    cout << " -c : performs a full compile and optimization for the current "
            "system"
         << endl;
    cout << " -start_address $1000 : custom start address for the system"
         << endl;
}

int main(int argc, char *argv[]) {

    if (argc == 1) {
        print_usage();
        return 0;
    }
    Tripe t(argc, argv);
    t.Execute();
    return 0;
}