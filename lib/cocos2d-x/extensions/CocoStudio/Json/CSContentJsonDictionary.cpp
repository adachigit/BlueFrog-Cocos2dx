/*
 * Copyright (c) 2012 Chukong Technologies, Inc.
 *
 * http://www.cocostudio.com
 * http://tools.cocoachina.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */
#include <iostream>
#include "CSContentJsonDictionary.h"

// add by weixin begin
#if (USE_JSON_LUA != 0)
#include "../../../scripting/lua/cocos2dx_support/CCLuaEngine.h"
extern "C" {
#include "lauxlib.h"
}
#endif
// add by weixin end

namespace cs {
    
// add by weixin begin
#if (USE_JSON_LUA == 0)
// add by weixin end
    CSJsonDictionary::CSJsonDictionary()
    {
        m_cValue.clear();
    }
    
    
    CSJsonDictionary::~CSJsonDictionary()
    {
        m_cValue.clear();
    }
    
    
    void CSJsonDictionary::initWithDescription(const char *pszDescription)
    {
        CSJson::Reader cReader;
        m_cValue.clear();
        if (pszDescription && *pszDescription)
        {
            std::string strValue = pszDescription;
            cReader.parse(strValue, m_cValue, false);
        }
    }
    
    
    void CSJsonDictionary::initWithValue(CSJson::Value& value)
    {
        m_cValue = value;
    }
    
    
    void CSJsonDictionary::insertItem(const char *pszKey, int nValue)
    {
        m_cValue[pszKey] = nValue;
    }
    
    
    void CSJsonDictionary::insertItem(const char *pszKey, double fValue)
    {
        m_cValue[pszKey] = fValue;
    }
    
    
    void CSJsonDictionary::insertItem(const char *pszKey, const char * pszValue)
    {
        m_cValue[pszKey] = pszValue;
    }
    
    void CSJsonDictionary::insertItem(const char *pszKey, bool bValue)
    {
        m_cValue[pszKey] = bValue;
    }
    
    void CSJsonDictionary::insertItem(const char *pszKey, CSJsonDictionary * subDictionary)
    {
        if (subDictionary)
            m_cValue[pszKey] = subDictionary->m_cValue;
    }
    
    
    bool CSJsonDictionary::deleteItem(const char *pszKey)
    {
        if(!m_cValue.isMember(pszKey))
            return false;
        
        m_cValue.removeMember(pszKey);
        
        return true;
    }
    
    
    void CSJsonDictionary::cleanUp()
    {
        m_cValue.clear();
    }
    
    
    bool CSJsonDictionary::isKeyValidate(const char *pszKey)
    {
        return m_cValue.isMember(pszKey);
    }
    
    
    int CSJsonDictionary::getItemIntValue(const char *pszKey, int nDefaultValue)
    {
        if (!isKeyValidate(pszKey, m_cValue) || !m_cValue[pszKey].isNumeric())
            return nDefaultValue;
        
        return m_cValue[pszKey].asInt();
    }
    
    
    double CSJsonDictionary::getItemFloatValue(const char *pszKey, double fDefaultValue)
    {
        if (!isKeyValidate(pszKey, m_cValue) || !m_cValue[pszKey].isNumeric())
            return fDefaultValue;
        
        return m_cValue[pszKey].asDouble();
    }
    
    
    const char * CSJsonDictionary::getItemStringValue(const char *pszKey)
    {
        if (!isKeyValidate(pszKey, m_cValue) || !m_cValue[pszKey].isString())
            return NULL;
        
        return m_cValue[pszKey].asCString();
    }
    
    bool CSJsonDictionary::getItemBoolvalue(const char *pszKey, bool bDefaultValue)
    {
        if (!isKeyValidate(pszKey, m_cValue) || !m_cValue[pszKey].isBool())
            return bDefaultValue;
        
        return m_cValue[pszKey].asBool();
    }
    
    
    CSJsonDictionary * CSJsonDictionary::getSubDictionary(const char *pszKey)
    {
        CSJsonDictionary * pNewDictionary;
        if (!isKeyValidate(pszKey, m_cValue) || (!m_cValue[pszKey].isArray() &&
                                                 !m_cValue[pszKey].isObject() &&
                                                 !m_cValue[pszKey].isConvertibleTo(CSJson::arrayValue) &&
                                                 !m_cValue[pszKey].isConvertibleTo(CSJson::objectValue)))
        {
            pNewDictionary = NULL;
        }
        else
        {
            pNewDictionary = new CSJsonDictionary();
            pNewDictionary->initWithValue(m_cValue[pszKey]);
        }
        return pNewDictionary;
    }
    
    
    std::string CSJsonDictionary::getDescription()
    {
        std::string strReturn = m_cValue.toStyledString();
        return strReturn;
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, int nValue)
    {
        CSJson::Value array;
        if(m_cValue.isMember(pszArrayKey))
        {
            if (!m_cValue[pszArrayKey].isArray() && !m_cValue[pszArrayKey].isConvertibleTo(CSJson::arrayValue))
                return false;
            
            array = m_cValue[pszArrayKey];
        }
        
        array.append(nValue);
        m_cValue[pszArrayKey] = array;
        
        return true;
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, double fValue)
    {
        CSJson::Value array;
        if(m_cValue.isMember(pszArrayKey))
        {
            if (!m_cValue[pszArrayKey].isArray() && !m_cValue[pszArrayKey].isConvertibleTo(CSJson::arrayValue))
                return false;
            
            array = m_cValue[pszArrayKey];
        }
        
        array.append(fValue);
        m_cValue[pszArrayKey] = array;
        
        return true;
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, const char * pszValue)
    {
        CSJson::Value array;
        if(m_cValue.isMember(pszArrayKey))
        {
            if (!m_cValue[pszArrayKey].isArray() && !m_cValue[pszArrayKey].isConvertibleTo(CSJson::arrayValue))
                return false;
            
            array = m_cValue[pszArrayKey];
        }
        
        array.append(pszValue);
        m_cValue[pszArrayKey] = array;
        
        return true;
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, CSJsonDictionary * subDictionary)
    {
        CSJson::Value array;
        if(m_cValue.isMember(pszArrayKey))
        {
            if (!m_cValue[pszArrayKey].isArray() && !m_cValue[pszArrayKey].isConvertibleTo(CSJson::arrayValue))
                return false;
            
            array = m_cValue[pszArrayKey];
        }
        
        array.append(subDictionary->m_cValue);
        m_cValue[pszArrayKey] = array;
        
        return true;
    }
    
    
    int CSJsonDictionary::getItemCount()
    {
        return m_cValue.size();
    }
    
    
    DicItemType CSJsonDictionary::getItemType(int nIndex)
    {
        return (DicItemType)m_cValue[nIndex].type();
    }
    
    
    DicItemType CSJsonDictionary::getItemType(const char *pszKey)
    {
        return (DicItemType)m_cValue[pszKey].type();
    }
    
    std::vector<std::string> CSJsonDictionary::getAllMemberNames()
    {
        return m_cValue.getMemberNames();
    }
    
    
    int CSJsonDictionary::getArrayItemCount(const char *pszArrayKey)
    {
        int nRet = 0;
        if (!isKeyValidate(pszArrayKey, m_cValue) ||
            (!m_cValue[pszArrayKey].isArray() && !m_cValue[pszArrayKey].isObject() &&
             !m_cValue[pszArrayKey].isConvertibleTo(CSJson::arrayValue) && !m_cValue[pszArrayKey].isConvertibleTo(CSJson::objectValue)))
        {
            nRet = 0;
        }
        else
        {
            CSJson::Value arrayValue = m_cValue[pszArrayKey];
            nRet = arrayValue.size();
        }
        
        return nRet;
    }
    
    
    int CSJsonDictionary::getIntValueFromArray(const char *pszArrayKey, int nIndex, int nDefaultValue)
    {
        int nRet = nDefaultValue;
        CSJson::Value * arrayValue = validateArrayItem(pszArrayKey, nIndex);
        if (arrayValue)
        {
            if ((*arrayValue)[nIndex].isNumeric())
                nRet = (*arrayValue)[nIndex].asInt();
        }
        
        return nRet;
    }
    
    
    double CSJsonDictionary::getFloatValueFromArray(const char *pszArrayKey, int nIndex, double fDefaultValue)
    {
        double fRet = fDefaultValue;
        CSJson::Value * arrayValue = validateArrayItem(pszArrayKey, nIndex);
        if (arrayValue)
        {
            if ((*arrayValue)[nIndex].isNumeric())
                fRet = (*arrayValue)[nIndex].asDouble();
        }
        
        return fRet;
    }
    
    bool CSJsonDictionary::getBoolValueFromArray(const char *pszArrayKey, int nIndex, bool bDefaultValue)
    {
        bool bRet = bDefaultValue;
        CSJson::Value * arrayValue = validateArrayItem(pszArrayKey, nIndex);
        if (arrayValue)
        {
            if ((*arrayValue)[nIndex].isNumeric())
                bRet = (*arrayValue)[nIndex].asBool();
        }
        
        return bRet;
    }
    
    
    const char * CSJsonDictionary::getStringValueFromArray(const char *pszArrayKey, int nIndex)
    {
        CSJson::Value * arrayValue = validateArrayItem(pszArrayKey, nIndex);
        if (arrayValue)
        {
            if ((*arrayValue)[nIndex].isString())
                return (*arrayValue)[nIndex].asCString();
        }
        
        return NULL;
    }
    
    
    CSJsonDictionary * CSJsonDictionary::getSubItemFromArray(const char *pszArrayKey, int nIndex)
    {
        CSJson::Value * arrayValue = validateArrayItem(pszArrayKey, nIndex);
        if (arrayValue)
        {
            if ((*arrayValue)[nIndex].isArray() || (*arrayValue)[nIndex].isObject())
            {
                CSJsonDictionary * pNewDictionary = new CSJsonDictionary();
                pNewDictionary->initWithValue((*arrayValue)[nIndex]);
                return pNewDictionary;
            }
        }
        
        return NULL;
    }
    
    
    DicItemType CSJsonDictionary::getItemTypeFromArray(const char *pszArrayKey, int nIndex)
    {
        CSJson::Value * arrayValue = validateArrayItem(pszArrayKey, nIndex);
        if (arrayValue)
            return (DicItemType)((*arrayValue)[nIndex].type());
        
        return (DicItemType)CSJson::nullValue;
    }
    
    
    inline bool CSJsonDictionary::isKeyValidate(const char *pszKey, CSJson::Value& root)
    {
        if (root.isNull() || !root.isMember(pszKey))
            return false;
        
        return true;
    }
    
    
    inline CSJson::Value * CSJsonDictionary::validateArrayItem(const char *pszArrayKey, int nIndex)
    {
        if (!isKeyValidate(pszArrayKey, m_cValue) && !m_cValue[pszArrayKey].isArray() && !m_cValue[pszArrayKey].isConvertibleTo(CSJson::arrayValue))
            return NULL;
        if (!m_cValue[pszArrayKey].isValidIndex(nIndex))
            return NULL;
        
        return &m_cValue[pszArrayKey];
    }
// add by weixin begin
#else
    static cocos2d::CCLuaEngine s_luaEngine;
    
    CSJsonDictionary::CSJsonDictionary(bool async)
    {
        if (!s_luaEngine.getLuaStack())
            s_luaEngine.init();
        
        if (async)
            m_ccLuaStack = s_luaEngine.getLuaStack();
        else
            m_ccLuaStack = cocos2d::CCLuaEngine::defaultEngine()->getLuaStack();
        
        m_isAsync = async;
        m_jsonLuaName.clear();
        m_luaItemIndex = 0;
    }
    
    
    CSJsonDictionary::~CSJsonDictionary()
    {
        cleanUp();
    }
    
    
    void CSJsonDictionary::initWithDescription(const char *pszDescription)
    {
        m_jsonLuaName = "json_export";
        m_luaItemIndex = 1;
        
        if (m_isAsync)
            m_luaState = lua_newthread(m_ccLuaStack->getLuaState());
        else
            m_luaState = m_ccLuaStack->getLuaState();
        
        lua_getglobal(m_luaState, m_jsonLuaName.c_str());
        if (lua_istable(m_luaState, -1))
        {
            lua_pushstring(m_luaState, "use");
            lua_gettable(m_luaState, -2);
            lua_call(m_luaState, 0, 0);
            lua_pop(m_luaState, 1);
        }
        else
        {
            m_ccLuaStack->executeScriptFile(pszDescription, m_isAsync);
        }
    }

    
    void CSJsonDictionary::initWithValue(CSJson::Value& value)
    {
        assert(0 && "not support in lua mode");
    }
    
    
    void CSJsonDictionary::initWithIndex(int itemIndex, lua_State* luaStack)
    {
        m_jsonLuaName = "json_export";
        m_luaItemIndex = itemIndex;
        m_luaState = luaStack;
    }
    
    
    void CSJsonDictionary::insertItem(const char *pszKey, int nValue)
    {
        assert(0 && "not support in lua mode");
    }
    
    
    void CSJsonDictionary::insertItem(const char *pszKey, double fValue)
    {
        assert(0 && "not support in lua mode");
    }
    
    
    void CSJsonDictionary::insertItem(const char *pszKey, const char * pszValue)
    {
        assert(0 && "not support in lua mode");
    }
    
    void CSJsonDictionary::insertItem(const char *pszKey, bool bValue)
    {
        assert(0 && "not support in lua mode");
    }
    
    void CSJsonDictionary::insertItem(const char *pszKey, CSJsonDictionary * subDictionary)
    {
        assert(0 && "not support in lua mode");
    }
    
    
    bool CSJsonDictionary::deleteItem(const char *pszKey)
    {
        assert(0 && "not support in lua mode");
        return false;
    }
    
    
    void CSJsonDictionary::cleanUp()
    {
        if (m_luaItemIndex == 1)
        {
            if (prepareLuaState("free"))
            {
                lua_call(m_luaState, 0, 0);
                lua_pop(m_luaState, 1);
            }
            if (m_isAsync)
            {
                int refKey = luaL_ref(m_luaState, LUA_REGISTRYINDEX);
                luaL_unref(m_luaState, LUA_REGISTRYINDEX, refKey);
            }
        }
        else if (m_luaItemIndex > 1)
        {
            if (prepareLuaState("removeSubItemIndex"))
            {
                lua_pushinteger(m_luaState, m_luaItemIndex);
                lua_call(m_luaState, 1, 0);
                lua_pop(m_luaState, 1);
            }
        }

        m_ccLuaStack = NULL;
        m_jsonLuaName.clear();
        m_luaItemIndex = 0;
    }
    
    
    bool CSJsonDictionary::isKeyValidate(const char *pszKey)
    {
        if (!prepareLuaState("isKeyValidate"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszKey);
        lua_call(m_luaState, 2, 1);
        bool bRet = false;
        if (lua_isboolean(m_luaState, -1))
            bRet = lua_toboolean(m_luaState, -1);
        
        lua_pop(m_luaState, 2);

        return bRet;
    }
    
    
    int CSJsonDictionary::getItemIntValue(const char *pszKey, int nDefaultValue)
    {
        if (!prepareLuaState("getItemValue"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszKey);
        lua_call(m_luaState, 2, 1);
        int bRet = nDefaultValue;
        if (lua_isnumber(m_luaState, -1))
            bRet = (int)lua_tointeger(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }

    
    double CSJsonDictionary::getItemFloatValue(const char *pszKey, double fDefaultValue)
    {
        if (!prepareLuaState("getItemValue"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszKey);
        lua_call(m_luaState, 2, 1);
        double bRet = fDefaultValue;
        if (lua_isnumber(m_luaState, -1))
            bRet = lua_tonumber(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    
    const char * CSJsonDictionary::getItemStringValue(const char *pszKey)
    {
        if (!prepareLuaState("getItemValue"))
            return NULL;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszKey);
        lua_call(m_luaState, 2, 1);
        const char* bRet = NULL;
        if (lua_isstring(m_luaState, -1))
            bRet = lua_tostring(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    bool CSJsonDictionary::getItemBoolvalue(const char *pszKey, bool bDefaultValue)
    {
        if (!prepareLuaState("getItemValue"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszKey);
        lua_call(m_luaState, 2, 1);
        bool bRet = bDefaultValue;
        if (lua_isboolean(m_luaState, -1))
            bRet = lua_toboolean(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    
    CSJsonDictionary * CSJsonDictionary::getSubDictionary(const char *pszKey)
    {
        if (!prepareLuaState("getSubItemIndex"))
            return NULL;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszKey);
        lua_call(m_luaState, 2, 1);
        int bRet = 0;
        if (lua_isnumber(m_luaState, -1))
            bRet = (int)lua_tointeger(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        if (bRet <= 0)
            return NULL;
        
        CSJsonDictionary * pNewDictionary = new CSJsonDictionary();
        pNewDictionary->initWithIndex(bRet, m_luaState);
        
        return pNewDictionary;
    }
    
    
    std::string CSJsonDictionary::getDescription()
    {
        assert(0 && "not support in lua mode");
        return "";
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, int nValue)
    {
        assert(0 && "not support in lua mode");
        return false;
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, double fValue)
    {
        assert(0 && "not support in lua mode");
        return false;
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, const char * pszValue)
    {
        assert(0 && "not support in lua mode");
        return false;
    }
    
    
    bool CSJsonDictionary::insertItemToArray(const char *pszArrayKey, CSJsonDictionary * subDictionary)
    {
        assert(0 && "not support in lua mode");
        return false;
    }
    
    
    int CSJsonDictionary::getItemCount()
    {
        assert(0 && "not support in lua mode");
        return 0;
    }
    
    
    DicItemType CSJsonDictionary::getItemType(int nIndex)
    {
        assert(0 && "not support in lua mode");
        return (DicItemType)0;
    }
    
    
    DicItemType CSJsonDictionary::getItemType(const char *pszKey)
    {
        assert(0 && "not support in lua mode");
        return (DicItemType)0;
    }
    
    std::vector<std::string> CSJsonDictionary::getAllMemberNames()
    {
        assert(0 && "not support in lua mode");
        return std::vector<std::string>();
    }
    
    
    int CSJsonDictionary::getArrayItemCount(const char *pszArrayKey)
    {
        if (!prepareLuaState("getSubArrayLength"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszArrayKey);
        lua_call(m_luaState, 2, 1);
        int bRet = 0;
        if (lua_isnumber(m_luaState, -1))
            bRet = (int)lua_tointeger(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    
    int CSJsonDictionary::getIntValueFromArray(const char *pszArrayKey, int nIndex, int nDefaultValue)
    {
        if (!prepareLuaState("getSubArrayItemValue"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszArrayKey);
        lua_pushinteger(m_luaState, nIndex+1);
        lua_call(m_luaState, 3, 1);
        int bRet = nDefaultValue;
        if (lua_isnumber(m_luaState, -1))
            bRet = (int)lua_tointeger(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    
    double CSJsonDictionary::getFloatValueFromArray(const char *pszArrayKey, int nIndex, double fDefaultValue)
    {
        if (!prepareLuaState("getSubArrayItemValue"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszArrayKey);
        lua_pushinteger(m_luaState, nIndex+1);
        lua_call(m_luaState, 3, 1);
        double bRet = fDefaultValue;
        if (lua_isnumber(m_luaState, -1))
            bRet = lua_tonumber(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    bool CSJsonDictionary::getBoolValueFromArray(const char *pszArrayKey, int nIndex, bool bDefaultValue)
    {
        if (!prepareLuaState("getSubArrayItemValue"))
            return false;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszArrayKey);
        lua_pushinteger(m_luaState, nIndex+1);
        lua_call(m_luaState, 3, 1);
        bool bRet = bDefaultValue;
        if (lua_isboolean(m_luaState, -1))
            bRet = lua_toboolean(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    
    const char * CSJsonDictionary::getStringValueFromArray(const char *pszArrayKey, int nIndex)
    {
        if (!prepareLuaState("getSubArrayItemValue"))
            return NULL;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszArrayKey);
        lua_pushinteger(m_luaState, nIndex+1);
        lua_call(m_luaState, 3, 1);
        const char* bRet = NULL;
        if (lua_isstring(m_luaState, -1))
            bRet = lua_tostring(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        return bRet;
    }
    
    
    CSJsonDictionary * CSJsonDictionary::getSubItemFromArray(const char *pszArrayKey, int nIndex)
    {
        if (!prepareLuaState("getSubArrayItemIndex"))
            return NULL;
        
        lua_pushinteger(m_luaState, m_luaItemIndex);
        lua_pushstring(m_luaState, pszArrayKey);
        lua_pushinteger(m_luaState, nIndex+1);
        lua_call(m_luaState, 3, 1);
        int bRet = 0;
        if (lua_isnumber(m_luaState, -1))
            bRet = (int)lua_tointeger(m_luaState, -1);
        
        lua_pop(m_luaState, 2);
        
        if (bRet <= 0)
            return NULL;
        
        CSJsonDictionary * pNewDictionary = new CSJsonDictionary();
        pNewDictionary->initWithIndex(bRet, m_luaState);
        
        return pNewDictionary;
    }
    
    
    DicItemType CSJsonDictionary::getItemTypeFromArray(const char *pszArrayKey, int nIndex)
    {
        assert(0 && "not support in lua mode");
        return (DicItemType)0;
    }
    
    
    inline bool CSJsonDictionary::isKeyValidate(const char *pszKey, CSJson::Value& root)
    {
        assert(0 && "not support in lua mode");
        return false;
    }
    
    
    inline CSJson::Value * CSJsonDictionary::validateArrayItem(const char *pszArrayKey, int nIndex)
    {
        assert(0 && "not support in lua mode");
        return NULL;
    }
    
    
    bool CSJsonDictionary::prepareLuaState(const char* pszKey)
    {
        if (m_luaItemIndex <= 0)
            return false;
        
        lua_getglobal(m_luaState, m_jsonLuaName.c_str());
        if (!lua_istable(m_luaState, -1))
        {
            lua_pop(m_luaState, 1);
            return false;
        }
        
        lua_pushstring(m_luaState, pszKey);
        lua_gettable(m_luaState, -2);
        if (!lua_isfunction(m_luaState, -1))
        {
            lua_pop(m_luaState, 2);
            return false;
        }
        
        return true;
    }
    
#endif
// add by weixin end
}
