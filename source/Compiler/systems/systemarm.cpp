#include "systemarm.h"


SystemArm::SystemArm(QSharedPointer<CIniFile> settings, QSharedPointer<CIniFile> proj): AbstractSystem(settings, proj) {
        m_allowedGlobalTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" << "chipmem" <<"aligned" << "compressed";
        m_allowedProcedureTypeFlags << "pure"<<"pure_variable" <<"pure_number" << "signed" <<"no_term" <<"global" << "aligned";
        m_processor = ARM;
        m_allowedBaseTypes<<"LONG";
}
