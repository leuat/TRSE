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

        delete m_items[m_current];
        m_items.remove(m_current);

        if (m_current>=m_items.count())
            m_current = m_items.count()-1;
    }

}
