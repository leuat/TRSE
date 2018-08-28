#ifndef ERRORHANDLER_H
#define ERRORHANDLER_H


#include <QString>
#include <QDebug>
#include "source/Compiler/data_pmm.h"
#include <stdio.h>
#include <iostream>
#include <QTextEdit>
#include <exception>
using namespace std;


class FatalErrorException: public exception
{
public:
    QString message;
    QString file = "";
    int linenr;
    FatalErrorException() {}
    FatalErrorException(QString msg)
    {
        message = msg;
    }
    FatalErrorException(QString msg, int ln)
    {
        message = msg;
        linenr = ln;
    }
};

class ErrorHandler
{
public:
    ErrorHandler();
    static ErrorHandler e;
    int m_level = 0;

    int ERROR_ONLY = 2;
    int DEBUG_HIGH = 1;
    int DEBUG_LOW = 0;

    QString m_teOut;


    void OutputText(QString str) {
        std::cout << str.toStdString() << std::endl;
        m_teOut = m_teOut + str + "\n";
    }

    void Message(QString str, int lvl=0) {
        QString s ="";
        for (int i=0;i<lvl;i++)
            s+=" ";

        s = s + str + "\n";
        std::cout << s.toStdString();
        m_teOut = m_teOut + s;

    }

    void DebugHigh(QString str, int lvl=0) {
        if (m_level<=DEBUG_HIGH)
            Message(str, lvl);
    }

    void DebugLow(QString str, int lvl=0) {
        if (m_level<=DEBUG_LOW)
            Message(str, lvl);
    }
    bool exitOnError= true;

    void Error(QString str, int lineNumber=0) {
        throw FatalErrorException(str, lineNumber);
        //Message(str);

        //if (exitOnError)
        //    exit(1);
    }

    void CatchError(FatalErrorException e, QString extraMessage="") {
        Message(extraMessage + e.message);
        if (exitOnError)
            exit(1);
    }





};

#endif // ERRORHANDLER_H
