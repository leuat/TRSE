#pragma once


#include "guts/abstractguts.h"
#include "guts/z80.hpp"
#include "tripeutil.h"
#include <string>
#include <vector>
#include <map>

using namespace std;
using namespace tripe;


class GutsZ80 : public AbstractGuts {
public:
	GutsZ80() {
//		LoadDefs();
    	m_memory.resize(0x10000);
		for (int i=0;i<m_memory.size();i++)
			m_memory[i] = 0;

		m_cpu = new Z80(readByte, writeByte, inPort, outPort, this);

		// overload amstrad cpc write calls
		uint8_t ret = 0xc9;
		m_memory[0xbb5a] = ret;
		m_memory[0xbb8] = ret;
		m_memory[0xbd6] = ret;
		m_memory[0xbd2b] = 0x37; // set carry
		m_memory[0xbd2b+1] = ret;

//    	m_cpu->Reset();
  //  	m_cpu->pc = 0x810;	*/	
	}

	Z80* m_cpu = nullptr;

	static unsigned char readByte(void* arg, unsigned short addr)
	{
	    // NOTE: implement switching procedure here if your MMU has bank switch feature
	    return m_memory[addr];
	}

	// memory write request per 1 byte from CPU
	static void writeByte(void* arg, unsigned short addr, unsigned char value)
	{
	    // NOTE: implement switching procedure here if your MMU has bank switch feature
	    m_memory[addr] = value;
	}

	// IN operand request from CPU
	static unsigned char inPort(void* arg, unsigned short port)
	{
	    return 0;//((MMU*)arg)->IO[port];
	}

	// OUT operand request from CPU
	static void outPort(void* arg, unsigned short port, unsigned char value)
	{
//	    ((MMU*)arg)->IO[port] = value;
	}

	void LoadDefs();

	void Run(bool output) override {
		cout << "Gutsing.... " << endl;
		//cp l
		m_cpu->reg.PC = 0x4000;
		while (m_memory[m_cpu->reg.PC]!=0xbd) {
			m_cpu->execute(1);
			if (output) 
				cout << "pc:$" <<Util::toHex(m_cpu->reg.PC) <<  "  : "<< Util::toHex(m_memory[m_cpu->reg.PC]) <<endl;
		}
		
	}

/*
   static void BusWrite(uint16_t a, uint8_t v) {
   		m_memory[a] = v;
    }
    static uint8_t BusRead(uint16_t a) {
    	return m_memory[a];
    }
*/

};
