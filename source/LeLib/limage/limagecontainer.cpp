#include "limagecontainer.h"


//template <class T>
LImageContainer::LImageContainer()
{

}


//template <class T>
void LImageContainer::Prev()  {
    if (m_current>0)
        m_current--;
}

//template <class T>
void LImageContainer::Next()  {
    if (m_current<m_items.count()-1)
        m_current++;
}

//template<class T>
void LImageContainer::Delete()
{
    if (m_items.count()>1) {

        if (m_current>=m_items.count())
            m_current = m_items.count()-1;

        //delete m_items[m_current];
        m_items.remove(m_current);

        if (m_current>=m_items.count())
            m_current = m_items.count()-1;
    }

}

void LImageContainer::Shift(int val)
{
    int nxt = m_current+val;
    if (nxt<0)
        return;
    if (nxt>=m_items.count())
        return;
    if (m_items.count()<=1)
        return;

    auto tmp = m_items[m_current];
    m_items[m_current] = m_items[nxt];
    m_items[nxt] = tmp;

    m_current = nxt;


}
