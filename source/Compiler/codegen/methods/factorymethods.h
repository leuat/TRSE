#ifndef FACTORYMETHODS_H
#define FACTORYMETHODS_H

#include <QSharedPointer>

#include "abstractmethods.h"

class FactoryMethods {
public:
  FactoryMethods();

  static bool s_useTripe;

  static QSharedPointer<AbstractMethods>
  CreateMethods(AbstractSystem::System s);
};

#endif // FACTORYMETHODS_H
