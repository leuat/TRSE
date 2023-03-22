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

#include "nodevar.h"


NodeVar::NodeVar(Token t):Node() {
    m_op = t;
    value = t.m_value;
}

NodeVar::NodeVar(Token t, QSharedPointer<Node> expr) : Node() {
    m_op = t;
    value = t.m_value;
    m_expr = expr;
    //  if (m_op.m_type==TokenType::INTEGER)
    //     m_isWord = true;

}

void NodeVar::ReplaceInline(Assembler* as,QMap<QString, QSharedPointer<Node> > &inp)
{
    if (m_expr!=nullptr)
        m_expr->ReplaceInline(as,inp);

/*
    for (QString k: inp.keys()) {
        qDebug() << "HERRE NODEVAR" << k ;
        if (k==value) {
            value = inp[k]->getValue(as);
        }
    }*/
}

void NodeVar::ReplaceVariable(Assembler *as, QString name, QSharedPointer<Node> node)
{
    Node::ReplaceVariable(as,name,node);
    if (m_expr!=nullptr) {
        if (m_expr->isPureVariable() && m_expr->getValue(as)==name)
            m_expr = node;
        m_expr->ReplaceVariable(as,name,node);
    }

}



TokenType::Type NodeVar::getOrgType(Assembler *as) {


    if (as==nullptr) {
        // Use parser symbtab
        TokenType::Type t = m_op.m_type;
        if (parserSymTab != nullptr) {
            QSharedPointer<Symbol> s = parserSymTab->Lookup(value, m_op.m_lineNumber);
            //            qDebug() << "NodeVar::getType "<< s->m_name << TokenType::getType(s->getTokenType()) << " with forcetype " << TokenType::getType(m_loadType);
            if (s!=nullptr)
                t= s->getTokenType();
        }

        return t;
    }

//    if (m_subNode!=nullptr)
  //      return qSharedPointerDynamicCast<NodeVar>(m_subNode)->getOrgType(as);


    TokenType::Type t = m_op.m_type;
    if (as->m_symTab->Lookup(getValue(as), m_op.m_lineNumber)!=nullptr)
        t= as->m_symTab->Lookup(getValue(as), m_op.m_lineNumber)->getTokenType();

//    qDebug() << "NodeVar TYPE " +getValue(as) + "  "+as->m_symTab->Lookup(value, m_op.m_lineNumber)->m_type << " " << as->m_symTab->Lookup(value, m_op.m_lineNumber)->m_arrayTypeText;

    return t;
}



TokenType::Type NodeVar::getType(Assembler *as) {

    if (m_op.m_isBoolean)
            return TokenType::BOOLEAN;
    if (as==nullptr) {
        // Use parser symbtab
        TokenType::Type t = m_op.m_type;
        if (parserSymTab != nullptr) {
            QSharedPointer<Symbol> s = parserSymTab->Lookup(value, m_op.m_lineNumber);
//            qDebug() << "NodeVar::getType "<< s->m_name << TokenType::getType(s->getTokenType()) << " with forcetype " << TokenType::getType(m_loadType);
            if (s!=nullptr)
                t= s->getTokenType();

        }
        //qDebug() << "Forcetype = " << TokenType::getType(m_loadType);
        if ((m_loadType!=TokenType::NADA))// && (t!=TokenType::POINTER && m_expr==nullptr))
            return m_loadType;

        return t;
    }

    TokenType::Type t = m_op.m_type;
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(value, m_op.m_lineNumber);
//    qDebug() << "VAL "<< getValue(as) ;

    if (s!=nullptr)
        t= as->m_symTab->Lookup(getValue(as), m_op.m_lineNumber)->getTokenType();





    if (m_loadType!=TokenType::NADA && t!=TokenType::POINTER)
        return m_loadType;

//    qDebug() <<   " " + TokenType::getType(t);

//    if (as->m_symTab->Lookup(value, m_op.m_lineNumber)!=nullptr)
  //      return as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType();

    return t;
}



TokenType::Type NodeVar::getArrayType(Assembler *as)
{
    TokenType::Type t = m_op.m_type;
    if (as==nullptr)
        return t;
    if (as->m_symTab->Lookup(getValue(as), m_op.m_lineNumber)!=nullptr)
        t= as->m_symTab->Lookup(getValue(as), m_op.m_lineNumber)->m_arrayType;


/*    if (m_subNode!=nullptr) {
        qDebug() << getValue(as) << TokenType::getType(as->m_symTab->Lookup(getValue(as), m_op.m_lineNumber)->m_arrayType);

//         Ok this is an array of a record. Need to find the actual type
//        if (m_subNode!=nullptr) {
  //          QSharedPointer<Symbol> s = as->m_symTab->Lookup(v,m_op.m_lineNumber,true);

    }
*/
/*    if (m_loadType!=TokenType::NADA && t!=TokenType::POINTER)
        return m_loadType;
*/
    return t;
}

bool NodeVar::isPointer(Assembler *as)
{
    if (m_loadType==TokenType::POINTER)
        return true;
    return as->m_symTab->Lookup(value, m_op.m_lineNumber)->getTokenType()==TokenType::POINTER;

}

bool NodeVar::isPurePointer(Assembler *as)
{
    return (isPointer(as) && (m_expr==nullptr));
}



bool NodeVar::DataEquals(QSharedPointer<Node> other) {
    QSharedPointer<NodeVar> var = qSharedPointerDynamicCast<NodeVar>(other);
    if (var==nullptr)
        return false;
    return var->value==value;
}

bool NodeVar::isBool(Assembler* as)  {
//    if (getType(as)==TokenType::ADDRESS)
  //      return (getArrayType(as)==TokenType::BOOLEAN);
    if (getType(as)==TokenType::POINTER && m_expr!=nullptr) {
        if (getArrayType(as)==TokenType::BOOLEAN)
            return true;
    }
    return getType(as)==TokenType::BOOLEAN;

}

bool NodeVar::isWord(Assembler *as) {

    //    if (getType(as)==TokenType::POINTER)
    //    qDebug() << "NodeVar isWord : is array: " << (getType(as)==TokenType::ADDRESS) << TokenType::getType(getType(as));
    //    if (getType(as)==TokenType::ADDRESS && m_expr!=nullptr)
    if (isReference())
        return true;

    if (getType(as)==TokenType::ADDRESS)
        return (getArrayType(as)==TokenType::INTEGER);
    //qDebug() << "  Array type : " << getArrayType(as);
    // qDebug() << "NodeVar is Word : " << (getType(as)==TokenType::ARRAY) << getArrayType(as);
    //    if (m_fake16bit)
    //      qDebug() << "FAKE 16 bit: "<<value;
    if (getType(as)==TokenType::POINTER && m_expr!=nullptr) {
        if (getArrayType(as)==TokenType::INTEGER)
            return true;
        // Next one is for the z80 and 6502
        if (Syntax::s.m_currentSystem->m_processor!=AbstractSystem::PX86)
           if (m_classvariableType==TokenType::INTEGER) {
            as->Comment("IsWord TRUE pointer integer write array");
            return false;
        }
    }
//    if (m_classvariableType!=TokenType::NADA)
//        return m_classvariableType==TokenType::INTEGER;
    return getType(as)==TokenType::INTEGER || (getType(as)==TokenType::POINTER && m_expr==nullptr) || m_fake16bit;
    /*    return m_op.m_type==TokenType::INTEGER;
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(value, m_op.m_lineNumber);

    return s->m_type.toLower()=="integer";*/
}


bool NodeVar::isLong(Assembler *as) {
    return (getType(as)==TokenType::LONG || getArrayType(as)==TokenType::LONG)  && m_expr==nullptr;
}
bool NodeVar::isByte(Assembler *as) {
    return (getType(as)==TokenType::BYTE || getType(as)==TokenType::BOOLEAN)  && m_expr==nullptr;
//    return getType(as)==TokenType::BYTE  || getArrayType(as)==TokenType::BYTE;
}

bool NodeVar::isStringList(Assembler *as)
{
//    qDebug() << getValue(as) << TokenType::getType(getTyp)<< TokenType::getType(getArrayType(as));
   return (getArrayType(as)==Syntax::s.m_currentSystem->getSystemPointerArrayType());
}

bool NodeVar::containsPointer(Assembler *as)
{
    return (getType(as)==TokenType::POINTER && m_expr!=nullptr);
}

bool NodeVar::isRecord(Assembler *as)
{
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(value,m_op.m_lineNumber,true);
    QString t = s->m_type;
    if (t=="address") {
        t = s->m_arrayTypeText;
    }
    return as->m_symTab->m_records.contains(t);
}


bool NodeVar::isStackVariable()
{
    QSharedPointer<Symbol> s = s_as->m_symTab->Lookup(value,m_op.m_lineNumber,true);
    if (s==nullptr)
        return false;
    return s->m_isStackVariable;

}

int NodeVar::getStackShift()
{
    QSharedPointer<Symbol> s = s_as->m_symTab->Lookup(value,m_op.m_lineNumber,true);
    return s->m_stackPos;

}


bool NodeVar::isRecord(QSharedPointer<SymbolTable> sym, QString& str)
{
    QSharedPointer<Symbol> s = sym->Lookup(value,m_op.m_lineNumber,true);
    QString t = s->m_type;
    if (t.toLower()=="address") {
        t = s->m_arrayTypeText;
    }
    if (t.toLower()=="array") {
        t = s->m_arrayTypeText;
    }

    str = t;
    return sym->m_records.contains(t);

}


bool NodeVar::isClass(Assembler *as)
{
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(value,m_op.m_lineNumber,true);
    QString t = s->m_type;
    if (t=="address") {
        t = s->m_arrayTypeText;
    }
    return as->m_symTab->m_records.contains(t) && as->m_symTab->m_records[t]->m_isClass;
}


bool NodeVar::isRecordData(Assembler *as)
{
    return m_subNode!=nullptr;
}


bool NodeVar::typeIsArray(Assembler *as)
{
    if (as==nullptr)
        return false;

    QString v = getValue(as);
    QString t = as->m_symTab->Lookup(v, m_op.m_lineNumber)->m_type.toLower();
    //            qDebug() << v<< "TYPE" << as->m_symTab->Lookup(v, m_op.m_lineNumber)->m_type <<m_op.getType();
    if (m_op.m_type!=TokenType::ADDRESS) // const screen_bg_col etc
        if (!as->m_symTab->m_constants.contains(v))
             if ((t=="address") || t=="incbin"  || t=="incsid" || t=="incnsf" || t=="cstring" || t=="string")
                    if (!isPointer(as))
                        return true;


    return false;
}

QString NodeVar::getValue8bit(Assembler *as, int isHi) {

    if (isReference()) {
        if (isHi==1)
            return "#>"+getValue(as);
        if (isHi==0)
            return "#<"+getValue(as);
        if (isHi==2)
            return "#^"+getValue(as);
        return "";
    }
    QString pa="";
    QString pb="";
    if (Syntax::s.m_currentSystem->isZ80()){
        pa="(";pb=")";
    }
    if (isHi==1) {
        if (getOrgType(as)==TokenType::BYTE)
            return "#0";
       return pa+getValue(as)+"+1"+pb;
    }
    else
    if (isHi==2) {
        if (getOrgType(as)==TokenType::BYTE || getOrgType(as)==TokenType::INTEGER)
            return "#0";
       return pa+getValue(as)+"+2"+pb;
    }
    else return pa+getValue(as)+pb;
}



QString NodeVar::getTypeText(Assembler *as)
{
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(value,m_op.m_lineNumber,true);
    QString t = s->m_type;
    if (t=="address")
        t = s->m_arrayTypeText;
    return t;

}

QString NodeVar::getValue(Assembler* as) {
    QString v= value;
//    if (v.startsWith("$"))
  //      return v;
    if (m_ignoreLookup)
        return v;

    QSharedPointer<Symbol> s = as->m_symTab->Lookup(v,m_op.m_lineNumber,true);
    if (m_subNode!=nullptr) { // has subnode a.b
//        QSharedPointer<Symbol> s = as->m_symTab->Lookup(v,m_op.m_lineNumber,true);

        QString type = s->m_type;
        if ((m_expr!=nullptr) && !m_ignoreRecordExpr) { // Has array indices
            type = s->m_arrayTypeText;
        }

        QString test = type;
        if (type.toLower()=="pointer")
            test = s->m_pointsTo;

//        if (!as->m_symTab->m_records.contains(test))
  //          ErrorHandler::e.Error("Could not find record type : "+type + " of " + v,m_op.m_lineNumber);

        if (qSharedPointerDynamicCast<NodeVar>(m_subNode)==nullptr)
            ErrorHandler::e.Error("Unknown subnode: '"+m_subNode->getValue(as)+"'", m_op.m_lineNumber);

        v =v + "_"+type+"_"+qSharedPointerDynamicCast<NodeVar>(m_subNode)->value;//m_subNode->getValue(as);
    }
    if (as->m_symTab->getCurrentProcedure()!="") {
        //value = value.replace(as->m_symTab->getCurrentProcedure(),"");
        QString tstv = as->m_symTab->getCurrentProcedure()+value;
        if (as->m_symTab->m_symbols.contains(tstv))
                v = tstv;

    }
    if (isReference() && Syntax::s.m_currentSystem->m_processor == AbstractSystem::M68000) return "#" + v;
//    if (m_forceAddress && !(isPointer(as))) return "#" + v;
    QString test = s->m_type;
    if (s->m_type.toLower()=="pointer" || s->m_type.toLower()=="address")
        test = s->m_pointsTo;

//    qDebug() << value<<isReference() <<as->m_symTab->m_records.contains(test)<<test;
    // The following was used for trying to get pointers to records (point to 1st element)
/*    if (isReference()&&as->m_symTab->m_records.contains(test)) {
        // Trying to access a raw record/class as a pointer
        QSharedPointer<SymbolTable> r = as->m_symTab->m_records[test];
        // name the first object
        v = v+"_"+test+"_"+r->m_symbols.keys().first();
    }*/
 //   qDebug() << "NODEVAR GETVALUE " <<v << s->m_type;
    return v;
}

bool NodeVar::isAddress() {
    //  return m_op.m_type==TokenType::ADDRESS;
    return true;
}

bool NodeVar::isSigned(Assembler* as)
{
    QSharedPointer<Symbol> s = as->m_symTab->Lookup(getValue(as), m_op.m_lineNumber);
    return s->m_flags.contains("signed");
}


void NodeVar::ExecuteSym(QSharedPointer<SymbolTable> symTab) {
    QString varName = m_op.m_value;
    QSharedPointer<Symbol> varSymbol = symTab->Lookup(varName, m_op.m_lineNumber);

}
/*
 * VerifyReferences throws an error if type needs to be referenced
 *
 *
*/
void NodeVar::VerifyReferences(Assembler *as) {

    if (!isPointer(as))
        if (!isReference())
            if (!hasArrayIndex())
                if (typeIsArray(as))
                ErrorHandler::e.Error("Unknown usage of data or array. <font color=\"orange\">Did you mean to reference it? (#"+getValue(as)+")</font>",m_op.m_lineNumber);

    if (isPointer(as) && isReference() && !hasArrayIndex())
        ErrorHandler::e.Warning("Referencing the address of a pointer <font color=\"orange\">("+getValue(as)+")</font>. Is this what you intended?</font>",m_op.m_lineNumber);

}
