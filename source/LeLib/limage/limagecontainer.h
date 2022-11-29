#ifndef LIMAGECONTAINER_H
#define LIMAGECONTAINER_H

#include <QVector>
#include <QSharedPointer>

class LImageContainerItem {
public:
    int m_width, m_height;
    int m_blockSize = 8;

    virtual void Init(int w, int h) = 0;

    virtual QByteArray ToQByteArray(int) = 0;

    virtual void Clear(int val=0) = 0;


};


//template <class T>
class LImageContainer
{
public:
    LImageContainer();

    QVector<QSharedPointer<LImageContainerItem>> m_items;

    int m_current = 0;

    virtual void AddNew(int w, int h) = 0;
    void Prev();
    void Next();
    void Delete();

    void Shift(int val);

    virtual void setExtraData(int idx, char val) { return;}
    virtual char getExtraData(int idx) { return 0;}


    void DeleteAll() {
//        for (int i=0;i<m_items.count();i++)
  //          delete m_items[i];

        m_items.clear();

    }

    ~LImageContainer() {
        DeleteAll();
    }


};

#endif // LIMAGECONTAINER_H
