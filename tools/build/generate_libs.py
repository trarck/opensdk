#!/usr/bin/python
#-*- coding: UTF-8 -*-

import os
import sys
import shutil
import json
import utils
import generate_prebuilt_mk

from argparse import ArgumentParser
from build_android import AndroidBuilder

class LibsGenerator(object):
    CFG_FILE = 'configs/gen_libs_config.json'

    KEY_LIBS_OUTPUT = 'libs_output_dir'
    KEY_XCODE_PROJS_INFO = 'xcode_projs_info'
    KEY_VS_PROJS_INFO = 'vs_projs_info'
    KEY_SUPPORT_VS_VERSIONS = 'support_vs_versions'
    KEY_ANDROID_MKS = "android_mks"
    CHECK_KEYS = [
        KEY_LIBS_OUTPUT,
        KEY_XCODE_PROJS_INFO,
        KEY_VS_PROJS_INFO,
        KEY_SUPPORT_VS_VERSIONS,
        KEY_ANDROID_MKS
    ]

    KEY_XCODE_TARGETS = 'targets'
    KEY_VS_BUILD_TARGETS = 'build_targets'
    KEY_VS_RENAME_TARGETS = 'rename_targets'

    def parse_args(self, argv):
        """Custom and check param list.
        """
        parser = ArgumentParser(prog="gen libs",description="")
        parser.add_argument('-c', dest='clean', action="store_true",
                            help='Remove the prebuilt folder at first.')
        parser.add_argument('-w', dest='work_path', help="Specify the work path.")
        parser.add_argument('-p', dest='platform', action="append", choices=['ios', 'mac', 'android', 'win32'],
                            help="Specify the target platform. Can specify multi platform by using '-p' multi times. Default generate all available platforms.")
        parser.add_argument('-m', "--mode", dest='compile_mode', default='release', choices=['debug', 'release'],
                            help="Generate libs for debug or release. Default is release.")
        parser.add_argument('--dis-strip', dest='disable_strip', action="store_true",
                            help="Disable the strip of the generated libs.")
        group = parser.add_argument_group("Windows Options")
        group.add_argument('--vs', dest='vs_version', type=int, default=None,
                           help="Specify the Visual Studio version,  such as 2013. Default find available version automatically.")
        group = parser.add_argument_group("Android Options")
        group.add_argument("--app-abi", dest="app_abi",
                            help="Set the APP_ABI of ndk-build. Can be multi value separated with ':'. Sample : --app-abi armeabi:x86:mips. Default value is 'armeabi'.")

        (args, unknown) = parser.parse_known_args(argv)
        print args
        self.init(args)

        return args

    def run(self, argv):
        self.parse_args(argv)
        self.compile()

    def init(self, args):
        if getattr(sys, 'frozen', None):
            self.cur_dir = os.path.realpath(os.path.dirname(sys.executable))
            self.default_work_path = os.path.join(self.cur_dir, os.pardir, os.pardir)
        else:
            self.cur_dir = os.path.realpath(os.path.dirname(__file__))
            self.default_work_path = os.path.join(self.cur_dir, os.pardir, os.pardir)
        self.default_work_path = os.path.normpath(self.default_work_path)

        if args.work_path is None:
            self.repo_x = self.default_work_path
        else:
            work_path = os.path.expanduser(args.work_path)
            if os.path.isabs(work_path):
                self.repo_x = os.path.normpath(work_path)
            else:
                self.repo_x = os.path.normpath(os.path.abspath(work_path))

        if not os.path.isdir(self.repo_x):
            raise utils.YHError("%s is not a valid path." % self.repo_x,
                                utils.YHError.ERROR_WRONG_ARGS)

        self.cfg_file_path = os.path.join(self.cur_dir, LibsGenerator.CFG_FILE)
        self.parse_config()

        # arguments check and set
        self.clean = args.clean
        self.mode = args.compile_mode
        self._verbose = True

        if args.platform is None:
            self.build_ios = True
            self.build_mac = True
            self.build_win = True
            self.build_android = True
        else:
            self.build_ios = False
            self.build_mac = False
            self.build_win = False
            self.build_android = False
            if 'win32' in args.platform:
                self.build_win = True
            if 'ios' in args.platform:
                self.build_ios = True
            if 'mac' in args.platform:
                self.build_mac = True
            if 'android' in args.platform:
                self.build_android = True

        self.disable_strip = args.disable_strip
        self.vs_version = args.vs_version
        self.use_incredibuild = False
        print args.app_abi
        if args.app_abi is None:
            self.app_abi = 'armeabi'
        else:
            self.app_abi = args.app_abi

        self.lib_dir = os.path.normpath(os.path.join(self.repo_x, self.cfg_info[LibsGenerator.KEY_LIBS_OUTPUT]))

    def parse_config(self):
        if not os.path.isfile(self.cfg_file_path):
            raise utils.YHError("%s is not a valid config file." % self.cfg_file_path,
                                utils.YHError.ERROR_PATH_NOT_FOUND)

        try:
            f = open(self.cfg_file_path)
            self.cfg_info = json.load(f)
            f.close()
        except:
            raise utils.YHError("Parse %s failed." % self.cfg_file_path,
                                utils.YHError.ERROR_PARSE_FILE)

        for k in LibsGenerator.CHECK_KEYS:
            if k not in self.cfg_info.keys():
                raise utils.YHError("Key %s is not found in %s" %(k, self.cfg_file_path),
                                    utils.YHError.ERROR_WRONG_CONFIG)

    def get_cfg_info(self):
        return self.cfg_info
   
    def _run_cmd(self, command, cwd=None):
        utils.CMDRunner.run_cmd(command, self._verbose, cwd=cwd)   
        
    def compile(self):
        if self.clean:
            self.clean_libs()

        if utils.os_is_mac():
            if self.build_mac or self.build_ios:
                self.compile_mac_ios()

        if utils.os_is_win32():
            if self.build_win:
                self.compile_win()

        if self.build_android:
            self.compile_android()
            # generate prebuilt mk files
            self.modify_binary_mk()

    def build_win32_proj(self, cmd_path, sln_path, proj_name, mode):
        build_cmd = " ".join([
            "\"%s\"" % cmd_path,
            "\"%s\"" % sln_path,
            "/t:%s" % proj_name,
            "/property:Configuration=%s" % mode,
            "/m"
        ])
        self._run_cmd(build_cmd)

    def compile_win(self):
        if self.mode == 'debug':
            mode_str = 'Debug'
        else:
            mode_str = 'Release'

        # get the VS versions will be used for compiling
        support_vs_versions = self.cfg_info[LibsGenerator.KEY_SUPPORT_VS_VERSIONS]
        compile_vs_versions = support_vs_versions
        if self.vs_version is not None:
            if self.vs_version not in support_vs_versions:
                raise utils.YHError("Not support VS%d." % self.vs_version,
                                    utils.YHError.ERROR_WRONG_ARGS)
            else:
                compile_vs_versions = [ self.vs_version ]

        vs_cmd_info = {}
        for vs_version in compile_vs_versions:
            # get the vs command with specified version
            vs_command = utils.get_msbuild_path(vs_version)
            if vs_command is None:
                utils.Logging.warning("Not found VS%d" % vs_version)
            else:
                vs_cmd_info[vs_version] = vs_command

        if len(vs_cmd_info) == 0:
            raise utils.YHError("Not found available Visual Studio.",
                                utils.YHError.ERROR_TOOLS_NOT_FOUND)

        # get the VS projects info
        win32_proj_info = self.cfg_info[LibsGenerator.KEY_VS_PROJS_INFO]
        for vs_version in compile_vs_versions:
            if not vs_version in vs_cmd_info.keys():
                continue

            try:
                vs_command = vs_cmd_info[vs_version]
                for key in win32_proj_info.keys():
                    # clean solutions
                    proj_path = os.path.join(self.repo_x, key)
                    clean_cmd = " ".join([
                        "\"%s\"" % vs_command,
                        "\"%s\"" % proj_path,
                        "/t:Clean /p:Configuration=%s" % mode_str
                    ])
                    self._run_cmd(clean_cmd)

                for key in win32_proj_info.keys():
                    output_dir = os.path.join(self.lib_dir, "win32")
                    proj_path = os.path.join(self.repo_x, key)

                    # get the build folder & win32 output folder
                    build_folder_path = os.path.join(os.path.dirname(proj_path), "%s.win32" % mode_str)
                    win32_output_dir = os.path.join(self.repo_x, output_dir)
                    if not os.path.exists(win32_output_dir):
                        os.makedirs(win32_output_dir)

                    # build project
                    if self.use_incredibuild:
                        # use incredibuild, build whole sln
                        build_cmd = " ".join([
                            "BuildConsole",
                            "%s" % proj_path,
                            "/build",
                            "/cfg=\"%s|Win32\"" % mode_str
                        ])
                        self._run_cmd(build_cmd)
                    else:
                        for proj_name in win32_proj_info[key][LibsGenerator.KEY_VS_BUILD_TARGETS]:
                            # build the projects
                            self.build_win32_proj(vs_command, proj_path, proj_name, mode_str)

                    # copy the libs into prebuilt dir
                    for file_name in os.listdir(build_folder_path):
                        name, ext = os.path.splitext(file_name)
                        if ext != ".lib" and ext != ".dll":
                            continue

                        file_path = os.path.join(build_folder_path, file_name)
                        shutil.copy(file_path, win32_output_dir)

                    # rename the specified libs
                    suffix = "_%d" % vs_version
                    for proj_name in win32_proj_info[key][LibsGenerator.KEY_VS_RENAME_TARGETS]:
                        src_name = os.path.join(win32_output_dir, "%s.lib" % proj_name)
                        dst_name = os.path.join(win32_output_dir, "%s%s.lib" % (proj_name, suffix))
                        if not os.path.exists(src_name):
                            raise utils.YHError(MultiLanguage.get_string('GEN_LIBS_ERROR_LIB_NOT_GEN_FMT', src_name),
                                                utils.YHError.ERROR_PATH_NOT_FOUND)

                        if os.path.exists(dst_name):
                            os.remove(dst_name)
                        os.rename(src_name, dst_name)
            except Exception as e:
                raise e

    def compile_mac_ios(self):
        xcode_proj_info = self.cfg_info[LibsGenerator.KEY_XCODE_PROJS_INFO]
        if self.mode == 'debug':
            mode_str = 'Debug'
        else:
            mode_str = 'Release'

        XCODE_CMD_FMT = "xcodebuild -project \"%s\" -configuration %s -target \"%s\" %s CONFIGURATION_BUILD_DIR=%s"
        ios_out_dir = os.path.join(self.lib_dir, "ios")
        mac_out_dir = os.path.join(self.lib_dir, "mac")
        ios_sim_libs_dir = os.path.join(ios_out_dir, "simulator")
        ios_dev_libs_dir = os.path.join(ios_out_dir, "device")
        for key in xcode_proj_info.keys():
            proj_path = os.path.join(self.repo_x, key)
            target = xcode_proj_info[key][LibsGenerator.KEY_XCODE_TARGETS]

            if self.build_mac:
                # compile mac
                build_cmd = XCODE_CMD_FMT % (proj_path, mode_str, "%s Mac" % target, "", mac_out_dir)
                self._run_cmd(build_cmd)

            if self.build_ios:
                # compile ios simulator
                build_cmd = XCODE_CMD_FMT % (proj_path, mode_str, "%s iOS" % target, "-sdk iphonesimulator ARCHS=\"i386 x86_64\" VALID_ARCHS=\"i386 x86_64\"", ios_sim_libs_dir)
                self._run_cmd(build_cmd)

                # compile ios device
                build_cmd = XCODE_CMD_FMT % (proj_path, mode_str, "%s iOS" % target, "-sdk iphoneos", ios_dev_libs_dir)
                self._run_cmd(build_cmd)

        if self.build_ios:
            # generate fat libs for iOS
            for lib in os.listdir(ios_sim_libs_dir):
                sim_lib = os.path.join(ios_sim_libs_dir, lib)
                dev_lib = os.path.join(ios_dev_libs_dir, lib)
                output_lib = os.path.join(ios_out_dir, lib)
                lipo_cmd = "lipo -create -output \"%s\" \"%s\" \"%s\"" % (output_lib, sim_lib, dev_lib)

                self._run_cmd(lipo_cmd)

            # remove the simulator & device libs in iOS
            utils.rmdir(ios_sim_libs_dir)
            utils.rmdir(ios_dev_libs_dir)

        if not self.disable_strip:
            # strip the libs
            if self.build_ios:
                ios_strip_cmd = "xcrun -sdk iphoneos strip -S %s/*.a" % ios_out_dir
                self._run_cmd(ios_strip_cmd)
            if self.build_mac:
                mac_strip_cmd = "xcrun strip -S %s/*.a" % mac_out_dir
                self._run_cmd(mac_strip_cmd)

    def compile_android(self):
        # build .so for android
        ANDROID_A_PATH = "obj/local"
        proj_path = os.path.join(self.repo_x, "build/proj.android")
        
        android_out_dir = os.path.join(self.lib_dir, "android")
               
        builder = AndroidBuilder(True, proj_path,True, None, False)
               
        jobs=4
          
        ndk_build_param = [
            "-j8"
        ]
                                             
        if self.app_abi:
            abi_param = "APP_ABI=\"%s\"" % " ".join(self.app_abi.split(":"))
            ndk_build_param.append(abi_param)

        builder.do_ndk_build(ndk_build_param, self.mode, None)

        # copy .a to prebuilt dir
        obj_dir = os.path.join(proj_path, ANDROID_A_PATH)
        copy_cfg = {
            "from": obj_dir,
            "to": android_out_dir,
            "include": [
                "*.a$"
            ]
        }
        utils.copy_files_with_config(copy_cfg, obj_dir, android_out_dir)

        if not self.disable_strip:
            # strip the android libs
            ndk_root = os.environ["NDK_ROOT"]
            if utils.os_is_win32():
                if utils.os_is_32bit_windows():
                    check_bits = [ "", "-x86_64" ]
                else:
                    check_bits = [ "-x86_64", "" ]

                sys_folder_name = "windows"
                for bit_str in check_bits:
                    check_folder_name = "windows%s" % bit_str
                    check_path = os.path.join(ndk_root, "toolchains/arm-linux-androideabi-4.8/prebuilt/%s" % check_folder_name)
                    if os.path.isdir(check_path):
                        sys_folder_name = check_folder_name
                        break
            elif utils.os_is_mac():
                sys_folder_name = "darwin-x86_64"
            else:
                sys_folder_name = "linux-x86_64"

            # set strip execute file name
            if utils.os_is_win32():
                strip_execute_name = "strip.exe"
            else:
                strip_execute_name = "strip"

            # strip arm libs
            strip_cmd_path = os.path.join(ndk_root, "toolchains/arm-linux-androideabi-4.8/prebuilt/%s/arm-linux-androideabi/bin/%s"
                % (sys_folder_name, strip_execute_name))
            if os.path.exists(strip_cmd_path):
                armlibs = ["armeabi", "armeabi-v7a"]
                for fold in armlibs:
                    self.trip_libs(strip_cmd_path, os.path.join(android_out_dir, fold))

            # strip x86 libs
            strip_cmd_path = os.path.join(ndk_root, "toolchains/x86-4.8/prebuilt/%s/i686-linux-android/bin/%s" % (sys_folder_name, strip_execute_name))
            if os.path.exists(strip_cmd_path) and os.path.exists(os.path.join(android_out_dir, "x86")):
                self.trip_libs(strip_cmd_path, os.path.join(android_out_dir, 'x86'))

    def trip_libs(self, strip_cmd, folder):
        if not os.path.isdir(folder):
            return

        if utils.os_is_win32():
            for name in os.listdir(folder):
                basename, ext = os.path.splitext(name)
                if ext == ".a":
                    full_name = os.path.join(folder, name)
                    command = "%s -S %s" % (strip_cmd, full_name)
                    self._run_cmd(command)
        else:
            strip_cmd = "%s -S %s/*.a" % (strip_cmd, folder)
            self._run_cmd(strip_cmd)

    def modify_binary_mk(self):
        print "modify prebuild mk"
        android_libs = os.path.join(self.lib_dir, "android")
        android_mks = self.cfg_info[LibsGenerator.KEY_ANDROID_MKS]

        for mk_file in android_mks:
            mk_file_path = os.path.join(self.repo_x, mk_file)
            dst_file_path = os.path.join(android_libs, os.path.basename(mk_file_path))
            print("%s => %s" %(mk_file_path,dst_file_path))
            tmp_obj = generate_prebuilt_mk.PrebuiltMKGenerator(mk_file_path, android_libs, dst_file_path,os.path.dirname(dst_file_path))
            tmp_obj.do_generate()

    def clean_libs(self):
        print('clean %s' % self.lib_dir)
        utils.rmdir(self.lib_dir)

if __name__ == "__main__":
    genLibs = LibsGenerator()
    genLibs.run(sys.argv[1:])