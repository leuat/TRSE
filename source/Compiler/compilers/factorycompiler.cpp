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
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::ARM)
        return new CompilerArm(ini,pIni);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C816)
        return new Compiler65C816(ini,pIni,Syntax::s.m_currentSystem->m_processor);
    if (Syntax::s.m_currentSystem->m_processor == AbstractSystem::WDC65C02)
        return new Compiler65C816(ini,pIni,Syntax::s.m_currentSystem->m_processor);

    return nullptr;
}
