{
  'targets': [
      {
          'target_name': 'libopensdk',
          'type': '<(opensdk_library)',
          
          'defines':[
       
          ],

          'include_dirs': [
            'src/',
          ],
          'sources': [
            'src/PluginManager.cpp',
            'src/PluginParam.cpp',
            'src/include/AgentManager.h',
            'src/include/PluginFactory.h',
            'src/include/PluginManager.h',
            'src/include/PluginParam.h',
            'src/include/PluginProtocol.h',
            'src/include/ProtocolAds.h',
            'src/include/ProtocolAnalytics.h',
            'src/include/ProtocolIAP.h',
            'src/include/ProtocolPush.h',
            'src/include/ProtocolShare.h',
            'src/include/ProtocolSocial.h',
            'src/include/ProtocolUser.h',            
          ],
          'conditions': [
              ['OS=="win"', {
                  'defines': [
                    
                  ],
                  'sources': [

                  ]
                }
             ],
             ['OS=="mac"', {
                  'defines': [

                  ],
                  'sources': [

                  ]
                }
             ],

             ['OS=="ios"', {
                  'defines': [

                  ],
                  'sources': [
                    'src/ios/AdsWrapper.h',
                    'src/ios/AdsWrapper.mm',
                    'src/ios/AgentManager.mm',
                    'src/ios/IAPWrapper.h',
                    'src/ios/IAPWrapper.mm',
                    'src/ios/InterfaceAds.h',
                    'src/ios/InterfaceAnalytics.h',
                    'src/ios/InterfaceIAP.h',
                    'src/ios/InterfaceShare.h',
                    'src/ios/InterfaceSocial.h',
                    'src/ios/InterfaceUser.h',
                    'src/ios/ParseUtils.h',
                    'src/ios/ParseUtils.m',
                    'src/ios/PluginFactory.mm',
                    'src/ios/PluginOCMacros.h',
                    'src/ios/PluginProtocol.mm',
                    'src/ios/PluginUtilsIOS.h',
                    'src/ios/PluginUtilsIOS.mm',
                    'src/ios/ProtocolAds.mm',
                    'src/ios/ProtocolAnalytics.mm',
                    'src/ios/ProtocolIAP.mm',
                    'src/ios/ProtocolShare.mm',
                    'src/ios/ProtocolSocial.mm',
                    'src/ios/ProtocolUser.mm',
                    'src/ios/ShareWrapper.h',
                    'src/ios/ShareWrapper.mm',
                    'src/ios/SocialWrapper.h',
                    'src/ios/SocialWrapper.mm',
                    'src/ios/UserWrapper.h',
                    'src/ios/UserWrapper.mm',
                    'src/ios/iOSIAPAgent.mm'
                  ]
                }
             ],
             ['OS=="android"', {
                  'defines': [

                  ],
                  'sources': [
                    'src/android/AdsObject.cpp',
                    'src/android/AdsObject.h',
                    'src/android/AgentManager.cpp',
                    'src/android/AnalyticsObject.cpp',
                    'src/android/AnalyticsObject.h',
                    'src/android/Android.mk',
                    'src/android/IAPObject.cpp',
                    'src/android/IAPObject.h',
                    'src/android/InitManager.cpp',
                    'src/android/InitManager.h',
                    'src/android/PluginFactory.cpp',
                    'src/android/PluginJavaData.h',
                    'src/android/PluginJniHelper.cpp',
                    'src/android/PluginJniHelper.h',
                    'src/android/PluginJniMacros.h',
                    'src/android/PluginProtocol.cpp',
                    'src/android/PluginUtils.cpp',
                    'src/android/PluginUtils.h',
                    'src/android/PushObject.cpp',
                    'src/android/PushObject.h',
                    'src/android/ShareObject.cpp',
                    'src/android/ShareObject.h',
                    'src/android/SocialObject.cpp',
                    'src/android/SocialObject.h',
                    'src/android/UserObject.cpp',
                    'src/android/UserObject.h',
                    'src/android/jni/Java_com_opensdk_framework_Ads.cpp',
                    'src/android/jni/Java_com_opensdk_framework_IAP.cpp',
                    'src/android/jni/Java_com_opensdk_framework_Push.cpp',
                    'src/android/jni/Java_com_opensdk_framework_Share.cpp',
                    'src/android/jni/Java_com_opensdk_framework_Social.cpp',
                    'src/android/jni/Java_com_opensdk_framework_User.cpp',
                    'src/android/jni/Java_com_opensdk_init_InitManager.cpp'
                  ]
                }
             ],
             ['OS=="linux"', {
                  'defines': [

                  ],
                  'sources': [

                  ]
                }
             ],
             ['OS=="winrt"', {
                  'defines': [

                  ],
                  'sources': [

                  ]
                }
             ],
             ['opensdk_library=="static_library"',{
                  'defines':['OS_STATIC'],
                }
             ],
             ['opensdk_library=="shared_library"',{
                  'defines':['OS_USRDLL'],                  
                }
             ],
         ],
         'configurations': {
            'Debug': {
                'defines': [ 'OS_USE_SCRIPT_LOG','OS_DEBUG'],
            }
         },
      }
  ]
}