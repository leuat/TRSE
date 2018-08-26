#ifndef NODEVARDECL_H
#define NODEVARDECL_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"
#include "source/Compiler/ast/nodevar.h"
#include "source/Compiler/ast/nodevartype.h"
#include "source/Compiler/misc/sidfile.h"

class NodeVarDecl : public Node {
public:
    Node* m_varNode = nullptr;
    Node* m_typeNode;
    SidFile sid;

    int m_pushedPointers = 0;

    NodeVarDecl(Node* varNode, Node* typeNode) {
        m_varNode = varNode;
        m_typeNode = typeNode;
    }

    void Delete() override {
        if (m_varNode) {
            m_varNode->Delete();
            delete m_varNode;
        }
        if (m_typeNode) {
            m_typeNode->Delete();
            delete m_typeNode;
        }

    }


    void IncSid(Assembler* as) {

        // Init address or load address? hmmm


        QString pos = QString::number(sid.m_loadAddress,16);
        Appendix app("$"+pos);

        app.Append("org $" +pos,1);
//        as->Appendix(v->value,0);
        app.Append("incbin \"" + as->m_projectDir + sid.m_outFile + "\"",1);

        as->m_appendix.append(app);

        int size=0;
        QFile f(as->m_projectDir + sid.m_outFile);
        if (f.open(QIODevice::ReadOnly)){
            size = f.size();  //when file does open.
            f.close();
        }

        as->blocks.append(MemoryBlock(sid.m_loadAddress,sid.m_loadAddress+size, MemoryBlock::MUSIC, sid.m_fileName));


    }

    void IncBin(Assembler* as) {
        NodeVar* v = (NodeVar*)m_varNode;
        NodeVarType* t = (NodeVarType*)m_typeNode;
        QString filename = as->m_projectDir + "/" + t->m_filename;
        if (!QFile::exists(filename))
            ErrorHandler::e.Error("Could not locate binary file for inclusion :" +filename);


        int size=0;
        QFile f(filename);
        if (f.open(QIODevice::ReadOnly)){
            size = f.size();  //when file does open.
            f.close();
        }


        if (t->m_position=="") {
            as->Label(v->value);
            as->Asm("incbin \"" + filename + "\"");
        }
        else {
//            qDebug() << "bin: "<<v->value << " at " << t->m_position;
            Appendix app(t->m_position);

            app.Append("org " +t->m_position,1);
            app.Append(v->value,0);
            app.Append("incbin \"" + filename + "\"",1);
            as->m_appendix.append(app);
            bool ok;
            int start=0;
            if (t->m_position.startsWith("$")) {
               start = t->m_position.remove("$").toInt(&ok, 16);
            }
            else start = t->m_position.toInt();

            as->blocks.append(MemoryBlock(start,start+size, MemoryBlock::DATA,t->m_filename));

        }
    }

    void InitSid(QString projectDir) {
        NodeVar* v = (NodeVar*)m_varNode;
        NodeVarType* t = (NodeVarType*)m_typeNode;


        int headerShift = 0;
        if (t->m_position!="") {
            QString val = t->m_position;
            bool ok;
            headerShift = val.toInt(&ok);
            if (!ok)
                headerShift = 0;
        }

        sid.Load(t->m_filename, projectDir);
        sid.Convert(headerShift);
/*        qDebug() << "SID LOAD: " << QString::number(sid.m_loadAddress,16);
        qDebug() << "SID INIT: " << QString::number(sid.m_initAddress,16);
       qDebug() << "SID PLAY: " << QString::number(sid.m_playAddress,16);
*/
       if (sid.m_loadAddress ==0 ) {
           qDebug() << "BUG load sid at ZERO?";
           sid.m_loadAddress = sid.m_initAddress;
       }
        SymbolTable::DefineSid(sid.m_initAddress, sid.m_playAddress);

    }

    void DeclarePointer(Assembler*  as) {

        if (!as->CheckZPAvailability())
            ErrorHandler::e.Error("Could not allocate more free pointers! Please free some up, or declare more in the settings page. ", m_op.m_lineNumber);

        NodeVarType* t = (NodeVarType*)m_typeNode;
        QString initVal = t->initVal;

        if (initVal=="") {
            initVal = as->PushZeroPointer();
            m_pushedPointers++;
        }

        NodeVar* v = (NodeVar*)m_varNode;
        as->Label(v->value + "\t= " + initVal);

    }

    QString Build(Assembler* as) {
        Node::Build(as);

        ExecuteSym(as->m_symTab);

        NodeVar* v = (NodeVar*)m_varNode;
        NodeVarType* t = (NodeVarType*)m_typeNode;


        if (t->m_op.m_type==TokenType::ARRAY) {
            as->DeclareArray(v->value, t->m_arrayVarType.m_value, t->m_op.m_intVal, t->m_data, t->m_position);
            //qDebug() << "IS: " << TokenType::types[as->m_symTab->Lookup(v->value)->getTokenType()];
            as->m_symTab->Lookup(v->value, m_op.m_lineNumber)->m_type="address";
            return "";
        }
        if (t->m_op.m_type==TokenType::STRING) {
            as->DeclareString(v->value, t->m_data);
            return "";
        }
        if (t->m_op.m_type==TokenType::CSTRING) {
            as->DeclareCString(v->value, t->m_data);
            return "";
        }
        if (t->m_op.m_type==TokenType::INCBIN) {
            IncBin(as);
            return "";
        }
        if (t->m_op.m_type==TokenType::INCSID) {
            IncSid(as);
            return "";
        }
        if (t->m_op.m_type==TokenType::POINTER) {
            DeclarePointer(as);
            return "";
        }


        as->DeclareVariable(v->value, t->value, t->initVal);
        return "";
    }


    PVar Execute(SymbolTable* symTab, uint lvl) override {
        Pmm::Data::d.Set(m_op.m_lineNumber, m_op.m_currentLineText);
        level = lvl+1;
        ErrorHandler::e.DebugLow("Calling VarDecl",level);
        return PVar();

    }
    void ExecuteSym(SymbolTable* symTab) override {

        QString typeName = ((NodeVar*)m_typeNode)->value;
        QString varName = ((NodeVar*)m_varNode)->value;
        //if (symTab->Lookup(varName)!=nullptr)
         //       ErrorHandler::e.Error("Variable '" + varName +"' is already defined!",m_op.m_lineNumber);



        Symbol* typeSymbol = symTab->Lookup(typeName, m_op.m_lineNumber);
//        if (typeSymbol==nullptr)
  //          ErrorHandler::e.Error("Could not find type symbol :" + typeName,m_op.m_lineNumber);

        ErrorHandler::e.DebugLow("Typename define : " + typeName + "  variable " + varName);




        Symbol* varSymbol = new VarSymbol(varName, typeSymbol->m_name);
        symTab->Define(varSymbol);
       // qDebug() << "Nodevardecl:ExecuteSym " << varName << " " << varSymbol->m_type;

    }


};



#endif // NODEVARDECL_H
