#include "PluginParam.h"

namespace opensdk {

PluginParam::PluginParam()
{
    _type = kParamTypeNull;
}

PluginParam::PluginParam(int nValue)
: _intValue(nValue)
{
	_type = kParamTypeInt;
}

PluginParam::PluginParam(float fValue)
: _floatValue(fValue)
{
	_type = kParamTypeFloat;
}

PluginParam::PluginParam(bool bValue)
: _boolValue(bValue)
{
	_type = kParamTypeBool;
}

PluginParam::PluginParam(const char* strValue)
: _strValue(strValue)
{
	_type = kParamTypeString;
}

PluginParam::PluginParam(std::map<std::string, PluginParam*> mapValue)
: _mapValue(mapValue)
{
	_type = kParamTypeMap;
}

PluginParam::PluginParam(StringMap strMapValue)
: _strMapValue(strMapValue)
{
    _type = kParamTypeStringMap;
}

} //namespace cocos2d { namespace plugin {
