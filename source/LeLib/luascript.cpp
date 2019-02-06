#include "luascript.h"


LuaScript::LuaScript(const QString& filename) {
    L = luaL_newstate();

    int load = luaL_loadfile(L, filename.toStdString().c_str());
    if (load==0) {
        int err = lua_pcall(L, 0, 0, 0);
        qDebug() << "Error: " << err;
        if (err!=0) {
            L = nullptr;

            qDebug() <<"Error in file ("<<filename<<") : "; ;
        }
    }
    else {
        m_error = "Error in " +filename + " with error code " + QString::number(load) + " :";
        if (load==LUA_ERRSYNTAX)
            m_error +="\nSyntax error";
        L = nullptr;
    }


/*    if (luaL_loadfile(L, filename.toStdString().c_str()) || lua_pcall(L, 0, 0, 0)) {
          //  std::cout<<"Error: script not loaded ("<<filename<<")"<<std::endl;
            L = 0;
        }*/
    qDebug() << "Here: " << L;
    if(L) luaL_openlibs(L);
}

LuaScript::~LuaScript() {
    if(L) lua_close(L);
}

void LuaScript::printError(const QString& variableName, const QString& reason) {
//    qDebug() <<"Error: can't get ["<<variableName<<"]. "<<reason;
    m_error = "Error: can't get ["+ variableName+"]. "+reason;
}

QVector<int> LuaScript::getIntVector(const QString& name) {
    QVector<int> v;
    lua_gettostack(name.toStdString().c_str());
    if(lua_isnil(L, -1)) { // array is not found
        return QVector<int>();
    }
    lua_pushnil(L);
    while(lua_next(L, -2)) {
        v.push_back((int)lua_tonumber(L, -1));
        lua_pop(L, 1);
    }
    clean();
    return v;
}

QVector<QString> LuaScript::getTableKeys(const QString& name) {
    QString code =
        "function getKeys(name) "
        "s = \"\""
        "for k, v in pairs(_G[name]) do "
        "    s = s..k..\",\" "
        "    end "
        "return s "
        "end"; // function for getting table keys
    luaL_loadstring(L,
        code.toStdString().c_str()); // execute code
    lua_pcall(L,0,0,0);
    lua_getglobal(L, "getKeys"); // get function
    lua_pushstring(L, name.toStdString().c_str());
    lua_pcall(L, 1 , 1, 0); // execute function
    QString test = lua_tostring(L, -1);
    QVector<QString> strings;
    QString temp = "";
    qDebug() <<"TEMP:"<<test;
    for(unsigned int i = 0; i < test.size(); i++) {
        if(test.at(i) != ',') {
            temp += test.at(i);
        } else {
            strings.push_back(temp);
            temp= "";
        }
    }
    clean();
    return strings;
}
