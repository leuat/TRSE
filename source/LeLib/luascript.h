#ifndef LUASCRIPT_H
#define LUASCRIPT_H

#include <QString>
#include <QVector>
#include <QVector3D>
#include <QDebug>

extern "C" {
 #include "lua.h"
 #include "lauxlib.h"
 #include "lualib.h"
}


class LuaScript {
public:
    LuaScript(const QString& filename);
    ~LuaScript();
    void printError(const QString& variableName, const QString& reason);
    QVector<int> getIntVector(const QString& name);
    QVector<QString> getTableKeys(const QString& name);

    QString m_error = "";

    inline void clean() {
      int n = lua_gettop(L);
      lua_pop(L, n);
    }

    template<typename T>
    T get(const QString& variableName) {
      if(!L) {
        printError(variableName, "Script is not loaded");
        return lua_getdefault<T>();
      }

      T result;
      if(lua_gettostack(variableName)) { // variable succesfully on top of stack
        result = lua_get<T>(variableName);
      } else {
        result = lua_getdefault<T>();
      }


      clean();
      return result;
    }


    QVector3D getVec(const QString& variableName) {

      QVector3D result = QVector3D(0,0,0);
      if(lua_gettostack(variableName)) {
        result.setX(get<float>(variableName+".x"));
        result.setY(get<float>(variableName+".y"));
        result.setZ(get<float>(variableName+".z"));
      }

      clean();
      return result;
    }


    bool lua_gettostack(const QString& variableName) {
      level = 0;
      QString var = "";
        for(unsigned int i = 0; i < variableName.size(); i++) {
          if(variableName.at(i) == '.') {
            if(level == 0) {
              lua_getglobal(L, var.toStdString().c_str());
            } else {
              lua_getfield(L, -1, var.toStdString().c_str());
            }

            if(lua_isnil(L, -1)) {
              printError(variableName, var + " is not defined");
              return false;
            } else {
              var = "";
              level++;
            }
          } else {
            var += variableName.at(i);
          }
        }
        if(level == 0) {
          lua_getglobal(L, var.toStdString().c_str());
        } else {
          lua_getfield(L, -1, var.toStdString().c_str());
        }
        if(lua_isnil(L, -1)) {
            printError(variableName, var + " is not defined");
            return false;
        }

        return true;
    }

    // Generic get
    template<typename T>
    T lua_get(const QString& variableName) {
      return 0;
    }

    template<typename T>
    T lua_getdefault() {
      return 0;
    }

    lua_State* L;
    QString filename;
    int level;
};

 // Specializations

template <>
inline bool LuaScript::lua_get<bool>(const QString& variableName) {
    return (bool)lua_toboolean(L, -1);
}

template <>
inline float LuaScript::lua_get<float>(const QString& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (float)lua_tonumber(L, -1);
}

template <>
inline int LuaScript::lua_get<int>(const QString& variableName) {
    if(!lua_isnumber(L, -1)) {
      printError(variableName, "Not a number");
    }
    return (int)lua_tonumber(L, -1);
}

template <>
inline QString LuaScript::lua_get<QString>(const QString& variableName) {
    QString s = "null";
    if(lua_isstring(L, -1)) {
      s = QString(lua_tostring(L, -1));
    } else {
      printError(variableName, "Not a string");
    }
    return s;
}

template<>
inline QString LuaScript::lua_getdefault<QString>() {
  return "null";
}

#endif
