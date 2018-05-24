#ifndef DATA_H
#define DATA_H


class Data
{
public:
    Data();
    float version = 0.01;
    bool redrawOutput=false;
    bool redrawInput=false;
    bool forceRedraw = false;
    bool redrawFileList = false;
    bool abort = false;
    bool requestSaveAs = false;
    bool requestCloseWindow = false;
    bool blink = false;
    float percent = 0;
    unsigned char currentColor = 0;
    bool currentIsColor = false;
    static Data data;
    void Redraw();
};

#endif // DATA_H
