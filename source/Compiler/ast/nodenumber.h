#ifndef NODENUMBER_H
#define NODENUMBER_H


#include "source/Compiler/token.h"
#include "source/Compiler/pvar.h"
#include "source/Compiler/symboltable.h"
#include "source/Compiler/errorhandler.h"
#include "source/Compiler/ast/node.h"


class NodeNumber : public Node {
public:
    float m_val;
    NodeNumber(Token op, int val) {
        m_op = op;
        m_val = val;


        if (m_op.m_type!=TokenType::ADDRESS)
            m_op.m_type = TokenType::INTEGER_CONST;

    }
    PVar Execute(SymbolTable* symTab, uint lvl) override;
    void ExecuteSym(SymbolTable* symTab) override {
    }


    bool isAddress() override {
        return m_op.m_type == TokenType::ADDRESS;
    }

    bool isPureNumeric() override {
        return true;
    }

    bool isWord(Assembler* as) override {
        return (m_val>=256 && m_op.m_type == TokenType::INTEGER_CONST);
    }

    void LoadVariable(Assembler* as) override {
        as->ClearTerm();
        as->Term("lda ");
        Build(as);
        as->Term();
    }

    int numValue() { return m_val;}

    QString HexValue() {
        return "$" + QString::number((int)m_val,16);
    }

    bool DataEquals(Node *other) override {
        NodeNumber* var = dynamic_cast<NodeNumber*>(other);
        if (var==nullptr)
            return false;
        return var->m_val == m_val;
    }



    QString StringValue() {
        QString val;
        if (m_op.m_type==TokenType::BYTE)
            val = "#"+QString::number((int)m_val);
        if (m_op.m_type==TokenType::INTEGER)
            val = "#"+QString::number((int)m_val);
        if (m_op.m_type==TokenType::INTEGER_CONST)
            val = "#"+QString::number((int)m_val);
        if (m_op.m_type==TokenType::ADDRESS) {
            val = "$" + QString::number((int)m_val,16);
        }
        return val;
    }

    bool isMinusOne() override {
        if (m_val==255)
            return true;

        return false;
    }

    bool isOne() override { return m_val==1; }
    QString Build(Assembler *as) override {
        Node::Build(as);

        QString val = "";

/*        if (as->m_symTab->m_constants.contains(m_op.m_value)) {
            m_val = as->m_symTab->m_constants[m_op.m_value]->m_value->m_fVal;
        }
*/


        if (m_op.m_type==TokenType::BYTE)
            val = "#"+QString::number((int)m_val);
        if (m_op.m_type==TokenType::INTEGER)
            val = "#"+QString::number((int)m_val);
        if (m_op.m_type==TokenType::INTEGER_CONST)
            val = "#"+QString::number((int)m_val);
        if (m_op.m_type==TokenType::ADDRESS) {

            val = "$" + QString::number((int)m_val,16);
        }

        if (m_op.m_type==TokenType::INTEGER_CONST && m_val>255) {
            as->Comment("Integer constant assigning");
            int hiBit = ((int)m_val)>>8;
            int loBit = ((int)m_val)&0xff;
            as->ClearTerm();
            as->Asm("ldy #" + QString::number(hiBit) );
//            as->Asm("tax");
            as->Asm("lda #" + QString::number(loBit) );
            return val;

            //qDebug() << m_op.m_value <<":" << m_val << " : " << hiBit << "  , " << loBit;
            //exit(1);
        }

       // qDebug() << m_op.getType() << " for " << val;;

        if (as->m_term=="")

            as->Term("lda " + val);
        else
            as->Term(val);
        return val;
    }


};

#endif // NODENUMBER_H
