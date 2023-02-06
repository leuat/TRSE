/*
 * Turbo Rascal Syntax error, “;” expected but “BEGIN” (TRSE, Turbo Rascal SE)
 * 8 bit software development IDE for the Commodore 64
 * Copyright (C) 2018  Nicolaas Ervik Groeneboom (nicolaas.groeneboom@gmail.com)
 *
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program (LICENSE.txt).
 *   If not, see <https://www.gnu.org/licenses/>.
*/

#include "symboltable.h"
#include <qmath.h>

bool SymbolTable::isInitialized = false;
int SymbolTable::m_currentSid = 0;
QString Symbol::s_currentProcedure = "";
QMap<QString,int> SymbolTable::s_classSizes;
int SymbolTable::pass = 0;
QStringList SymbolTable::s_ignoreUnusedSymbolWarning;


//QString SymbolTable::m_gPrefix = "";
//QMap<QString,QSharedPointer<Symbol>> SymbolTable::m_constants;

SymbolTable::~SymbolTable() {
    /*    for (QString s: m_symbols.keys())
        delete m_symbols[s];
    m_symbols.clear();

    for (QString s: m_constants.keys())
        delete m_constants[s];
    m_constants.clear();

    for (QString s: m_records.keys())
        delete m_records[s];
    m_records.clear();
*/
    Delete();
}

void SymbolTable::ExitProcedureScope(bool removeSymbols) {
    // "TRUE" doesn't work
    if (removeSymbols)
        for (QString s: m_symbols.keys()) {
            if (s.startsWith(m_currentProcedure)) {
                m_symbols.remove(s);
            }
        }


    m_currentProcedure="";
}

SymbolTable::SymbolTable()
{
    m_currentSid = 0;
    InitBuiltins();
    Initialize();

}

void SymbolTable::DefineSid(unsigned int initAddress, unsigned int playAddress) {
    m_currentSid++;
    QString s = QString::number(m_currentSid);

    QString init = "SIDFILE_"+s+"_INIT";
    QString play = "SIDFILE_"+s+"_PLAY";
    m_constants[init] = QSharedPointer<Symbol>(new Symbol("$"+QString::number(initAddress,16),"ADDRESS", initAddress));
    m_constants[play] = QSharedPointer<Symbol>(new Symbol("$"+QString::number(playAddress,16),"ADDRESS", playAddress));
    //    qDebug() << "DEFINE SID "<<playAddress;

}

bool SymbolTable::ContainsArrays()
{
    for (QString ss: m_symbols.keys()) {
        QSharedPointer<Symbol> s  = m_symbols[ss];
        if (s->m_type=="ARRAY")
            return true;
    }
    return false;
}

void SymbolTable::Initialize()
{
    //  if (isInitialized)
    //        return;

    //  m_constants.clear();

    QVector<QSharedPointer<Symbol>> keeps;
    if (
            (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMSTRADCPC || Syntax::s.m_currentSystem->m_system==AbstractSystem::SPECTRUM)
            && m_constants.contains("INIT_MUSIC")) {
        // Addresses change due to compiler
        keeps.append(m_constants["INIT_MUSIC"]);
        keeps.append(m_constants["PLAY_MUSIC"]);
        keeps.append(m_constants["STOP_MUSIC"]);
    }

    m_tempPointers.m_vars.clear();
    for (int i=0;i<100;i++)
        m_tempPointers.push("tempPointer"+QString::number(i));

    QString currentSystem = AbstractSystem::StringFromSystem(Syntax::s.m_currentSystem->m_system).toLower();
    //    qDebug() << currentSystem;
    for (QString s: Syntax::s.m_syntaxData.split('\n')) {
        s= s.simplified();
        if (s.length()==0) continue;
        if (s.startsWith("#")) continue;
        s=s.replace(" ", "");

        QStringList data = s.split(";");
        if (data[0].toLower()!="c")
            continue;
        if (data.count()!=5) {
            qDebug() << "SymbolTable::Initialize reporting ERROR in syntax.txt";
            qDebug() << "Line: " << s;
            exit(1);
        }
        QString constant = data[1].toUpper();
        QString system = data[2].toLower();
        QString type = data[3].toLower();
        QStringList values = data[4].replace(" ","").split(",");
        QString value;
        if (Syntax::s.m_currentSystem->m_memoryType<values.count())
            value = values[Syntax::s.m_currentSystem->m_memoryType];
        else
            value = values[0];


        //    if (constant=="KEY_2")
        //      qDebug() << system << currentSystem << system.contains(currentSystem);

        if (Syntax::s.m_currentSystem->systemIsOfType(system.split(","))) {

            ulong ival = Util::NumberFromStringHex(value);
            if (ival==0 && value.length()>4) {
                //                qDebug() << "IVAL zero so : " << value;
                QString tst = value;
                tst = tst.remove("$");
                bool ok;
                ival = tst.toLongLong(&ok,16);

                //                if (constant.contains("PAL"))
                //                  qDebug() << "SymbolTable constants "<< constant << ival << value<<tst;


                if (!ok)
                    ErrorHandler::e.Error("Error reading constant '"+constant+"' with value '"+value+"' in syntax.txt. This should not happen, contant leuat@www.irio.co.uk at once!");
                //              qDebug() << "OK? " <<ok <<tst;
            }
            //            qDebug() << "TRYING : " << value << ival;
            if (type=="b")
                m_constants[constant] = QSharedPointer<Symbol>(new Symbol(value,"BYTE", ival));
            if (type=="a") {
                m_constants[constant] = QSharedPointer<Symbol>(new Symbol("^"+value,"ADDRESS", ival));
            }
            if (type=="i")
                m_constants[constant] = QSharedPointer<Symbol>(new Symbol(value,"INTEGER", ival));
            if (type=="l")
                m_constants[constant] = QSharedPointer<Symbol>(new Symbol(value,"LONG", ival));

            //            if (constant.toLower().contains("palette"))
            //            qDebug() << "SYMTAB "<<constant<<value<<ival <<m_constants[constant]->m_value->m_fVal;

            //            if (constant=="KEY_2")
            //              qDebug() << constant << Util::numToHex(value.toInt()));

            //            reservedWords.append(Token(TokenType::getType(word), word.toUpper())));
        }

    }
    //    if (Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES)
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 || Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65 ||  Syntax::s.m_currentSystem->m_system==AbstractSystem::C128)
        if (!m_constants.contains("SIDFILE_1_INIT")) {
            for (int i=1;i<10;i++) {
                if (!m_constants.contains("SIDFILE_"+QString::number(i)+"_INIT")) {
                    m_constants["SIDFILE_"+QString::number(i)+"_INIT"] = QSharedPointer<Symbol>(new Symbol("","ADDRESS", 0));
                    m_constants["SIDFILE_"+QString::number(i)+"_PLAY"] = QSharedPointer<Symbol>(new Symbol("","ADDRESS", 0));
                    //               qDebug() << "Redefining : "<<i;

                }
            }

        }

    if (Syntax::s.m_currentSystem->m_system == AbstractSystem::C64 )
        for (unsigned char key: Syntax::s.m_c64keys.keys()) {
            C64Key k = Syntax::s.m_c64keys[key];
            m_constants[k.m_key] = QSharedPointer<Symbol>(new Symbol(QString::number(k.m_value), "BYTE",  k.m_value));
        }
    //    qDebug()  << Util::numToHex(m_constants["KEY_2"]->m_value->m_fVal);


    if (
            (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMSTRADCPC ||
             Syntax::s.m_currentSystem->m_system==AbstractSystem::SPECTRUM)
            && keeps.count()==3) {
        m_constants["INIT_MUSIC"] = keeps[0];
        m_constants["PLAY_MUSIC"] = keeps[1];
        m_constants["STOP_MUSIC"] = keeps[2];
        //        qDebug() << Util::numToHex(keeps[0]->m_value->m_fVal);
    }



}

void SymbolTable::Merge(SymbolTable *other, bool mergeConstants)
{
    for (QString k : other->m_records.keys()) {
        //        if (m_records.contains(k) && m_externalRecords.contains(k))
        //          ErrorHandler::e.Error("Record already defined : '"+k+"'");
        m_records[k] = other->m_records[k];
    }
    for (QString k : other->m_symbols.keys()) {
        if (!m_symbols.contains(k)) {
            m_symbols[k] = other->m_symbols[k];
        }
    }
    //    qDebug() << other->m_constants.keys();
    m_globalList.append(other->m_globalList);
    m_globalList.removeDuplicates();
    if (mergeConstants)
        for (QString k : other->m_constants.keys()) {

            if (!m_constants.contains(k)) {
                QString newName = k;
                if (!other->m_constants[k]->m_constIsPrefixed) {
                    newName = other->m_gPrefix.toUpper()+k;
                    other->m_constants[k]->m_constIsPrefixed = true;
                }
                m_constants[newName] = other->m_constants[k];
            }


        }
    /*
    for (QString t : m_symbols.keys())
        qDebug() << m_symbols[t]->isUsedBy;
*/
}

void SymbolTable::Define(QSharedPointer<Symbol> s, bool isUsed) {

    //    if (isRegisterName(s->m_name))
    //       return ;
    QString name = m_currentProcedure+ s->m_name;
    if (m_ignoreAllprefixes)
        name = s->m_name;

    if (isRegisterName(s->m_name))
        name = s->m_name;

    //        qDebug() << "Registering : "<<s->m_name;
    m_symbols[name] = s;
    m_symbols[name]->isUsed = isUsed;

    s->m_fileName = m_currentFilename;
    if (m_addToGlobals)
        m_globalList.append(name);
}

void SymbolTable::Delete() {
    /*    for (QString val : m_symbols.keys()) {
        QSharedPointer<Symbol> s = m_symbols[val];
        if (s!=nullptr) {
            if (s->m_value)
                delete s->m_value;

        }
        delete s;
    }*/
    m_symbols.clear();
    // Delete static constants as well
    /*    if (isInitialized) {
        for (QString val : m_constants.keys()) {
            QSharedPointer<Symbol> s = m_symbols[val];

            if (s!=nullptr) {
                if (s->m_value)
                    delete s->m_value;

            }
            delete s;
        }
        isInitialized = false;

    }
    m_constants.clear();
    */
    //    for (QString s: m_records.keys())
    //        delete m_records[s];
    m_records.clear();

}

void SymbolTable::setName(QString s) {
    m_name = s;
}

void SymbolTable::InitBuiltins()
{

    /*    m_globalList << "screenmemory" << "sine" << "return";
     << "log2_table"
                 << "joystickup" << "joystickdown" << "joystickleft" << "joystickright" <<"joystickbutton"
                 << "joy1" << "joy1pressed" <<
*/
    m_addToGlobals = true;
    // Define global methods here
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("INTEGER","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("WORD","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("BOOLEAN","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("LONG","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("REAL","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("BYTE","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("STRING","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("CSTRING","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("POINTER","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("ARRAY","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("INCBIN","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("INCSID","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("INCNSF","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("RECORD","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("CLASS","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("GLOBAL","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("PURE","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("PURE_VARIABLE","")));
    Define(QSharedPointer<Symbol>(new BuiltInTypeSymbol("PURE_NUMBER","")));

    Define(QSharedPointer<Symbol>(new Symbol("return","")));

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::X86) {
        Define(QSharedPointer<Symbol>(new Symbol("currentKey", "integer")));
        m_globalList << "currentKey";
    }

    if (Syntax::s.m_currentSystem->m_system!=AbstractSystem::NES &&
            Syntax::s.m_currentSystem->m_system!=AbstractSystem::X86  &&
            Syntax::s.m_currentSystem->m_system!=AbstractSystem::GAMEBOY)
        Define(QSharedPointer<Symbol>(new Symbol("sine", "address")));
    Define(QSharedPointer<Symbol>(new Symbol("log2_table", "address")));
    Define(QSharedPointer<Symbol>(new Symbol("joystickup", "byte")));
    Define(QSharedPointer<Symbol>(new Symbol("joystickdown", "byte")));
    Define(QSharedPointer<Symbol>(new Symbol("joystickleft", "byte")));
    Define(QSharedPointer<Symbol>(new Symbol("joystickright", "byte")));
    Define(QSharedPointer<Symbol>(new Symbol("joystickbutton", "byte")));

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20) {

        Define(QSharedPointer<Symbol>(new Symbol("joy1", "byte")));
        Define(QSharedPointer<Symbol>(new Symbol("joy1pressed", "byte")));
    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64) {

        Define(QSharedPointer<Symbol>(new Symbol("joy2", "byte")));
        Define(QSharedPointer<Symbol>(new Symbol("joy2pressed", "byte")));
    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::GAMEBOY) {
        Define(QSharedPointer<Symbol>(new Symbol("cmpvar", "address")));

    }

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::NES) {

        Define(QSharedPointer<Symbol>(new Symbol("buttons", "address")));
    }

    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::C64 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::PLUS4 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::C128 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::VIC20 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::PET ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::MEGA65 ||
            Syntax::s.m_currentSystem->m_system==AbstractSystem::NES
            ) {

        Define(QSharedPointer<Symbol>(new Symbol("screenmemory", "pointer")));
        Define(QSharedPointer<Symbol>(new Symbol("colormemory", "pointer")));
        Define(QSharedPointer<Symbol>(new Symbol("_a", "byte")));
        Define(QSharedPointer<Symbol>(new Symbol("_x", "byte")));
        Define(QSharedPointer<Symbol>(new Symbol("_y", "byte")));
        Define(QSharedPointer<Symbol>(new Symbol("_ax", "byte")));
        Define(QSharedPointer<Symbol>(new Symbol("_ay", "byte")));
        Define(QSharedPointer<Symbol>(new Symbol("_xy", "byte")));
    }
    if (Syntax::s.m_currentSystem->m_system==AbstractSystem::AMIGA) {

        for (int i=0;i<8;i++) {
            QString k = QString::number(i);
            Define(QSharedPointer<Symbol>(new Symbol("copper_spritestruct"+k, "integer")));
            Define(QSharedPointer<Symbol>(new Symbol("copper_spritedata"+k, "integer")));
        }

        Define(QSharedPointer<Symbol>(new Symbol("copper_bitplane0", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_bitplane1", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_bitplane2", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_bitplane3", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_bitplane4", "integer")));
        //  Define(new Symbol("copper_bitplane0", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_mod_even", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_mod_odd", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_palette", "integer")));
        //        Define(QSharedPointer<Symbol>(new Symbol("copper_diwstrt", "integer")));
        //      Define(QSharedPointer<Symbol>(new Symbol("copper_diwstop", "integer")));


        Define(QSharedPointer<Symbol>(new Symbol("ddfstrt", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("ddfstop", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_resolution", "integer")));
        Define(QSharedPointer<Symbol>(new Symbol("copper_custom", "integer")));
        /*        Define(new Symbol("copper_wait_for_raster", "integer")));
        Define(new Symbol("copper_palette_start", "integer")));
        Define(new Symbol("copper_scroll", "integer")));
*/

    }
    m_addToGlobals = false;
}

bool SymbolTable::exists(QString name) {
    if (m_constants.contains(name.toUpper()))
        return true;

    if (m_symbols.contains(name))
        return true;

    return false;
}

QString SymbolTable::findSimilarSymbol(QString sim, float percentage, int n, QStringList procedures)
{
    sim = sim.remove(m_gPrefix);
    for (QString s:m_records.keys()) {
        s = s.remove(m_gPrefix);
        if (Util::QStringIsSimilar(sim,s,percentage,n,Qt::CaseInsensitive))
            return s;
    }
    for (QString s:m_symbols.keys()) {
        s = s.remove(m_gPrefix);
        if (Util::QStringIsSimilar(sim,s,percentage,n,Qt::CaseInsensitive))
            return s;
    }
    for (QString s:procedures) {
        s = s.remove(m_gPrefix);
        if (Util::QStringIsSimilar(sim,s,percentage,n,Qt::CaseInsensitive))
            return s;
    }
    for (QString s:m_constants.keys()) {
        s = s.remove(m_gPrefix);
        if (Util::QStringIsSimilar(sim,s,percentage,n,Qt::CaseInsensitive))
            return s;
    }
    return "";
}

QStringList SymbolTable::getUnusedVariables()
{
    QStringList lst;
    for (QString s : m_symbols.keys()) {
        if (!m_symbols[s]->isUsed && !s_ignoreUnusedSymbolWarning.contains(s))
            lst<<s;
    }
    return lst;
}

QSharedPointer<Symbol> SymbolTable::Lookup(QString name, int lineNumber, bool isAddress) {
    //            name = name.toUpper();

    /*    if (name.toLower()=="_a" || name.toLower()=="_x"  || name.toLower()=="_z")
        return nullptr;

*/

    if (m_constants.contains(name.toUpper())) {
        return m_constants[name.toUpper()];
    }
    // Create address on the fly

    if (name.startsWith("$")) name=name.toUpper();
    //        qDebug() <<name << " exists: " <<m_symbols.contains(name) ;

    // if ((isAddress || name.startsWith("$")) && !m_symbols.contains(name) ) {
    if ((name.startsWith("$")) && !m_symbols.contains(name) ) {
        //            qDebug() << "Creating new symbol:" << name;
        QSharedPointer<Symbol> s = QSharedPointer<Symbol>(new Symbol(name, "address"));
        s->setIsUsed();
        m_symbols[name] = s;
        return s;
    }
    bool forceGlobal = false;

    if (name.toLower().startsWith("global_")) {
        //qDebug() << "FORCE GLOBAL " <<name;
        //        if (pass==0)
        name = name.replace("global_","");

    }

    if (name.toLower().startsWith("g_global_")) {
        //qDebug() << "FORCE GLOBAL " <<name;
        //        if (pass==0)
        name = name.replace("g_global_","");
        if (pass==1) {
            forceGlobal = true;
        }

    }


    QString localName = name;
    if (!isRegisterName(name) &&!forceGlobal && !m_ignoreAllprefixes) {
        localName = m_currentProcedure+name;
        //        qDebug() << "LOCAL NAME "<<localName << SymbolTable::pass;
    }

    QString localUnitName = name;
    if (!isRegisterName(name))
        localUnitName = m_currentUnit+name;



    //    qDebug() << "SYMTAB "<< localName << name;;
    //    if (name.contains("posX"))
    //qDebug() << "SYMTAB ERROR Looking up : "<<name <<localName <<m_symbols.keys();
    name = name.remove("#");

    if (!m_symbols.contains(name) && !m_symbols.contains(localName)&& !m_symbols.contains(localUnitName)) {
        QString similarSymbol = findSimilarSymbol(name,85,2,QStringList());
        //        qDebug() << name << m_forwardedSymbols.keys();
        if (m_forwardedSymbols.contains(name))
            return m_forwardedSymbols[name];

        // IF you are in a class, you might want to look up a GLOBAL variable in that class
        if (m_currentClass!="") {
            auto n = name;
            n = n.replace(m_currentClass+"_", m_currentUnit);
            if (m_symbols.contains(n))
                return m_symbols[n];
            if (m_constants.contains(n))
                return m_constants[n];
        }
        // Check if it is a global variable used from within a class
        // IN dispatcher.
        if (pass==1)
            for (auto r:m_units)
            if (m_symbols.contains(r+"_"+name))
                ErrorHandler::e.Error("Could not find global variable '<font color=\"#FF8080\">" + name + "'</font>. If you declared '<font color=\"#FF8080\">"+name+"</font>' as a global procedure variable within a class, then please declare it as '<font color=\"#FF8080\">"+r+"::"+name+"'</font>.", lineNumber);


        similarSymbol = similarSymbol.remove(m_gPrefix);
        QString em = "";
        if (similarSymbol!="") {
            em+="Did you mean '<font color=\"#A080FF\">"+similarSymbol+"</font>'?<br>";
        }
        //        qDebug() << "SYMTAB HERE " << "NAME "<< name <<   "    UNIT "<< m_currentUnit<<localName <<m_symbols.keys();;


        ErrorHandler::e.Error("Could not find variable '<font color=\"#FF8080\">" + name + "'</font>.<br>"+em, lineNumber);
        return nullptr;
    }
    if (m_symbols.contains(localUnitName) && !m_symbols.contains(name)) {
        //        qDebug() << "SWITCHING FROM "<<name <<localUnitName;
        name = localUnitName;
    }
    if (m_symbols.contains(localName) &&!forceGlobal) {
        //    qDebug() << "Found local name " << localName << forceGlobal;
        m_symbols[localName]->setIsUsed();

        if (m_symbols.contains(name))
            m_symbols[name]->setIsUsed();
        return m_symbols[localName];

    }
    if (m_symbols[name]==nullptr)
        ErrorHandler::e.Error("Could not find variable '<font color=\"#FF8080\">" + name + "'</font>.<br>", lineNumber);

    m_symbols[name]->setIsUsed();
    //  if (forceGlobal)
    //    qDebug() << "ISUSED " <<  name << forceGlobal <<m_symbols[name]->m_name;
    return m_symbols[name];
}

QSharedPointer<Symbol> SymbolTable::LookupVariables(QString name, int lineNumber) {
    if (!m_symbols.contains(name)) {
        ErrorHandler::e.Error("Symbol/variable '" + name + "' does not exist in the current scope", lineNumber);
        return nullptr;
    }
    m_symbols[name]->isUsed=true;
    return m_symbols[name];
}

QSharedPointer<Symbol> SymbolTable::LookupConstants(QString name)
{
    if (m_constants.contains(name)) {
        return m_constants[name];
    }
    // Removes the current unit prefix and tests for constants.
    if (name.startsWith(m_gPrefix.toUpper())){
        QString n = name.remove(0,m_gPrefix.length());
        if (m_constants.contains(n)) {
            return m_constants[n];
        }
    }
    return nullptr;
}

QString SymbolTable::strip(QString val)
{
    return val.remove(m_gPrefix);
}

bool SymbolTable::isRegisterName(QString sn)
{
    sn = sn.toLower().trimmed();
    return Syntax::s.m_currentSystem->m_registers.contains(sn);
}

bool SymbolTable::isThisPointer(QString sn)
{
    return sn.toLower().endsWith("_this");
}

int SymbolTable::getShiftedPositionOfVariable(QString var, int mul)
{
    if (!m_orderedByDefinition.contains(var))
        ErrorHandler::e.Error("Record/class '"+m_name+"' does not contain any variable '"+var +"'");

    int cnt = 0;
    int cur = 0;

    while (m_orderedByDefinition[cur]!=var) {
        cnt+= m_symbols[m_orderedByDefinition[cur]]->m_size*mul;
        cur+=1;

    }
    //   qDebug() << "Counting length for "<<var<<cnt;
    return cnt;

}


void Symbol::setIsUsed()
{
    isUsed = true;
    if (Symbol::s_currentProcedure!="")
        if (!isUsedBy.contains(Symbol::s_currentProcedure))
            isUsedBy<<Symbol::s_currentProcedure;

    //    qDebug() <<"SYMBOLTABLE : "<< m_name <<isUsedBy;

}

TokenType::Type Symbol::getTokenType() {
    //qDebug() << "gettokentype: " <<m_name <<" : "<<m_type;
    if (m_type.toLower()=="integer")
        return TokenType::INTEGER;
    if (m_type.toLower()=="word")
        return TokenType::INTEGER;
    if (m_type.toLower()=="float")
        return TokenType::REAL;
    if (m_type.toLower()=="address")
        return TokenType::ADDRESS;
    if (m_type.toLower()=="pointer")
        return TokenType::POINTER;
    if (m_type.toLower()=="byte")
        return TokenType::BYTE;
    if (m_type.toLower()=="string")
        return TokenType::STRING;
    if (m_type.toLower()=="cstring")
        return TokenType::CSTRING;
    if (m_type.toLower()=="incbin")
        return TokenType::INCBIN;
    if (m_type.toLower()=="long")
        return TokenType::LONG;
    if (m_type.toLower()=="incsid")
        return TokenType::INCSID;
    if (m_type.toLower()=="record")
        return TokenType::RECORD;
    if (m_type.toLower()=="class")
        return TokenType::CLASS;
    if (m_type.toLower()=="global")
        return TokenType::GLOBAL;
    if (m_type.toLower()=="pure")
        return TokenType::PPURE;
    if (m_type.toLower()=="pure_variable")
        return TokenType::PURE_VARIABLE;
    if (m_type.toLower()=="pure_number")
        return TokenType::PURE_NUMBER;

    return TokenType::NADA;
}

Symbol::Symbol(QString name, QString type) {
    m_name = name;
    m_type = type;
    m_value = QSharedPointer<PVar>(new PVar());
}

Symbol::Symbol(QString name, QString type, long var) {
    m_name = name;
    m_type = type;
    m_value = QSharedPointer<PVar>(new PVar(var));

}

Symbol::Symbol(QString name, QString type, QString var) {
    m_name = name;
    m_type = type;;
    m_value = QSharedPointer<PVar>(new PVar(var));
}

int Symbol::getLength() {
    return m_size;
}

void Symbol::setSizeFromCountOfData(int cnt)
{
    m_size = std::max(cnt,1);
    //  if (getEndType().toLower()=="record")
    //    return;
    m_size*=getCountingLength();
    //qDebug() << "SYMBOL " <<m_name<< getCountingLength()<<cnt<<m_size<<getEndType()<<m_type<<m_arrayTypeText<<m_pointsTo;


}

int Symbol::getCountingLength()
{
    QString type = getEndType();

    if (SymbolTable::s_classSizes.contains(type)) {
        return SymbolTable::s_classSizes[type];
    }
    int l = 1;
    if (type.toLower() == "integer")
        l = 2;
    if (type.toLower() == "long")
        l = 4;
    // Actual pointer
    if (type.toLower() == "pointer")
        return Syntax::s.m_currentSystem->getPointerSize();


    return l;
}

QString Symbol::getEndType()
{
    if (m_type.toLower()=="array")
        return m_arrayTypeText;
    if (m_type.toLower()=="pointer") {
        if (m_pointsTo!="")
            return m_pointsTo;
        else return "pointer";
    }
    return m_type;
}


int SymbolTable::getSize()
{
    int cnt = 0;
    for (QString s: m_orderedByDefinition) {
        cnt+= m_symbols[s]->m_size;
        //        qDebug() << "Size of "<<s <<m_symbols[s]->m_size;
    }

    return cnt;

}


SymbolPointer::SymbolPointer() {}

SymbolPointer::SymbolPointer(QString name, int ln, QString file) {
    m_ln = ln;
    m_file = file;
    m_name = name;
}
