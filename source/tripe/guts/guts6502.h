#pragma once


#include "guts/gcpu6502.h"
#include "tripeutil.h"
#include <string>
#include <vector>
#include <map>
using namespace std;
using namespace tripe;


class Guts6502 {
public:
	Guts6502() {
		LoadDefs();
    	m_memory.resize(0xFFFF);
		for (int i=0;i<m_memory.size();i++)
			m_memory[i] = 0;

		m_cpu = new mos6502(BusRead, BusWrite);
    	m_cpu->Reset();
    	m_cpu->pc = 0x810;		
	}


	static vector<uint8_t> m_memory;
	map<uint8_t, string> m_ins;
	map<uint8_t, uint8_t> m_size;
	map<int,string> m_symbols;

	void LoadDefs();

	void LoadSymbols(string inFile) {
		string s = Util::load_text_file(inFile);
		auto lst = Util::split(s,'\n');
		for (auto l :lst) {
			l = Util::trim(l);
			l = Util::ReplaceString(l,"  "," ");
			auto sp = Util::split(l,' ');
			if (sp[0]!="al")
				continue;
			m_symbols[Util::fromNumber(Util::trim(sp[1]))] = Util::trim(sp[2]);
		}		
	}

	void injectBin(string inFile) {
		auto data = tripe::Util::load_binary(inFile);
		uint16_t addr = (data[0]) | (data[1]<<8);
		data.erase(data.begin(),data.begin()+2);
		int start = addr;
		for (auto& d: data) 
			m_memory[addr++] = d;
		cout << "Loaded "<<data.size()<<" bytes to address "<< Util::toHex(start)<<endl;

		string symf = Util::split(inFile,'.')[0];
		LoadSymbols(symf+".sym");
	}

	string ins(uint16_t addr) {
		uint16_t op = m_memory[addr];
		string s = m_ins[op];
		int val = -1;
		if (m_size[op]==2)
			val = m_memory[addr+1];
		if (m_size[op]==3)
			val = m_memory[addr+1] | (m_memory[addr+2]<<8);

//		s += "  size:"+std::to_string(m_size[op]);
		if (m_symbols.contains(val))
			s+=" "+m_symbols[val];
		else
			if (val!=-1)
				s+=" $"+Util::toHex(val);
		return s;
	}

	void Run() {
		cout << "Gutsing.... " << endl;
		m_cpu->pc = 0x810;
		while (m_memory[m_cpu->pc]!=00) {
			m_cpu->Run(1);
			cout << "pc:$" <<Util::toHex(m_cpu->pc) <<  "  : "<< ins(m_cpu->pc) <<endl;
		}
	}

	mos6502* m_cpu = nullptr;

   static void BusWrite(uint16_t a, uint8_t v) {
   		m_memory[a] = v;
    }
    static uint8_t BusRead(uint16_t a) {
    	return m_memory[a];
    }

};
