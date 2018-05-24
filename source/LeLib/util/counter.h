#pragma once

#include <string>
#include <iostream>
#include <QElapsedTimer>
#include "source/util/util.h"

using namespace std;

class Counter {
public:
    int ticks;
    float Max;
    int current, currentTick;
    string str;
    bool outputClams;
    long prevTime;
    float m_progress = 0;

    long startTime;
    QElapsedTimer ttimer;
public:
    float percent;
    Counter() {}
    Counter(int m, string s, bool output) {
        outputClams = output;
        if (outputClams)
            cout << s << endl;
        Init(m);
    }

    Counter(int m, string s) {
        str = s;
        outputClams = true;
        cout << s << endl;
        Init(m);

    }


    void Init(int m) {
        ticks = 2;
        Max = m;
        current = 0;
        currentTick = -1;
        outputClams = false;
        if (outputClams)
            cout << "[";

        ttimer.start();
        prevTime = ttimer.elapsed();
        startTime = ttimer.elapsed();

    }




    void Tick() {

        int dt = ttimer.elapsed() - prevTime;
        prevTime = ttimer.elapsed();
        percent = (current*100.0)/(float)Max;

        int totalTime = (ttimer.elapsed() - startTime)/(percent/100.0);

        int timeLeft = (ttimer.elapsed());
        m_progress = percent;

        //cout << percent << endl;
        //if (percent % ticks==0 && percent!=currentTick)
        if (outputClams)
        {
            currentTick = percent;
            cout << "\r" <<str << " " << QString::number( percent,'f',1).toStdString() << "%" << "    " << Util:: MilisecondToString(timeLeft).toStdString() << " of total "  <<Util::MilisecondToString(totalTime).toStdString() << "                                                  ";
            cout.flush();
            if (percent==100-ticks)
                if (outputClams)
                    cout << "]" << endl;;
        }
        current++;
    }

};
