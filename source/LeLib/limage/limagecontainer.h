#ifndef LIMAGECONTAINER_H
#define LIMAGECONTAINER_H

#include <QVector>


class LImageContainerItem {
public:
    int m_width, m_height;
    virtual void Init(int w, int h) = 0;

    virtual QByteArray ToQByteArray(int) = 0;

    virtual void Clear() = 0;


};


//template <class T>
class LImageContainer
{
public:
    LImageContainer();

    QVector<LImageContainerItem*> m_items;

    int m_current = 0;

    virtual void AddNew(int w, int h) = 0;
    void Prev();
    void Next();
    void Delete();

    void Shift(int val);

    virtual void setExtraData(int idx, char val) { return;}
    virtual char getExtraData(int idx) { return 0;}


    void DeleteAll() {
        for (int i=0;i<m_items.count();i++)
            delete m_items[i];

        m_items.clear();

    }

    ~LImageContainer() {
        DeleteAll();
    }


};

#endif // LIMAGECONTAINER_H
