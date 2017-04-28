#!/usr/bin/python
# build_native.py
# Build native codes


import sys
import os, os.path
import shutil
import utils
import json
import re
from xml.dom import minidom

import build_project

BUILD_CFIG_FILE="build-cfg.json"

class AndroidBuilder(object):

    CFG_KEY_COPY_TO_ASSETS = "copy_to_assets"
    CFG_KEY_MUST_COPY_TO_ASSERTS = "must_copy_to_assets"
    CFG_KEY_STORE = "key_store"
    CFG_KEY_STORE_PASS = "key_store_pass"
    CFG_KEY_ALIAS = "alias"
    CFG_KEY_ALIAS_PASS = "alias_pass"

    ANT_KEY_STORE = "key.store"
    ANT_KEY_ALIAS = "key.alias"
    ANT_KEY_STORE_PASS = "key.store.password"
    ANT_KEY_ALIAS_PASS = "key.alias.password"

    GRADLE_KEY_STORE = "RELEASE_STORE_FILE"
    GRADLE_KEY_ALIAS = "RELEASE_KEY_ALIAS"
    GRADLE_KEY_STORE_PASS = "RELEASE_STORE_PASSWORD"
    GRADLE_KEY_ALIAS_PASS = "RELEASE_KEY_PASSWORD"

    def __init__(self, verbose, app_android_root, no_res, proj_obj, use_studio=False,exts=None):
        self._verbose = verbose

        self.app_android_root = app_android_root
        self._no_res = no_res
        self._project = proj_obj
        self.use_studio = use_studio
        self.ndk_module_paths = []
        
        # check environment variable
        if self.use_studio:
            self.ant_root = None
            self.sign_prop_file = os.path.join(self.app_android_root, 'app', "gradle.properties")
        else:
            if exts is not None and exts.has_key('ANT_ROOT'):
                self.ant_root =exts['ANT_ROOT']
            else:
                self.ant_root = utils.check_environment_variable('ANT_ROOT')
            self.sign_prop_file = os.path.join(self.app_android_root, "ant.properties")
        if exts is not None and exts.has_key('ANDROID_SDK_ROOT'):
            self.sdk_root = exts['ANDROID_SDK_ROOT']
        else:
            self.sdk_root = utils.check_environment_variable('ANDROID_SDK_ROOT')

        if exts is not None and exts.has_key('NDK_ROOT'):
            self.ndk_root = exts['NDK_ROOT']        
        else:
            self.ndk_root = None
            
        self._parse_cfg()

    def _run_cmd(self, command, cwd=None):
        utils.CMDRunner.run_cmd(command, self._verbose, cwd=cwd)

    def _parse_cfg(self):
        self.cfg_path = os.path.join(self.app_android_root, BUILD_CFIG_FILE)
        if (os.path.exists(self.cfg_path) and os.path.isfile(self.cfg_path)):       
            try:
                f = open(self.cfg_path)
                cfg = json.load(f, encoding='utf8')
                f.close()
            except Exception:
                raise utils.YHError("error parse cfg[%s] failed " %(self.cfg_path),
                                          utils.YHError.ERROR_PARSE_FILE)

            if cfg.has_key(build_project.BuildProject.CFG_KEY_MUST_COPY_RESOURCES):
                if self._no_res:
                    self.res_files = cfg[build_project.BuildProject.CFG_KEY_MUST_COPY_RESOURCES]
                else:
                    self.res_files = cfg[build_project.BuildProject.CFG_KEY_MUST_COPY_RESOURCES] + cfg[build_project.BuildProject.CFG_KEY_COPY_RESOURCES]
            else:
                self.res_files = cfg[build_project.BuildProject.CFG_KEY_COPY_RESOURCES]

            self.ndk_module_paths = cfg['ndk_module_path']

            # get the properties for sign release apk
            if self.use_studio:
                self.key_store_str = AndroidBuilder.GRADLE_KEY_STORE
                self.key_alias_str = AndroidBuilder.GRADLE_KEY_ALIAS
                self.key_store_pass_str = AndroidBuilder.GRADLE_KEY_STORE_PASS
                self.key_alias_pass_str = AndroidBuilder.GRADLE_KEY_ALIAS_PASS
            else:
                self.key_store_str = AndroidBuilder.ANT_KEY_STORE
                self.key_alias_str = AndroidBuilder.ANT_KEY_ALIAS
                self.key_store_pass_str = AndroidBuilder.ANT_KEY_STORE_PASS
                self.key_alias_pass_str = AndroidBuilder.ANT_KEY_ALIAS_PASS

            move_cfg = {}
            self.key_store = None
            if cfg.has_key(AndroidBuilder.CFG_KEY_STORE):
                self.key_store = cfg[AndroidBuilder.CFG_KEY_STORE]
                move_cfg[self.key_store_str] = self.key_store
                del cfg[AndroidBuilder.CFG_KEY_STORE]

            self.key_store_pass = None
            if cfg.has_key(AndroidBuilder.CFG_KEY_STORE_PASS):
                self.key_store_pass = cfg[AndroidBuilder.CFG_KEY_STORE_PASS]
                move_cfg[self.key_store_pass_str] = self.key_store_pass
                del cfg[AndroidBuilder.CFG_KEY_STORE_PASS]

            self.alias = None
            if cfg.has_key(AndroidBuilder.CFG_KEY_ALIAS):
                self.alias = cfg[AndroidBuilder.CFG_KEY_ALIAS]
                move_cfg[self.key_alias_str] = self.alias
                del cfg[AndroidBuilder.CFG_KEY_ALIAS]

            self.alias_pass = None
            if cfg.has_key(AndroidBuilder.CFG_KEY_ALIAS_PASS):
                self.alias_pass = cfg[AndroidBuilder.CFG_KEY_ALIAS_PASS]
                move_cfg[self.key_alias_pass_str] = self.alias_pass
                del cfg[AndroidBuilder.CFG_KEY_ALIAS_PASS]

            if len(move_cfg) > 0:
                # move the config into ant.properties
                self._move_cfg(move_cfg)
                with open(self.cfg_path, 'w') as outfile:
                    json.dump(cfg, outfile, sort_keys = True, indent = 4)
                    outfile.close()
        
        
    def has_keystore_in_signprops(self):
        keystore = None
        if self.use_studio:
            pattern = re.compile(r"^RELEASE_STORE_FILE=(.+)")
        else:
            pattern = re.compile(r"^key\.store=(.+)")

        try:
            file_obj = open(self.sign_prop_file)
            for line in file_obj:
                str1 = line.replace(' ', '')
                str2 = str1.replace('\t', '')
                match = pattern.match(str2)
                if match is not None:
                    keystore = match.group(1)
                    break
            file_obj.close()
        except:
            pass

        if keystore is None:
            return False
        else:
            return True

    def _write_sign_properties(self, cfg):
        file_obj = open(self.sign_prop_file, "a+")
        for key in cfg.keys():
            str_cfg = "%s=%s\n" % (key, cfg[key])
            file_obj.write(str_cfg)

        file_obj.close()

    def _move_cfg(self, cfg):
        if not self.has_keystore_in_signprops():
            self._write_sign_properties(cfg)

    def remove_c_libs(self, libs_dir):
        for file_name in os.listdir(libs_dir):
            lib_file = os.path.join(libs_dir,  file_name)
            if os.path.isfile(lib_file):
                ext = os.path.splitext(lib_file)[1]
                if ext == ".a" or ext == ".so":
                    os.remove(lib_file)
                    
    def update_project(self, android_platform):
        if self.use_studio:
            manifest_path = os.path.join(self.app_android_root, 'app')
        else:
            manifest_path = self.app_android_root

        sdk_tool_path = os.path.join(self.sdk_root, "tools", "android")

        # check the android platform
        target_str = self.check_android_platform(self.sdk_root, android_platform, manifest_path, False)

        # update project
        command = "%s update project -t %s -p %s" % (utils.CMDRunner.convert_path_to_cmd(sdk_tool_path), target_str, manifest_path)
        self._run_cmd(command)

        # update lib-projects
        property_path = manifest_path
        self.update_lib_projects(self.sdk_root, sdk_tool_path, android_platform, property_path)

        if self.use_studio:
            # copy the local.properties to the app_android_root
            file_name = 'local.properties'
            src_path = os.path.normpath(os.path.join(manifest_path, file_name))
            dst_path = os.path.normpath(os.path.join(self.app_android_root, file_name))
            if src_path != dst_path:
                if os.path.isfile(dst_path):
                    os.remove(dst_path)
                shutil.copy(src_path, dst_path)

    def get_toolchain_version(self, ndk_root, compile_obj):
        # use the folder name in toolchains to check get gcc version
        toolchains_path = os.path.join(ndk_root, 'toolchains')
        dir_names = os.listdir(toolchains_path)
        # check if gcc 4.9 exists
        for dir_name in dir_names:
            if dir_name.endswith('4.9'):
                return 4.9

        # use gcc 4.8
        #compile_obj.add_warning_at_end("Your application may crash when using c++ 11 regular expression with NDK_TOOLCHAIN_VERSION %s" %('4.8'))
        return '4.8'


    def do_ndk_build(self, ndk_build_param, build_mode, compile_obj):
        utils.Logging.info("NDK build mode: %s" %(build_mode))
        if self.ndk_root :
            ndk_root = self.ndk_root
        else:
            ndk_root = utils.check_environment_variable('NDK_ROOT')        

        toolchain_version = self.get_toolchain_version(ndk_root, compile_obj)

        if self.use_studio:
            ndk_work_dir = os.path.join(self.app_android_root, 'app')
        else:
            ndk_work_dir = self.app_android_root
        reload(sys)
        sys.setdefaultencoding('utf8')
        ndk_path = utils.CMDRunner.convert_path_to_cmd(os.path.join(ndk_root, "ndk-build"))

        module_paths = []
        if self.ndk_module_paths is not None:
            for cfg_path in self.ndk_module_paths:
                module_paths.append(os.path.join(self.app_android_root, cfg_path))

        # delete template static and dynamic files
        obj_local_dir = os.path.join(ndk_work_dir, "obj", "local")
        if os.path.isdir(obj_local_dir):
            for abi_dir in os.listdir(obj_local_dir):
                static_file_path = os.path.join(ndk_work_dir, "obj", "local", abi_dir)
                if os.path.isdir(static_file_path):
                    self.remove_c_libs(static_file_path)
                
        # windows should use ";" to seperate module paths
        if utils.os_is_win32():
            ndk_module_path = ';'.join(module_paths)
        else:
            ndk_module_path = ':'.join(module_paths)
        
        ndk_module_path= 'NDK_MODULE_PATH=' + ndk_module_path

        if ndk_build_param is None:
            ndk_build_cmd = '%s -C %s %s' % (ndk_path, ndk_work_dir, ndk_module_path)
        else:
            ndk_build_cmd = '%s -C %s %s %s' % (ndk_path, ndk_work_dir, ' '.join(ndk_build_param), ndk_module_path)

        ndk_build_cmd = '%s NDK_TOOLCHAIN_VERSION=%s' % (ndk_build_cmd, toolchain_version)

        if build_mode == 'debug':
            ndk_build_cmd = '%s NDK_DEBUG=1' % ndk_build_cmd

        self._run_cmd(ndk_build_cmd)


    def _xml_attr(self, dir, file_name, node_name, attr):
        doc = minidom.parse(os.path.join(dir, file_name))
        return doc.getElementsByTagName(node_name)[0].getAttribute(attr)

    def update_lib_projects(self, sdk_root, sdk_tool_path, android_platform, property_path):
        property_file = os.path.join(property_path, "project.properties")
        if not os.path.isfile(property_file):
            return

        patten = re.compile(r'^android\.library\.reference\.[\d]+=(.+)')
        for line in open(property_file):
            str1 = line.replace(' ', '')
            str2 = str1.replace('\t', '')
            match = patten.match(str2)
            if match is not None:
                # a lib project is found
                lib_path = match.group(1)
                abs_lib_path = os.path.join(property_path, lib_path)
                abs_lib_path = os.path.normpath(abs_lib_path)
                if os.path.isdir(abs_lib_path):
                    target_str = self.check_android_platform(sdk_root, android_platform, abs_lib_path, True)
                    command = "%s update lib-project -p %s -t %s" % (utils.CMDRunner.convert_path_to_cmd(sdk_tool_path), abs_lib_path, target_str)
                    self._run_cmd(command)

                    self.update_lib_projects(sdk_root, sdk_tool_path, android_platform, abs_lib_path)

    def select_default_android_platform(self, min_api_level):
        ''' select a default android platform in SDK_ROOT
        '''

        sdk_root = utils.check_environment_variable('ANDROID_SDK_ROOT')
        platforms_dir = os.path.join(sdk_root, "platforms")
        ret_num = -1
        ret_platform = ""
        if os.path.isdir(platforms_dir):
            for dir_name in os.listdir(platforms_dir):
                if not os.path.isdir(os.path.join(platforms_dir, dir_name)):
                    continue

                num = self.get_api_level(dir_name, raise_error=False)
                if num >= min_api_level:
                    if ret_num == -1 or ret_num > num:
                        ret_num = num
                        ret_platform = dir_name

        if ret_num != -1:
            return ret_platform
        else:
            return None


    def get_api_level(self, target_str, raise_error=True):
        special_targats_info = {
            "android-4.2" : 17,
            "android-L" : 20
        }

        if special_targats_info.has_key(target_str):
            ret = special_targats_info[target_str]
        else:
            match = re.match(r'android-(\d+)', target_str)
            if match is not None:
                ret = int(match.group(1))
            else:
                if raise_error:
                    raise utils.YHError("%s is not a valid Android target platform." %(target_str),
                                              utils.YHError.ERROR_PARSE_FILE)
                else:
                    ret = -1

        return ret

    def get_target_config(self, proj_path):
        property_file = os.path.join(proj_path, "project.properties")
        if not os.path.isfile(property_file):
            raise utils.YHError("%s is not found" %(property_file),
                                      utils.YHError.ERROR_PATH_NOT_FOUND)

        patten = re.compile(r'^target=(.+)')
        for line in open(property_file):
            str1 = line.replace(' ', '')
            str2 = str1.replace('\t', '')
            match = patten.match(str2)
            if match is not None:
                target = match.group(1)
                target_num = self.get_api_level(target)
                if target_num > 0:
                    return target_num

        raise utils.YHError("Can't find 'target' in file '%s'" %(property_file),
                                  utils.YHError.ERROR_PARSE_FILE)

    # check the selected android platform
    def check_android_platform(self, sdk_root, android_platform, proj_path, auto_select):
        ret = android_platform
        min_platform = self.get_target_config(proj_path)
        if android_platform is None:
            # not specified platform, found one
            utils.Logging.info("Android platform not specified, searching a default one...")
            ret = self.select_default_android_platform(min_platform)
        else:
            # check whether it's larger than min_platform
            select_api_level = self.get_api_level(android_platform)
            if select_api_level < min_platform:
                if auto_select:
                    # select one for project
                    ret = self.select_default_android_platform(min_platform)
                else:
                    # raise error
                    raise utils.YHError("The Android-platform of project '%s' should be equal/larger than %d, but %d is specified."%(proj_path, min_platform, select_api_level),
                                              utils.YHError.ERROR_WRONG_ARGS)

        if ret is None:
            raise utils.YHError( "Can't find right Android-platform for project : '%s'. The Android-platform should be equal/larger than %d." %(proj_path, min_platform),
                                      utils.YHError.ERROR_PARSE_FILE)

        ret_path = os.path.join(utils.CMDRunner.convert_path_to_python(sdk_root), "platforms", ret)
        if not os.path.isdir(ret_path):
            raise utils.YHError("The directory '%s' can't be found in Android SDK" % ret,
                                      utils.YHError.ERROR_PATH_NOT_FOUND)

        special_platforms_info = {
            "android-4.2" : "android-17"
        }
        if special_platforms_info.has_key(ret):
            ret = special_platforms_info[ret]

        return ret

    def ant_build_apk(self, build_mode, custom_step_args):
        app_android_root = self.app_android_root

        # run ant build
        ant_path = os.path.join(self.ant_root, 'ant')
        buildfile_path = os.path.join(app_android_root, "build.xml")

        # generate paramters for custom step
        args_ant_copy = custom_step_args.copy()
        target_platform = build_project.Platforms.ANDROID

        # invoke custom step: pre-ant-build
        #self._project.invoke_custom_step_script(build_project.BuildProject.CUSTOM_STEP_PRE_ANT_BUILD,
        #                                        target_platform, args_ant_copy)

        command = "%s clean %s -f %s -Dsdk.dir=%s" % (utils.CMDRunner.convert_path_to_cmd(ant_path),
                                                      build_mode, buildfile_path,
                                                      utils.CMDRunner.convert_path_to_cmd(self.sdk_root))
        self._run_cmd(command)

        # invoke custom step: post-ant-build
        #self._project.invoke_custom_step_script(build_project.BuildProject.CUSTOM_STEP_POST_ANT_BUILD,
        #                                        target_platform, args_ant_copy)

    def gradle_build_apk(self, build_mode):
        # check the compileSdkVersion & buildToolsVersion
        check_file = os.path.join(self.app_android_root, 'app', 'build.gradle')
        f = open(check_file)
        lines = f.readlines()
        f.close()

        compile_sdk_ver = None
        build_tools_ver = None
        compile_sdk_pattern = r'compileSdkVersion[ \t]+([\d]+)'
        build_tools_pattern = r'buildToolsVersion[ \t]+"(.+)"'
        for line in lines:
            line_str = line.strip()
            match1 = re.match(compile_sdk_pattern, line_str)
            if match1:
                compile_sdk_ver = match1.group(1)

            match2 = re.match(build_tools_pattern, line_str)
            if match2:
                build_tools_ver = match2.group(1)

        if compile_sdk_ver is not None:
            # check the compileSdkVersion
            check_folder_name = 'android-%s' % compile_sdk_ver
            check_path = os.path.join(self.sdk_root, 'platforms', check_folder_name)
            if not os.path.isdir(check_path):
                utils.Logging.warning("The value of compileSdkVersion is %s in file build.gradle, but %s is not found."%(compile_sdk_ver, check_path))

        if build_tools_ver is not None:
            # check the buildToolsVersion
            check_path = os.path.join(self.sdk_root, 'build-tools', build_tools_ver)
            if not os.path.isdir(check_path):
                utils.Logging.warning("The value of buildToolsVersion is %s in file build.gradle, but %s is not found." %(build_tools_ver, check_path))

        # invoke gradlew for gradle building
        if utils.os_is_win32():
            gradle_path = os.path.join(self.app_android_root, 'gradlew.bat')
        else:
            gradle_path = os.path.join(self.app_android_root, 'gradlew')

        if not os.path.isfile(gradle_path):
            raise utils.YHError("%s is not existed." % gradle_path,
                                      utils.YHError.ERROR_PATH_NOT_FOUND)

        mode_str = 'Debug' if build_mode == 'debug' else 'Release'
        cmd = '"%s" --parallel --info assemble%s' % (gradle_path, mode_str)
        self._run_cmd(cmd, cwd=self.app_android_root)

    def do_build_apk(self, build_mode, no_apk, output_dir, custom_step_args, compile_obj):
        if self.use_studio:
            assets_dir = os.path.join(self.app_android_root, "app", "assets")
            project_name = None
            setting_file = os.path.join(self.app_android_root, 'settings.gradle')
            if os.path.isfile(setting_file):
                # get project name from settings.gradle
                f = open(setting_file)
                lines = f.readlines()
                f.close()

                pattern = r"project\(':(.*)'\)\.projectDir[ \t]*=[ \t]*new[ \t]*File\(settingsDir, 'app'\)"
                for line in lines:
                    line_str = line.strip()
                    match = re.match(pattern, line_str)
                    if match:
                        project_name = match.group(1)
                        break

            if project_name is None:
                # use default project name
                project_name = 'app'
            gen_apk_folder = os.path.join(self.app_android_root, 'app/build/outputs/apk')
        else:
            assets_dir = os.path.join(self.app_android_root, "assets")
            project_name = self._xml_attr(self.app_android_root, 'build.xml', 'project', 'name')
            gen_apk_folder = os.path.join(self.app_android_root, 'bin')

        # copy resources
        self._copy_resources(custom_step_args, assets_dir)

        if not no_apk:
            # gather the sign info if necessary
            if build_mode == "release" and not self.has_keystore_in_signprops():
                self._gather_sign_info()

            # build apk
            if self.use_studio:
                self.gradle_build_apk(build_mode)
            else:
                self.ant_build_apk(build_mode, custom_step_args)

            # copy the apk to output dir
            if output_dir:
                apk_name = '%s-%s.apk' % (project_name, build_mode)
                gen_apk_path = os.path.join(gen_apk_folder, apk_name)
                if not os.path.exists(output_dir):
                    os.makedirs(output_dir)
                shutil.copy(gen_apk_path, output_dir)
                utils.Logging.info("Move apk to %s"%output_dir)

                if build_mode == "release":
                    signed_name = "%s-%s-signed.apk" % (project_name, build_mode)
                    apk_path = os.path.join(output_dir, signed_name)
                    if os.path.exists(apk_path):
                        os.remove(apk_path)
                    os.rename(os.path.join(output_dir, apk_name), apk_path)
                else:
                    apk_path = os.path.join(output_dir, apk_name)

                return apk_path
            else:
                raise utils.YHError("Not specified the output directory!",
                                          utils.YHError.ERROR_WRONG_ARGS)

    def _gather_sign_info(self):
        user_cfg = {}
        # get the path of keystore file
        while True:
            inputed = self._get_user_input("Please input the absolute/relative path of '.keystore' file:")
            inputed = inputed.strip()
            if not os.path.isabs(inputed):
                if self.use_studio:
                    start_path = os.path.join(self.app_android_root, 'app')
                else:
                    start_path = self.app_android_root
                abs_path = os.path.join(start_path, inputed)
            else:
                abs_path = inputed

            if os.path.isfile(abs_path):
                user_cfg[self.key_store_str] = inputed.replace('\\', '/')
                break
            else:
                utils.Logging.warning("The string inputed is not a file!")

        # get the alias of keystore file
        user_cfg[self.key_alias_str] = self._get_user_input("Please input the alias:")

        # get the keystore password
        user_cfg[self.key_store_pass_str] = self._get_user_input("Please input the password of key store:")

        # get the alias password
        user_cfg[self.key_alias_pass_str] = self._get_user_input("Please input the password of alias:")

        # write the config into ant.properties
        self._write_sign_properties(user_cfg)

    def _get_user_input(self, tip_msg):
        utils.Logging.warning(tip_msg)
        ret = None
        while True:
            ret = raw_input()
            break

        return ret

    def _copy_resources(self, custom_step_args, assets_dir):
        app_android_root = self.app_android_root
        res_files = self.res_files

        # remove app_android_root/assets if it exists
        if os.path.isdir(assets_dir):
            shutil.rmtree(assets_dir)

        # generate parameters for custom steps
        target_platform = build_project.Platforms.ANDROID
        cur_custom_step_args = custom_step_args.copy()
        cur_custom_step_args["assets-dir"] = assets_dir

        # make dir
        os.mkdir(assets_dir)
 
        # invoke custom step : pre copy assets
        #self._project.invoke_custom_step_script(build_project.BuildProject.CUSTOM_STEP_PRE_COPY_ASSETS, target_platform, cur_custom_step_args)

        # copy resources
        for cfg in res_files:
            utils.copy_files_with_config(cfg, app_android_root, assets_dir)

        # invoke custom step : post copy assets
        #self._project.invoke_custom_step_script(build_project.BuildProject.CUSTOM_STEP_POST_COPY_ASSETS, target_platform, cur_custom_step_args)

    def get_apk_info(self):
        if self.use_studio:
            manifest_path = os.path.join(self.app_android_root, 'app')
            gradle_cfg_path = os.path.join(manifest_path, 'build.gradle')
            package = None
            if os.path.isfile(gradle_cfg_path):
                # get package name from build.gradle
                f = open(gradle_cfg_path)
                for line in f.readlines():
                    line_str = line.strip()
                    pattern = r'applicationId[ \t]+"(.*)"'
                    match = re.match(pattern, line_str)
                    if match:
                        package = match.group(1)
                        break

            if package is None:
                # get package name from AndroidManifest.xml
                package = self._xml_attr(manifest_path, 'AndroidManifest.xml', 'manifest', 'package')
        else:
            manifest_path = self.app_android_root
            package = self._xml_attr(manifest_path, 'AndroidManifest.xml', 'manifest', 'package')

        activity_name = self._xml_attr(manifest_path, 'AndroidManifest.xml', 'activity', 'android:name')
        if activity_name.startswith('.'):
            activity = package + activity_name
        else:
            activity = activity_name
        ret = (package, activity)

        return ret
