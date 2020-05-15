#include "factorycompiler.h"

FactoryCompiler::FactoryCompiler()
{

}

Compiler *FactoryCompiler::CreateCompiler(QSharedPointer<CIniFile> ini, QSharedPointer<CIniFile> pIni)
{
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::MOS6502)
        return new Compiler6502(ini,pIni);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::PX86)
        return new CompilerX86(ini,pIni);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::M68000)
        return new CompilerM68K(ini,pIni);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::Z80)
        return new CompilerZ80(ini,pIni);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::GBZ80)
        return new CompilerGBZ80(ini,pIni);

    return nullptr;
}
