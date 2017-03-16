#ifndef __OPENSDK_PLUGIN_PARAM_H__
#define __OPENSDK_PLUGIN_PARAM_H__

#include <map>
#include <string>

typedef std::map<std::string, std::string>   StringMap;

namespace opensdk {

class PluginProtocol;
class PluginParam
{
public:
    PluginParam();
	PluginParam(int nValue);
	PluginParam(float fValue);
	PluginParam(bool bValue);
	PluginParam(const char* strValue);
	PluginParam(StringMap strMapValue);

	typedef enum{
		kParamTypeNull = 0,
		kParamTypeInt,
		kParamTypeFloat,
		kParamTypeBool,
		kParamTypeString,
        kParamTypeStringMap,
		kParamTypeMap,
	} ParamType;

	inline ParamType getCurrentType() {
		return _type;
	}

	inline int getIntValue() {
		return _intValue;
	}

	inline float getFloatValue() {
		return _floatValue;
	}

	inline bool getBoolValue() {
		return _boolValue;
	}

	inline const char* getStringValue() {
		return _strValue.c_str();
	}

	inline std::map<std::string, PluginParam*> getMapValue() {
		return _mapValue;
	}

	inline StringMap getStrMapValue() {
        return _strMapValue;
    }

private:
	friend class PluginProtocol;
    PluginParam(std::map<std::string, PluginParam*> mapValue);

private:
	ParamType _type;

	int _intValue;
	float _floatValue;
	bool _boolValue;
	std::string _strValue;
	std::map<std::string, PluginParam*> _mapValue;
	StringMap _strMapValue;
};

} //namespace opensdk {

#endif /* __OPENSDK_PLUGIN_PARAM_H__ */
