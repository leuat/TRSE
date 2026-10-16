// Force-included when building against Qt 5.6 (Windows XP build).
#pragma once
#ifdef QT_CORE_LIB
#include <cstddef>
#include <QtCore/QSharedPointer>
#include <QtCore/QByteArray>
#include <QtCore/QtGlobal>
typedef int qsizetype;
#endif
