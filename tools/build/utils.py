#!/usr/bin/python
# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
'''

'''

__docformat__ = 'restructuredtext'


# python
import sys
import os
import subprocess
from contextlib import contextmanager
import shutil
import string
import re

class Logging:
    # TODO maybe the right way to do this is to use something like colorama?
    RED = '\033[31m'
    GREEN = '\033[32m'
    YELLOW = '\033[33m'
    MAGENTA = '\033[35m'
    RESET = '\033[0m'

    @staticmethod
    def _print(s, color=None):
        if color and sys.stdout.isatty() and sys.platform != 'win32':
            print(color + s + Logging.RESET)
        else:
            print(s)

    @staticmethod
    def debug(s):
        Logging._print(s, Logging.MAGENTA)

    @staticmethod
    def info(s):
        Logging._print(s, Logging.GREEN)

    @staticmethod
    def warning(s):
        Logging._print(s, Logging.YELLOW)

    @staticmethod
    def error(s):
        Logging._print(s, Logging.RED)

    @staticmethod
    def _log_path():
        log_dir = os.path.expanduser("~/.yh")
        if not os.path.exists(log_dir):
            os.mkdir(log_dir)
        return os.path.join(log_dir, "yh.log")

class YHError(Exception):
    ERROR_WRONG_ARGS = 11           # wrong arguments
    ERROR_PATH_NOT_FOUND = 12       # path not found
    ERROR_BUILD_FAILED = 13         # build failed
    ERROR_RUNNING_CMD = 14          # error when running command
    ERROR_CMD_NOT_FOUND = 15        # command not found
    ERROR_ENV_VAR_NOT_FOUND = 16    # environment variable not found
    ERROR_TOOLS_NOT_FOUND = 17      # depend on tools not found
    ERROR_PARSE_FILE = 18           # error when parse files
    ERROR_WRONG_CONFIG = 19         # configuration is wrong

    ERROR_OTHERS = 101              # other errors

    def __init__(self, err_args, err_no=1):
        super(YHError, self).__init__(err_args)
        self.error_no = err_no

    def get_error_no(self):
        return self.error_no


class CMDRunner(object):

    @staticmethod
    def run_cmd(command, verbose, cwd=None):
        if verbose:
            Logging.debug("run:%s"%command)
        else:
            log_path = Logging._log_path()
            command += ' >"%s" 2>&1' % log_path
        sys.stdout.flush()
        ret = subprocess.call(command, shell=True, cwd=cwd)
        if ret != 0:
            message = str(ret)
            if not verbose:
                message += log_path
            raise YHError(message, YHError.ERROR_RUNNING_CMD)

    @staticmethod
    def output_for(command, verbose):
        if verbose:
            Logging.debug(command)
        else:
            log_path = Logging._log_path()

        try:
            return subprocess.check_output(command, stderr=subprocess.STDOUT, shell=True)
        except subprocess.CalledProcessError as e:
            output = e.output
            message = 'error running cmd'

            if not verbose:
                with open(log_path, 'w') as f:
                    f.write(output)
                message += log_path
            else:
                Logging.error(output)

            raise YHError(message, YHError.ERROR_RUNNING_CMD)

    @staticmethod
    def convert_path_to_cmd(path):
        """ Escape paths which include spaces to correct style which bash(mac) and cmd(windows) can treat correctly.

            eg: on mac: convert '/usr/xxx/apache-ant 1.9.3' to '/usr/xxx/apache-ant\ 1.9.3'
            eg: on windows: convert '"c:\apache-ant 1.9.3"\bin' to '"c:\apache-ant 1.9.3\bin"'
        """
        ret = path
        if os_is_mac():
            ret = path.replace("\ ", " ").replace(" ", "\ ")

        if os_is_win32():
            ret = "\"%s\"" % (path.replace("\"", ""))

        # print("!!!!! Convert %s to %s\n" % (path, ret))
        return ret

    @staticmethod
    def convert_path_to_python(path):
        """ Escape paths which include spaces to correct style which python can treat correctly.

            eg: on mac: convert '/usr/xxx/apache-ant\ 1.9.3' to '/usr/xxx/apache-ant 1.9.3'
            eg: on windows: convert '"c:\apache-ant 1.9.3"\bin' to 'c:\apache-ant 1.9.3\bin'
        """
        ret = path
        if os_is_mac():
            ret = path.replace("\ ", " ")

        if os_is_win32():
            ret = ret.replace("\"", "")

        # print("!!!!! Convert %s to %s\n" % (path, ret))
        return ret

def get_current_path():
    if getattr(sys, 'frozen', None):
        ret = os.path.realpath(os.path.dirname(sys.executable))
    else:
        ret = os.path.realpath(os.path.dirname(__file__))

    return ret

# get_class from: http://stackoverflow.com/a/452981
def get_class(kls):
    parts = kls.split('.')
    module = ".".join(parts[:-1])
    if len(parts) == 1:
        m = sys.modules[__name__]
        m = getattr(m, parts[0])
    else:
        m = __import__(module)
        for comp in parts[1:]:
            m = getattr(m, comp)
    return m

# common functions

def check_environment_variable(var):
    ''' Checking the environment variable, if found then return it's value, else raise error
    '''
    try:
        value = os.environ[var]
    except Exception:
        raise YHError("%s env not defined" % (var),YHError.ERROR_ENV_VAR_NOT_FOUND)

    return value

def copy_files_in_dir(src, dst):
    for item in os.listdir(src):
        path = os.path.join(src, item)
        if os.path.isfile(path):
            path = add_path_prefix(path)
            copy_dst = add_path_prefix(dst)
            shutil.copy(path, copy_dst)
        if os.path.isdir(path):
            new_dst = os.path.join(dst, item)
            if not os.path.isdir(new_dst):
                os.makedirs(add_path_prefix(new_dst))
            copy_files_in_dir(path, new_dst)


def copy_files_with_config(config, src_root, dst_root):
    src_dir = config["from"]
    dst_dir = config["to"]

    src_dir = os.path.join(src_root, src_dir)
    dst_dir = os.path.join(dst_root, dst_dir)

    include_rules = None
    if "include" in config:
        include_rules = config["include"]
        include_rules = convert_rules(include_rules)

    exclude_rules = None
    if "exclude" in config:
        exclude_rules = config["exclude"]
        exclude_rules = convert_rules(exclude_rules)

    copy_files_with_rules(
        src_dir, src_dir, dst_dir, include_rules, exclude_rules)


def copy_files_with_rules(src_rootDir, src, dst, include=None, exclude=None):
    if os.path.isfile(src):
        if not os.path.exists(dst):
            os.makedirs(add_path_prefix(dst))

        copy_src = add_path_prefix(src)
        copy_dst = add_path_prefix(dst)
        shutil.copy(copy_src, copy_dst)
        return

    if (include is None) and (exclude is None):
        if not os.path.exists(dst):
            os.makedirs(add_path_prefix(dst))
        copy_files_in_dir(src, dst)
    elif (include is not None):
        # have include
        for name in os.listdir(src):
            abs_path = os.path.join(src, name)
            rel_path = os.path.relpath(abs_path, src_rootDir)
            if os.path.isdir(abs_path):
                sub_dst = os.path.join(dst, name)
                copy_files_with_rules(
                    src_rootDir, abs_path, sub_dst, include=include)
            elif os.path.isfile(abs_path):
                if _in_rules(rel_path, include):
                    if not os.path.exists(dst):
                        os.makedirs(add_path_prefix(dst))

                    abs_path = add_path_prefix(abs_path)
                    copy_dst = add_path_prefix(dst)
                    shutil.copy(abs_path, copy_dst)
    elif (exclude is not None):
        # have exclude
        for name in os.listdir(src):
            abs_path = os.path.join(src, name)
            rel_path = os.path.relpath(abs_path, src_rootDir)
            if os.path.isdir(abs_path):
                sub_dst = os.path.join(dst, name)
                copy_files_with_rules(
                    src_rootDir, abs_path, sub_dst, exclude=exclude)
            elif os.path.isfile(abs_path):
                if not _in_rules(rel_path, exclude):
                    if not os.path.exists(dst):
                        os.makedirs(add_path_prefix(dst))

                    abs_path = add_path_prefix(abs_path)
                    copy_dst = add_path_prefix(dst)
                    shutil.copy(abs_path, copy_dst)


def _in_rules(rel_path, rules):
    import re
    ret = False
    path_str = rel_path.replace("\\", "/")
    for rule in rules:
        if re.match(rule, path_str):
            ret = True

    return ret


def convert_rules(rules):
    ret_rules = []
    for rule in rules:
        ret = rule.replace('.', '\\.')
        ret = ret.replace('*', '.*')
        ret = "%s" % ret
        ret_rules.append(ret)

    return ret_rules


def os_is_win32():
    return sys.platform == 'win32'


def os_is_32bit_windows():
    if not os_is_win32():
        return False

    arch = os.environ['PROCESSOR_ARCHITECTURE'].lower()
    archw = "PROCESSOR_ARCHITEW6432" in os.environ
    return (arch == "x86" and not archw)


def os_is_mac():
    return sys.platform == 'darwin'


def os_is_linux():
    return 'linux' in sys.platform


def add_path_prefix(path_str):
    if not os_is_win32():
        return path_str

    if path_str.startswith("\\\\?\\"):
        return path_str

    ret = "\\\\?\\" + os.path.abspath(path_str)
    ret = ret.replace("/", "\\")
    return ret


# get from http://stackoverflow.com/questions/6194499/python-os-system-pushd
@contextmanager
def pushd(newDir):
    previousDir = os.getcwd()
    os.chdir(newDir)
    yield
    os.chdir(previousDir)

def rmdir(folder):
    if os.path.exists(folder):
        if sys.platform == 'win32':
            CMDRunner.run_cmd("rd /s/q \"%s\"" % folder, verbose=True)
        else:
            shutil.rmtree(folder)

def _check_python_version():
    major_ver = sys.version_info[0]
    minor_ver = sys.version_info[1]
    ret = True
    if major_ver != 2:
        ret = False
    elif minor_ver < 7:
        ret = False

    if not ret:
        print("The Python version is %d.%d. But Python 2.7 is required.\nDownload it here: https://www.python.org/" % (major_ver, minor_ver))

    return ret

def get_xcode_version():
    commands = [
        "xcodebuild",
        "-version"
    ]
    child = subprocess.Popen(commands, stdout=subprocess.PIPE)

    xcode = None
    version = None
    for line in child.stdout:
        if 'Xcode' in line:
            xcode, version = str.split(line, ' ')

    child.wait()

    if xcode is None:
        raise YHError('xcode not installed',YHError.ERROR_TOOLS_NOT_FOUND)

    return version

VS_VERSION_MAP = {
    2012 : "11.0",
    2013 : "12.0",
    2015 : "14.0"
}

def get_msbuild_path(vs_version):
    if cocos.os_is_win32():
        import _winreg
    else:
        return None

    if isinstance(vs_version, int):
        # The value of vs_version is int. such as : 2013, 2015
        if vs_version in VS_VERSION_MAP.keys():
            vs_ver = VS_VERSION_MAP[vs_version]
        else:
            # not supported VS version
            return None
    elif isinstance(vs_version, str):
        # The value of vs_version is string. such as: "12.0", "14.0"
        vs_ver = vs_version
    else:
        return None

    # If the system is 64bit, find VS in both 32bit & 64bit registry
    # If the system is 32bit, only find VS in 32bit registry
    if cocos.os_is_32bit_windows():
        reg_flag_list = [ _winreg.KEY_WOW64_32KEY ]
    else:
        reg_flag_list = [ _winreg.KEY_WOW64_64KEY, _winreg.KEY_WOW64_32KEY ]

    # Find VS path
    msbuild_path = None
    for reg_flag in reg_flag_list:
        try:
            vs = _winreg.OpenKey(
                _winreg.HKEY_LOCAL_MACHINE,
                r"SOFTWARE\Microsoft\MSBuild\ToolsVersions\%s" % vs_ver,
                0,
                _winreg.KEY_READ | reg_flag
            )
            msbuild_path, type = _winreg.QueryValueEx(vs, 'MSBuildToolsPath')
        except:
            continue

        if msbuild_path is not None:
            msbuild_path = os.path.join(msbuild_path, "MSBuild.exe")
            if os.path.exists(msbuild_path):
                break
            else:
                msbuild_path = None

    return msbuild_path

def get_devenv_path(vs_version):
    if cocos.os_is_win32():
        import _winreg
    else:
        return None

    if isinstance(vs_version, int):
        # The value of vs_version is int. such as : 2013, 2015
        if vs_version in VS_VERSION_MAP.keys():
            vs_ver = VS_VERSION_MAP[vs_version]
        else:
            # not supported VS version
            return None
    elif isinstance(vs_version, str):
        # The value of vs_version is string. such as: "12.0", "14.0"
        vs_ver = vs_version
    else:
        return None

    # If the system is 64bit, find VS in both 32bit & 64bit registry
    # If the system is 32bit, only find VS in 32bit registry
    if cocos.os_is_32bit_windows():
        reg_flag_list = [ _winreg.KEY_WOW64_32KEY ]
    else:
        reg_flag_list = [ _winreg.KEY_WOW64_64KEY, _winreg.KEY_WOW64_32KEY ]

    devenv_path = None
    for reg_flag in reg_flag_list:
        try:
            vs = _winreg.OpenKey(
                _winreg.HKEY_LOCAL_MACHINE,
                r"SOFTWARE\Microsoft\VisualStudio",
                0,
                _winreg.KEY_READ | reg_flag
            )
        except:
            continue

        # find specified VS
        try:
            key = _winreg.OpenKey(vs, r"SxS\VS7")
            devenv_path, type = _winreg.QueryValueEx(key, vs_ver)
        except:
            pass

        if devenv_path is not None:
            devenv_path = os.path.join(devenv_path, "Common7", "IDE", "devenv.com")
            if os.path.exists(devenv_path):
                break
            else:
                devenv_path = None

    return devenv_path

def get_vs_versions():
    # Get the VS versions
    ret = []
    if cocos.os_is_win32():
        import _winreg
    else:
        return ret

    # If the system is 64bit, find VS in both 32bit & 64bit registry
    # If the system is 32bit, only find VS in 32bit registry
    if cocos.os_is_32bit_windows():
        reg_flag_list = [ _winreg.KEY_WOW64_32KEY ]
    else:
        reg_flag_list = [ _winreg.KEY_WOW64_64KEY, _winreg.KEY_WOW64_32KEY ]

    version_pattern = re.compile(r'(\d+)\.(\d+)')
    for reg_flag in reg_flag_list:
        try:
            vs = _winreg.OpenKey(
                _winreg.HKEY_LOCAL_MACHINE,
                r"SOFTWARE\Microsoft\VisualStudio",
                0,
                _winreg.KEY_READ | reg_flag
            )
        except:
            continue

        i = 0
        while True:
            # enum the keys in vs reg
            try:
                version = _winreg.EnumKey(vs, i)
            except:
                break
            i += 1

            match = re.match(version_pattern, version)
            if match is None:
                continue

            ver_str = '%s.%s' % (match.group(1), match.group(2))
            if ver_str not in ret:
                ret.append(ver_str)

    return ret

def get_newest_msbuild(min_ver=None):
    versions = get_vs_versions()

    min_ver_float = 0.0
    if isinstance(min_ver, str):
        # value of min_ver is string. such as : "12.0", "14.0"
        min_ver_float = float(min_ver)
    elif isinstance(min_ver, int) and min_ver in VS_VERSION_MAP.keys():
        # value of min_ver is int. such as : 2013, 2015
        min_ver_float = float(VS_VERSION_MAP[min_ver])

    find_ver = None
    find_path = None
    for v in versions:
        cur_v = float(v)
        if cur_v < min_ver_float:
            continue

        v_path = get_msbuild_path(v)
        if v_path is not None:
            if (find_ver is None) or (cur_v > find_ver):
                find_ver = cur_v
                find_path = v_path

    return find_path

def get_newest_devenv(min_ver=None):
    versions = get_vs_versions()

    min_ver_float = 0.0
    if isinstance(min_ver, str):
        # value of min_ver is string. such as : "12.0", "14.0"
        min_ver_float = float(min_ver)
    elif isinstance(min_ver, int) and min_ver in VS_VERSION_MAP.keys():
        # value of min_ver is int. such as : 2013, 2015
        min_ver_float = float(VS_VERSION_MAP[min_ver])

    find_ver = None
    find_path = None
    for v in versions:
        cur_v = float(v)
        if cur_v < min_ver_float:
            continue

        v_path = get_devenv_path(v)
        if v_path is not None:
            if (find_ver is None) or (cur_v > find_ver):
                find_ver = cur_v
                find_path = v_path

    if min_ver_float > 0 and find_ver > min_ver_float:
        need_upgrade = True
    else:
        need_upgrade = False

    return (need_upgrade, find_path)
