#pragma once

#include "guts/abstractguts.h"
#include "guts/gcpu6502.h"
#include "tripeutil.h"
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace tripe;


class Guts6502 : public AbstractGuts {
public:

	mos6502* m_cpu = nullptr;

	Guts6502() {
		LoadDefs();
    	m_memory.resize(0x10000);
		for (int i=0;i<m_memory.size();i++)
			m_memory[i] = 0;

		m_cpu = new mos6502(BusRead, BusWrite);
    	m_cpu->Reset();
    	m_cpu->pc = 0x810;		
	}



	void LoadDefs();

	void Run(bool output) override {
		cout << "Gutsing.... " << endl;
		m_cpu->pc = 0x810;
		while (m_memory[m_cpu->pc]!=00) {
			m_cpu->Run(1);
			if (output) 
				cout << "pc:$" <<Util::toHex(m_cpu->pc) <<  "  : "<< ins(m_cpu->pc) <<endl;
		}
	}


   static void BusWrite(uint16_t a, uint8_t v) {
   		m_memory[a] = v;
    }
    static uint8_t BusRead(uint16_t a) {
    	return m_memory[a];
    }


};
