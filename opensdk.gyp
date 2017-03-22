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

                  ]
                }
             ],
             ['OS=="android"', {
                  'defines': [

                  ],
                  'sources': [
                    
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