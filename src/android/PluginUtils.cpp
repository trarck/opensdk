#include "PluginUtils.h"
#include <android/log.h>
#include <map>

#define MAX_LOG_LEN			256

namespace opensdk {

#define JAVAVM    opensdk::PluginJniHelper::getJavaVM()

void PluginUtils::initPluginWrapper(android_app* app)
{
    PluginJniMethodInfo t;
    if (! PluginJniHelper::getStaticMethodInfo(t
        , "com/opensdk/framework/PluginWrapper"
        , "initFromNativeActivity"
        , "(Landroid/app/Activity;)V"))
    {
        outputLog("PluginUtils", "Failed to init context of plugin");
        return;
    }

    t.env->CallStaticVoidMethod(t.classID, t.methodID, app->activity->clazz);
    t.env->DeleteLocalRef(t.classID);
}

jobject PluginUtils::createJavaMapObject(std::map<std::string, std::string>* paramMap)
{
    JNIEnv* env = getEnv();
	jclass class_Hashtable = env->FindClass("java/util/Hashtable");
	jmethodID construct_method = env->GetMethodID( class_Hashtable, "<init>","()V");
	jobject obj_Map = env->NewObject( class_Hashtable, construct_method, "");
	if (paramMap != NULL)
	{
		jmethodID add_method= env->GetMethodID( class_Hashtable,"put","(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
		for (std::map<std::string, std::string>::const_iterator it = paramMap->begin(); it != paramMap->end(); ++it)
		{
            jstring first = env->NewStringUTF(it->first.c_str());
            jstring second = env->NewStringUTF(it->second.c_str());
			env->CallObjectMethod(obj_Map, add_method, first, second);
            env->DeleteLocalRef(first);
            env->DeleteLocalRef(second);
		}
	}
    env->DeleteLocalRef(class_Hashtable);
    return obj_Map;
}
    
jobject PluginUtils::createJavaListObject(std::list<std::string>* paramList)
{
    
    JNIEnv* env = getEnv();
    jclass class_ArrayList = env->FindClass("java/util/ArrayList");
    jmethodID construct_method = env->GetMethodID( class_ArrayList, "<init>","()V");
    jobject obj_List = env->NewObject( class_ArrayList, construct_method, "");
    if (paramList != NULL)
    {
        jmethodID add_method= env->GetMethodID( class_ArrayList,"add","(Ljava/lang/Object;)Z");
        for (std::list<std::string>::const_iterator it = paramList->begin(); it != paramList->end(); ++it)
        {
            jstring value = env->NewStringUTF(it->c_str());
            env->CallObjectMethod(obj_List, add_method, value);
            env->DeleteLocalRef(value);
        }
    }
    env->DeleteLocalRef(class_ArrayList);
    return obj_List;
}

void PluginUtils::initJavaPlugin(PluginProtocol* pPlugin, jobject jObj, const char* className)
{
	opensdk::PluginJavaData* pUserData = new opensdk::PluginJavaData();
	pUserData->jobj = PluginUtils::getEnv()->NewGlobalRef(jObj);
	pUserData->jclassName = className;
	opensdk::PluginUtils::setPluginJavaData(pPlugin, pUserData);
}

JNIEnv* PluginUtils::getEnv()
{
    bool bRet = false;
    JNIEnv* env = NULL;
    do 
    {
        if (JAVAVM->GetEnv((void**)&env, JNI_VERSION_1_4) != JNI_OK)
        {
        	outputLog("PluginUtils", "Failed to get the environment using GetEnv()");
            break;
        }

        if (JAVAVM->AttachCurrentThread(&env, 0) < 0)
        {
            outputLog("PluginUtils", "Failed to get the environment using AttachCurrentThread()");
            break;
        }

        bRet = true;
    } while (0);        

    if (!bRet) {
    	env = NULL; 
    }

    return env;
}

std::map<PluginProtocol*, PluginJavaData*> s_PluginObjMap;
std::map<std::string, PluginProtocol*> s_JObjPluginMap;

typedef std::map<PluginProtocol*, PluginJavaData*>::iterator ObjMapIter;
typedef std::map<std::string, PluginProtocol*>::iterator JObjPluginMapIter;

PluginJavaData* PluginUtils::getPluginJavaData(PluginProtocol* pKeyObj)
{
    PluginJavaData* ret = NULL;
    ObjMapIter it = s_PluginObjMap.find(pKeyObj);
    if (it != s_PluginObjMap.end()) {
        ret = it->second;
    }

    return ret;
}

PluginProtocol* PluginUtils::getPluginPtr(std::string className)
{
	PluginProtocol* ret = NULL;
	JObjPluginMapIter it = s_JObjPluginMap.find(className);
	if (it != s_JObjPluginMap.end()) {
		ret = it->second;
	}

	return ret;
}

void PluginUtils::setPluginJavaData(PluginProtocol* pKeyObj, PluginJavaData* pData)
{
    erasePluginJavaData(pKeyObj);
    s_PluginObjMap.insert(std::pair<PluginProtocol*, PluginJavaData*>(pKeyObj, pData));
    s_JObjPluginMap.insert(std::pair<std::string, PluginProtocol*>(pData->jclassName, pKeyObj));
}

void PluginUtils::erasePluginJavaData(PluginProtocol* pKeyObj)
{
    ObjMapIter it = s_PluginObjMap.find(pKeyObj);
    if (it != s_PluginObjMap.end()) {
        PluginJavaData* pData = it->second;
        if (pData != NULL)
        {
            jobject jobj = pData->jobj;

            JObjPluginMapIter pluginIt = s_JObjPluginMap.find(pData->jclassName);
            if (pluginIt != s_JObjPluginMap.end())
            {
            	s_JObjPluginMap.erase(pluginIt);
            }

            JNIEnv* pEnv = getEnv();
            outputLog("PluginUtils", "Delete global reference.");
            pEnv->DeleteGlobalRef(jobj);
            delete pData;
        }
        s_PluginObjMap.erase(it);
    }
}

void PluginUtils::outputLog(const char* logTag, const char* pFormat, ...)
{
	char buf[MAX_LOG_LEN + 1];

	va_list args;
	va_start(args, pFormat);
	vsnprintf(buf, MAX_LOG_LEN, pFormat, args);
	va_end(args);

	__android_log_print(ANDROID_LOG_DEBUG, logTag, "%s", buf);
}

jobject PluginUtils::getJObjFromParam(PluginParam* param)
{
	if (NULL == param)
	{
		return NULL;
	}

	jobject obj = NULL;
	PluginJniMethodInfo t;
	JNIEnv* env = PluginUtils::getEnv();

	switch(param->getCurrentType())
	{
	case PluginParam::kParamTypeInt:
		if (PluginJniHelper::getStaticMethodInfo(t, "java/lang/Integer", "valueOf", "(I)Ljava/lang/Integer;"))
		{
			obj = t.env->CallStaticObjectMethod(t.classID, t.methodID, param->getIntValue());
			t.env->DeleteLocalRef(t.classID);
		}
		break;
	case PluginParam::kParamTypeFloat:
		if (PluginJniHelper::getStaticMethodInfo(t, "java/lang/Float", "valueOf", "(F)Ljava/lang/Float;"))
		{
			obj = t.env->CallStaticObjectMethod(t.classID, t.methodID, param->getFloatValue());
			t.env->DeleteLocalRef(t.classID);
		}
		break;
	case PluginParam::kParamTypeBool:
		if (PluginJniHelper::getStaticMethodInfo(t, "java/lang/Boolean", "valueOf", "(Z)Ljava/lang/Boolean;"))
		{
			obj = t.env->CallStaticObjectMethod(t.classID, t.methodID, param->getBoolValue());
			t.env->DeleteLocalRef(t.classID);
		}
		break;
	case PluginParam::kParamTypeString:
		obj = env->NewStringUTF(param->getStringValue());
		break;
	case PluginParam::kParamTypeStringMap:
	    {
	        jclass cls = env->FindClass("org/json/JSONObject");
            jmethodID mid = env->GetMethodID(cls,"<init>","()V");
            obj = env->NewObject(cls,mid);
            env->DeleteLocalRef(cls);
            std::map<std::string, std::string>::iterator it;
            std::map<std::string, std::string> mapParam = param->getStrMapValue();
            for (it = mapParam.begin(); it != mapParam.end(); it++)
            {
                PluginJniMethodInfo tInfo;
                if (PluginJniHelper::getMethodInfo(tInfo, "org/json/JSONObject", "put", "(Ljava/lang/String;Ljava/lang/Object;)Lorg/json/JSONObject;"))
                {
                    jstring strKey = tInfo.env->NewStringUTF(it->first.c_str());
                    jstring strValue = tInfo.env->NewStringUTF(it->second.c_str());

                    tInfo.env->CallObjectMethod(obj, tInfo.methodID, strKey, strValue);
                    tInfo.env->DeleteLocalRef(tInfo.classID);

                    tInfo.env->DeleteLocalRef(strKey);
                    tInfo.env->DeleteLocalRef(strValue);
                }
            }
        }
	    break;
	case PluginParam::kParamTypeMap:
		{
			jclass cls = env->FindClass("org/json/JSONObject");
			jmethodID mid = env->GetMethodID(cls,"<init>","()V");
			obj = env->NewObject(cls,mid);
            env->DeleteLocalRef(cls);
			std::map<std::string, PluginParam*>::iterator it;
			std::map<std::string, PluginParam*> mapParam = param->getMapValue();
			for (it = mapParam.begin(); it != mapParam.end(); it++)
			{
				PluginJniMethodInfo tInfo;
				if (PluginJniHelper::getMethodInfo(tInfo, "org/json/JSONObject", "put", "(Ljava/lang/String;Ljava/lang/Object;)Lorg/json/JSONObject;"))
				{
					jstring strKey = tInfo.env->NewStringUTF(it->first.c_str());
					jobject objValue = PluginUtils::getJObjFromParam(it->second);

					tInfo.env->CallObjectMethod(obj, tInfo.methodID, strKey, objValue);
					tInfo.env->DeleteLocalRef(tInfo.classID);

					tInfo.env->DeleteLocalRef(strKey);
					PluginUtils::getEnv()->DeleteLocalRef(objValue);
				}
			}
		}
		break;
	default:
		break;
	}

	return obj;
}
    
void PluginUtils::callJavaFunctionWithName_map(PluginProtocol* thiz, const char* funcName, std::map<std::string, std::string>* paramMap)
{
    return_if_fails(funcName != NULL && strlen(funcName) > 0);
    return_if_fails(paramMap != NULL);
    
    PluginJavaData* pData = PluginUtils::getPluginJavaData(thiz);
    
    PluginJniMethodInfo t;

    if (PluginJniHelper::getMethodInfo(t
                                       , pData->jclassName.c_str()
                                       , funcName
                                       , "(Ljava/util/Hashtable;)V"))
    {
        jobject obj_Map = PluginUtils::createJavaMapObject(paramMap);
        t.env->CallVoidMethod(pData->jobj, t.methodID, obj_Map);
        t.env->DeleteLocalRef(obj_Map);
        t.env->DeleteLocalRef(t.classID);
    }
    
}

void PluginUtils::callJavaFunctionWithName_string_map(PluginProtocol* thiz, const char* funcName, const char* param, std::map<std::string, std::string>* paramMap)
{
    return_if_fails(funcName != NULL && strlen(funcName) > 0);
    return_if_fails(param != NULL);
    
    PluginJavaData* pData = PluginUtils::getPluginJavaData(thiz);
    
    PluginJniMethodInfo t;
    if (NULL == paramMap)
    {
        if (PluginJniHelper::getMethodInfo(t
                                           , pData->jclassName.c_str()
                                           , funcName
                                           , "(Ljava/lang/String;)V"))
        {
            jstring jparam = t.env->NewStringUTF(param);
            t.env->CallVoidMethod(pData->jobj, t.methodID, jparam);
            t.env->DeleteLocalRef(jparam);
            t.env->DeleteLocalRef(t.classID);
        }
    }
    else
    {
        if (PluginJniHelper::getMethodInfo(t
                                           , pData->jclassName.c_str()
                                           , funcName
                                           , "(Ljava/lang/String;Ljava/util/Hashtable;)V"))
        {
            jstring jparam = t.env->NewStringUTF(param);
            jobject obj_Map = PluginUtils::createJavaMapObject(paramMap);
            t.env->CallVoidMethod(pData->jobj, t.methodID, jparam, obj_Map);
            t.env->DeleteLocalRef(jparam);
            t.env->DeleteLocalRef(obj_Map);
            t.env->DeleteLocalRef(t.classID);
        }
    }
}
    
void PluginUtils::callJavaFunctionWithName_string_string(PluginProtocol* thiz, const char* funcName,const char* param1, const char* param2)
{
    return_if_fails(param1 != NULL && strlen(param1) > 0);
    return_if_fails(param2 != NULL && strlen(param2) > 0);
    
    PluginJavaData* pData = PluginUtils::getPluginJavaData(thiz);
    PluginJniMethodInfo t;
    if (PluginJniHelper::getMethodInfo(t
                                       , pData->jclassName.c_str()
                                       , funcName
                                       , "(Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jparam1 = t.env->NewStringUTF(param1);
        jstring jparam2 = t.env->NewStringUTF(param2);
        
        t.env->CallVoidMethod(pData->jobj, t.methodID, jparam1, jparam2);
        t.env->DeleteLocalRef(jparam1);
        t.env->DeleteLocalRef(jparam2);
        t.env->DeleteLocalRef(t.classID);
    }
}

}// namespace opensdk {
