#include "PushObject.h"
#include "PluginJniHelper.h"
#include <android/log.h>
#include "PluginUtils.h"
#include "PluginJavaData.h"

namespace opensdk {
	
std::vector<PushActionResult> PushObject::_actionResultList;

PushObject::PushObject()
:_listener(NULL)
, _pluginName("")
{
	
}

PushObject::~PushObject()
{
	
}

void PushObject::setPluginName(const char* name)
{
	_pluginName = name;
}

const char* PushObject::getPluginName()
{
	return _pluginName.c_str();
}
    
int PushObject::getPluginType()
{
    return kPluginPush;
}

void PushObject::startPush()
{
    PluginUtils::callJavaFunctionWithName(this,"startPush");
}

void PushObject::closePush()
{
    PluginUtils::callJavaFunctionWithName(this,"closePush");
}

void PushObject::setAlias(const std::string& alias)
{
    PluginParam paramAlias(alias.c_str());
    
    callFuncWithParam("setAlias", &paramAlias,NULL);
}

void PushObject::delAlias(const std::string& alias)
{
    PluginParam paramAlias(alias.c_str());
    
    callFuncWithParam("delAlias", &paramAlias,NULL);
}

void PushObject::setTags(std::list<std::string>& tags)
{
    PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    
    if (NULL == pData) {
        PluginUtils::outputLog("PushObject", "Can't find java data for plugin : %s", this->getPluginName());
        return;
    }
    
    PluginJniMethodInfo t;

 
    if (PluginJniHelper::getMethodInfo(t
                                       , pData->jclassName.c_str()
                                       , "setTags"
                                       , "(Ljava/util/ArrayList;)V"))
    {
        jobject obj_List = PluginUtils::createJavaListObject(&tags);
        t.env->CallVoidMethod(pData->jobj, t.methodID, obj_List);
        t.env->DeleteLocalRef(obj_List);
        t.env->DeleteLocalRef(t.classID);
    }
}

void PushObject::delTags(std::list<std::string>& tags)
{
    PluginJavaData* pData = PluginUtils::getPluginJavaData(this);
    
    if (NULL == pData) {
        PluginUtils::outputLog("PushObject", "Can't find java data for plugin : %s", this->getPluginName());
        return;
    }
    
    PluginJniMethodInfo t;
    
    
    if (PluginJniHelper::getMethodInfo(t
                                       , pData->jclassName.c_str()
                                       , "delTags"
                                       , "(Ljava/util/ArrayList;)V"))
    {
        jobject obj_List = PluginUtils::createJavaListObject(&tags);
        t.env->CallVoidMethod(pData->jobj, t.methodID, obj_List);
        t.env->DeleteLocalRef(obj_List);
        t.env->DeleteLocalRef(t.classID);
    }
}

void PushObject::setActionListener(PushActionListener* listener)
{
	_listener=listener;
    popActionResult();
}

PushActionListener* PushObject::getActionListener()
{
	return _listener;
}
    
void PushObject::popActionResult()
{
    for(std::vector<PushActionResult>::iterator iter=_actionResultList.begin();iter!=_actionResultList.end();){
        
        PushObject* pushObject = dynamic_cast<PushObject*>(PluginUtils::getPluginPtr(iter->className));
        if(pushObject){
            PushActionListener* listener = pushObject->getActionListener();
            if(listener){
                listener->onActionResult(pushObject,iter->resultCode, iter->msg.c_str());
                
                //remove from record
                iter=_actionResultList.erase(iter);
                continue;
            }
        }
        
        ++iter;
    }
}

void PushObject::pushActionResult(const PushActionResult& actionResult)
{
    _actionResultList.push_back(actionResult);
}

} // namespace opensdk

