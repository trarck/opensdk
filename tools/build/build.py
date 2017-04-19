#!/usr/bin/python
# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
'''
'''
import sys
import subprocess
import os
import json
import inspect
import traceback
from argparse import ArgumentParser
from build_android import AndroidBuilder
import utils

def main():
    parser = ArgumentParser(description="Generate prebuilt engine for Cocos Engine.")
    parser.add_argument("-j", "--jobs", dest="jobs", type=int,
                          help="how many thread for build")
    parser.add_argument("--proj-dir",
                            dest="proj_dir",
                            help="project dir .default is working dir")
                            
    parser.add_argument("--ap", dest="android_platform",
                       help="Specify the Android platform used for building Android apk.")
    parser.add_argument("--ndk-mode", dest="ndk_mode",
                       help="Set the compiling mode of ndk-build, should be debug|release|none, native code will not be compiled when the value is none. Default is same value with '-m'.")
    parser.add_argument("--app-abi", dest="app_abi",
                       help="Set the APP_ABI of ndk-build.Can be multi value separated with ':'. Sample : --app-aib armeabi:x86:mips. Default value is 'armeabi'.")
    parser.add_argument("--ndk-toolchain", dest="toolchain",
                       help="Specify the NDK_TOOLCHAIN of ndk-build.")
    parser.add_argument("--ndk-cppflags", dest="cppflags",
                       help="Specify the APP_CPPFLAGS of ndk-build.")
    parser.add_argument("--android-studio", dest="use_studio", action="store_true",
                       help="Use the Android Studio project for Android platform.")
    parser.add_argument("--no-apk", dest="no_apk", action="store_true",
                       help="Not generate the apk file.")
    parser.add_argument("--ndk-module-path", dest="ndk_module_path",
                       help="ndk module path separated with ':'")
    (args, unknown) = parser.parse_known_args()

    using_studio = False
    
    if args.use_studio:
        using_studio=args.use_studio
    
    project_dir= os.path.abspath(os.getcwd())
    if args.proj_dir:
        project_dir=args.proj_dir
    
    builder = AndroidBuilder(True, project_dir,True, None, using_studio)
               
    jobs=4
    if args.jobs:
        jobs=args.jobs
    
    ndk_build_param = [
        "-j%s" % jobs
    ]
    
    if args.ndk_module_path:
        builder.ndk_module_paths=args.ndk_module_path.split(":")
                                 
    if args.app_abi:
        abi_param = "APP_ABI=\"%s\"" % " ".join(args.app_abi.split(":"))
        ndk_build_param.append(abi_param)

    if args.toolchain:
        toolchain_param = "NDK_TOOLCHAIN=%s" % args.toolchain
        ndk_build_param.append(toolchain_param) 
    builder.do_ndk_build(ndk_build_param, args.ndk_mode, None)
    '''    
    modify_mk = False
    if using_studio:
        app_mk = os.path.join(project_dir, "app/jni/Application.mk")
    else:
        app_mk = os.path.join(project_dir, "jni/Application.mk")
    mk_content = None
    if args.cppflags and os.path.exists(app_mk):
        # record the content of Application.mk
        f = open(app_mk)
        mk_content = f.read()
        f.close()

        # Add cpp flags
        f = open(app_mk, "a")
        f.write("\nAPP_CPPFLAGS += %s" % args.cppflags)
        f.close()
        modify_mk = True

    try:
        builder.do_ndk_build(ndk_build_param, args.ndk_mode, None)
    except Exception as e:
        if e.__class__.__name__ == 'YHError':
            raise e
        else:
            raise utils.YHError("build error :%s" % e,utils.YHError.ERROR_BUILD_FAILED)
    finally:
        # roll-back the Application.mk
        if modify_mk:
            f = open(app_mk, "w")
            f.write(mk_content)
            f.close()
    '''
# -------------- main --------------
if __name__ == '__main__':
    try:
        main()
    except Exception as e:
        traceback.print_exc()
        sys.exit(1)