package com.opensdk.init;

import java.util.ArrayList;
import java.util.List;

public class SpecialInitHelper {
	private static SpecialInitHelper _instance;
	
	public static SpecialInitHelper getInstance() {
		if (_instance == null) {
			_instance = new SpecialInitHelper();
		}
		return _instance;
	}
	
	/**
	 * 取得初始化类的信息
	 * class@staticMethod.method.method
	 * TODO support parameter
	 * @param descStr
	 * @return
	 */
	public List<SpecialInitInfo> getSpecialInitData(String descStr){
		List<SpecialInitInfo> infos= new ArrayList<SpecialInitInfo>();

		
		String [] items=descStr.split(",");
		String it=null;
		String cls=null;
		SpecialInitInfo info=null;
		List<String> instanceMethods=null;
		
		int pos=-1;
		for(int i=0;i<items.length;++i){
			
			it=items[i];
			
			pos=it.indexOf('@');
			
			//pos must not in first
			if(pos>0){
				//get class name
				cls=it.substring(0, pos).trim();
				
				//get method 
				String[] methods=it.substring(pos+1).split("\\.");
				
				//first must be static method
				if(methods.length>0){
					info=new SpecialInitInfo();
					info.setInitClass(cls);
					info.setStaticMethod(methods[0]);
					
					//get instanceMethods
					if(methods.length>1){
						instanceMethods=new ArrayList<String>();
						
						for(int j=1;j<methods.length;++j){
							instanceMethods.add(methods[j]);
						}
					}
					
					infos.add(info);
				}
			}
		}
		
		return infos;
	}
	
	
	
}
