#include "factorycompiler.h"

FactoryCompiler::FactoryCompiler()
{

}

Compiler *FactoryCompiler::CreateCompiler(CIniFile *ini, CIniFile *pIni)
{
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::MOS6502)
        return new Compiler6502(ini,pIni);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::PX86)
        return new CompilerX86(ini,pIni);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::M68000)
        return new CompilerM68K(ini,pIni);

    return nullptr;
}
