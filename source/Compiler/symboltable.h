#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H

#include <QString>
#include <QMap>
#include <QDebug>
#include "errorhandler.h"
#include "pvar.h"
#include "token.h"
#include "syntax.h"

class Symbol {
public:
    QString m_name;
    QString m_type;
    int m_org = 0;
    int m_size = 0;

    PVar* m_value = nullptr;
    TokenType::Type getTokenType();
    Symbol(QString name, QString type="");
    Symbol(QString name, QString type, float var);
    Symbol(QString name, QString type, QString var);

};


class BuiltInTypeSymbol : public Symbol {
public:
    BuiltInTypeSymbol(QString name, QString type) : Symbol(name, type) {
    }
};

class VarSymbol : public Symbol {
public:
    VarSymbol(QString name, QString type) : Symbol(name, type) {

    }
};

class SymbolTable
{
public:
    QMap<QString, Symbol*> m_symbols;
    static QMap<QString, Symbol*> m_constants;
    QString m_name;
    SymbolTable();
    static SymbolTable s;


    static int m_currentSid;

    static void DefineSid(unsigned int initAddress, unsigned int playAddress);


    static bool isInitialized;
    static void Initialize();

    void Define(Symbol* s) {
        m_symbols[s->m_name] = s;
    }
    void Delete();

    void setName(QString s);

    void InitBuiltins();

    Symbol* Lookup(QString name, int lineNumber, bool isAddress=false);
    Symbol* LookupVariables(QString name, int lineNumber);

    Symbol* LookupConstants(QString name);

};

#endif // SYMBOLTABLE_H
