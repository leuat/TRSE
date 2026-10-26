#pragma once

#include <map>
#include <string>
#include "tripeutil.h"

using namespace std;
using namespace tripe;

class AbstractGuts {

public:

	static vector<uint8_t> m_memory;
	map<uint8_t, string> m_ins;
	map<uint8_t, uint8_t> m_size;
	map<int,string> m_symbols;


	virtual void Run(bool output) = 0;	

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

	void injectBin(string inFile, uint16_t adr) {
		auto data = tripe::Util::load_binary(inFile);
		uint16_t addr = (data[0]) | (data[1]<<8);
		int start = addr;
		if (inFile.ends_with(".prg")) {
			data.erase(data.begin(),data.begin()+2);
		}
		else {
			start = adr;
			addr = adr;

		}
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

    void Output(string addr) {
		int a = Util::fromNumber(addr);
		cout << addr << " : $"<< Util::toHex(m_memory[a])<<endl;
		exit(m_memory[a]);
    }


};