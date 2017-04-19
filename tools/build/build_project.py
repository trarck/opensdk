#!/usr/bin/python
# ----------------------------------------------------------------------------
# ----------------------------------------------------------------------------
'''
'''

__docformat__ = 'restructuredtext'

import multiprocessing
import os
import re
import sys
import shutil
import json
import utils

class Platforms(object):
    ANDROID = 'android'
    IOS = 'ios'
    MAC = 'mac'
    WEB = 'web'
    WIN32 = 'win32'
    LINUX = 'linux'
    WP8 = "wp8"
    WP8_1 = "wp8_1"
    METRO = "metro"

class BuildProject:
    """
    compiles a project
    """

    BUILD_CONFIG_FILE = "build-cfg.json"
    CFG_KEY_WIN32_COPY_FILES = "copy_files"
    CFG_KEY_WIN32_MUST_COPY_FILES = "must_copy_files"

    CFG_KEY_COPY_RESOURCES = "copy_resources"
    CFG_KEY_MUST_COPY_RESOURCES = "must_copy_resources"

    OUTPUT_DIR_NATIVE = "bin"
    OUTPUT_DIR_SCRIPT_DEBUG = "simulator"
    OUTPUT_DIR_SCRIPT_RELEASE = "publish"
    WEB_PLATFORM_FOLDER_NAME = "html5"

    PROJ_CFG_KEY_IOS_SIGN_ID = "ios_sign_id"
    PROJ_CFG_KEY_ENGINE_DIR = "engine_dir"

    BACKUP_SUFFIX = "-backup"
    
    CUSTOM_STEP_PRE_BUILD        = "pre-build"
    CUSTOM_STEP_POST_BUILD       = "post-build"
    CUSTOM_STEP_PRE_NDK_BUILD    = "pre-ndk-build"
    CUSTOM_STEP_POST_NDK_BUILD   = "post-ndk-build"
    CUSTOM_STEP_PRE_COPY_ASSETS  = "pre-copy-assets"
    CUSTOM_STEP_POST_COPY_ASSETS = "post-copy-assets"
    CUSTOM_STEP_PRE_ANT_BUILD    = "pre-ant-build"
    CUSTOM_STEP_POST_ANT_BUILD   = "post-ant-build"
    