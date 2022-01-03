#include "systemx16.h"


SystemX16::SystemX16(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj) : System65C816(settings, proj) {
    m_processor = WDC65C816;
    m_system = X16;
    m_systemColor = QColor(100,60,40);

}
