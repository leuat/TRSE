#include "symboltable.h"
#include <qmath.h>
bool SymbolTable::isInitialized = false;
int SymbolTable::m_currentSid = 0;
QMap<QString, Symbol*> SymbolTable::m_constants;

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
    m_constants[init] = new Symbol("$"+QString::number(initAddress,16),"ADDRESS", initAddress);
    m_constants[play] = new Symbol("$"+QString::number(playAddress,16),"ADDRESS", playAddress);


}

void SymbolTable::Initialize()
{
    if (isInitialized)
        return;
    //    m_constants["PI"] = new Symbol("PI","4REAL", M_PI);
    m_constants["SCREEN_BG_COL"] = new Symbol("^53280","ADDRESS", 53280);
    m_constants["SCREEN_FG_COL"] = new Symbol("^53281","ADDRESS", 53281);
    m_constants["SCREEN_CHAR_LOC"] = new Symbol("^$0400","ADDRESS", 0x0400);
    m_constants["SCREEN_COL_LOC"] = new Symbol("^$D800","ADDRESS", 0xD800);
    m_constants["RASTERLINE_POS"] = new Symbol("^$D012","ADDRESS", 0xd012);
    m_constants["BLACK"] = new Symbol("0","BYTE", 0);
    m_constants["WHITE"] = new Symbol("1","BYTE", 1);
    m_constants["RED"] = new Symbol("2","BYTE", 2);
    m_constants["CYAN"] = new Symbol("3","BYTE", 3);
    m_constants["PURPLE"] = new Symbol("4","BYTE", 4);
    m_constants["GREEN"] = new Symbol("5","BYTE", 5);
    m_constants["BLUE"] = new Symbol("6","BYTE", 6);
    m_constants["YELLOW"] = new Symbol("7","BYTE", 7);
    m_constants["ORANGE"] = new Symbol("8","BYTE", 8);
    m_constants["BROWN"] = new Symbol("9","BYTE", 9);
    m_constants["LIGHT_RED"] = new Symbol("10","BYTE", 10);
    m_constants["DARK_GREY"] = new Symbol("11","BYTE", 11);
    m_constants["GREY"] = new Symbol("12","BYTE", 12);
    m_constants["LIGHT_GREEN"] = new Symbol("13","BYTE", 13);
    m_constants["LIGHT_BLUE"] = new Symbol("14","BYTE", 14);
    m_constants["LIGHT_GREY"] = new Symbol("15","BYTE", 15);

    m_constants["SPRITE_POS"] = new Symbol("^$D000","ADDRESS", 0xD000);
    m_constants["SPRITE_DATA_LOC"] = new Symbol("^$07f8", "ADDRESS", 0x07f8);
    m_constants["SPRITE_BITMASK"] = new Symbol("^$d015", "ADDRESS", 0xd015);
    m_constants["SPRITE_COLOR"] = new Symbol("^53287","ADDRESS", 0xD027);
    m_constants["SPRITE_DATA"] = new Symbol("^$0340","ADDRESS", 0x0340);
    m_constants["SPRITE_COLLISION"] = new Symbol("^$D01E","ADDRESS", 0xD01E);
    m_constants["SPRITE_BG_COLLISION"] = new Symbol("^$D01F","ADDRESS", 0xD01F);
    m_constants["SPRITE_STRETCH_X"] = new Symbol("^$D01D", "ADDRESS", 0xD01D);
    m_constants["SPRITE_STRETCH_Y"] = new Symbol("^$D017", "ADDRESS", 0xD017);

    m_constants["SPRITE_MULTICOLOR"] = new Symbol("^$D01C", "ADDRESS", 0xD01C);

    m_constants["SPRITE_MULTICOLOR_REG1"] = new Symbol("^$D025", "ADDRESS", 0xD025);
    m_constants["SPRITE_MULTICOLOR_REG2"] = new Symbol("^$D026", "ADDRESS", 0xD026);



    m_constants["MULTICOLOR_CHAR_COL"] = new Symbol("^$D021", "ADDRESS", 0xD021);


    m_constants["SID_TRI"] = new Symbol("16","BYTE", 16);
    m_constants["SID_SAW"] = new Symbol("32","BYTE", 32);
    m_constants["SID_PULSE"] = new Symbol("64","BYTE", 64);
    m_constants["SID_NOISE"] = new Symbol("128","BYTE", 128);
    m_constants["SID_CHANNEL1"] = new Symbol("0","BYTE", 0);
    m_constants["SID_CHANNEL2"] = new Symbol("7","BYTE", 7);
    m_constants["SID_CHANNEL3"] = new Symbol("14","BYTE", 14);

    m_constants["SID"] = new Symbol("^54272","ADDRESS", 54272);

    m_constants["SIDFILE_1_INIT"] = new Symbol("","ADDRESS", 0);
    m_constants["SIDFILE_2_INIT"] = new Symbol("","ADDRESS", 0);
    m_constants["SIDFILE_1_PLAY"] = new Symbol("","ADDRESS", 0);
    m_constants["SIDFILE_2_PLAY"] = new Symbol("","ADDRESS", 0);

    m_constants["VIC_BANK0"] = new Symbol("3","BYTE", 3);
    m_constants["VIC_BANK1"] = new Symbol("2","BYTE", 2);
    m_constants["VIC_BANK2"] = new Symbol("1","BYTE", 1);
    m_constants["VIC_BANK3"] = new Symbol("0","BYTE", 0);

    m_constants["VIC_DATA_LOC"] = new Symbol("^$d018", "ADDRESS", 0xd018);

    for (unsigned char key: Syntax::s.m_c64keys.keys()) {
        C64Key k = Syntax::s.m_c64keys[key];
        m_constants[k.m_key] = new Symbol(QString::number(k.m_value), "BYTE",  k.m_value);
    }

}

void SymbolTable::Delete() {
    for (QString val : m_symbols.keys()) {
        Symbol* s = m_symbols[val];
        if (s!=nullptr) {
            if (s->m_value)
                delete s->m_value;

        }
        delete s;
    }

    // Delete static constants as well
    if (isInitialized) {
        for (QString val : m_constants.keys()) {
            Symbol* s = m_symbols[val];

            if (s!=nullptr) {
                if (s->m_value)
                    delete s->m_value;

            }
            delete s;
        }
        isInitialized = false;

    }
}

void SymbolTable::setName(QString s) {
    m_name = s;
}

void SymbolTable::InitBuiltins()
{

    Define(new BuiltInTypeSymbol("INTEGER",""));
    Define(new BuiltInTypeSymbol("REAL",""));
    Define(new BuiltInTypeSymbol("BYTE",""));
    Define(new BuiltInTypeSymbol("STRING",""));
    Define(new BuiltInTypeSymbol("CSTRING",""));
    Define(new BuiltInTypeSymbol("POINTER",""));
    Define(new BuiltInTypeSymbol("ARRAY",""));
    Define(new BuiltInTypeSymbol("INCBIN",""));
    Define(new BuiltInTypeSymbol("INCSID",""));

    Define(new Symbol("return",""));
    Define(new Symbol("sine", "address"));
    Define(new Symbol("joystickup", "byte"));
    Define(new Symbol("joystickdown", "byte"));
    Define(new Symbol("joystickleft", "byte"));
    Define(new Symbol("joystickright", "byte"));
    Define(new Symbol("joystickbutton", "byte"));

/*    Define(new Symbol("zeropage1", "pointer"));
    Define(new Symbol("zeropage2", "pointer"));
    Define(new Symbol("zeropage3", "pointer"));
    Define(new Symbol("zeropage4", "pointer"));
    Define(new Symbol("zeropage5", "pointer"));
    Define(new Symbol("zeropage6", "pointer"));
    Define(new Symbol("zeropage7", "pointer"));*/
    Define(new Symbol("screenmemory", "pointer"));

}

Symbol *SymbolTable::Lookup(QString name, int lineNumber, bool isAddress) {
    //        name = name.toUpper();
    if (m_constants.contains(name.toUpper())) {
        return m_constants[name.toUpper()];
    }
    // Create address on the fly

    if (name.startsWith("$")) name=name.toUpper();
    //        qDebug() <<name << " exists: " <<m_symbols.contains(name) ;

    // if ((isAddress || name.startsWith("$")) && !m_symbols.contains(name) ) {
    if ((name.startsWith("$")) && !m_symbols.contains(name) ) {
        //            qDebug() << "Creating new symbol:" << name;
        Symbol* s = new Symbol(name, "address");
        m_symbols[name] = s;
        return s;
    }


    if (!m_symbols.contains(name)) {
        ErrorHandler::e.Error("Could not find variable '" + name + "'.", lineNumber);
        return nullptr;
    }
    //qDebug() << name << " " << m_symbols[name]->m_type;
    //        qDebug() << "FOUND "<< name;

    return m_symbols[name];
}

Symbol *SymbolTable::LookupVariables(QString name, int lineNumber) {
    if (!m_symbols.contains(name)) {
        ErrorHandler::e.Error("Symbol/variable '" + name + "' does not exist in the current scope", lineNumber);
        return nullptr;
    }
    return m_symbols[name];
}

Symbol *SymbolTable::LookupConstants(QString name) {
    if (m_constants.contains(name)) {
        return m_constants[name];
    }
    return nullptr;
}


TokenType::Type Symbol::getTokenType() {
    //qDebug() << "gettokentype: " <<m_name <<" : "<<m_type;
    if (m_type.toLower()=="integer")
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
    if (m_type.toLower()=="incsid")
        return TokenType::INCSID;
    return TokenType::NADA;
}

Symbol::Symbol(QString name, QString type) {
    m_name = name;
    m_type = type;
    m_value = new PVar();

}

Symbol::Symbol(QString name, QString type, float var) {
    m_name = name;
    m_type = type;
    m_value = new PVar(var);
}

Symbol::Symbol(QString name, QString type, QString var) {
    m_name = name;
    m_type = type;
    m_value = new PVar(var);
}
