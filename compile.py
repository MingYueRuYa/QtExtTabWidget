#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2021-11-28 17:45:33
# @Author  : shixiong.Liu (liushixiong@corp.netease.com)

import os
import psutil
import subprocess
import sys
import argparse
import colorama

'''
- 配置json数据 配置文件可以指定路径再加载
- √ 编译处理，Debug|Release
- 能够杀掉进程，考虑多进程的问题
- √ 资源编译
'''

process_name = 'HearthstoneOfficialAddon.exe'


class App:
    def __init__(self):
        pass

    def _search_process(self, process_name):
        find_flag = 0
        process_id = 0
        for proc in psutil.process_iter(['name', 'pid']):
            if proc.info['name'] == process_name:
                find_flag = 1
                process_id = proc.info['pid']
                break
        if find_flag == 0:
            print(colorama.Fore.GREEN + "Not find target process")

        return find_flag, process_id

    def _kill_process(self, process_id):
        target_process = psutil.Process(process_id)
        try:
            target_process.kill()
            print(colorama.Fore.GREEN + "kill target process successful.")
        except psutil.ZombieProcess:
            print(colorama.Fore.RED + "Error: this is zombie process")
        except psutil.AccessDenied:
            print(colorama.Fore.RED + "Error: Access denied")
        except psutil.TimeoutExpired:
            print(colorama.Fore.RED + "Error: Time out expired")
        else:
            return 0
        return 1

    def _pre_compile_code(self):
        # json配置
        os.system("D:/code/Tools/Publish/Scripts/generate_run_env.bat")
        print(colorama.Fore.GREEN + "compile finished.")
        return

    def _compile_code(self):
        compile_tool = 'devenv'
        compile_parameter = '/build "' + self.compile_args + '"'
        compile_file = 'D:/code/HearthstoneBox/HearthstoneBox.sln'
        os.system('devenv ' + compile_file + ' ' + compile_parameter)

    def _pre_env(self):
        python_dir = "C:/"
        compile_tool_dir = "C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE"
        strewn = os.getenv("path")
        os.putenv("path", python_dir + ";" + strewn + ";" + compile_tool_dir)
        return

    def _start_process(self):
        app_path = "D:/code/bin/Debug/HearthstoneOfficialAddon.exe"
        try:
            subprocess.Popen([app_path, '-qa', 'devdebug'], shell=False)
            print(colorama.Fore.GREEN + "start target process successful.")
        except Exception as error:
            print(str(error))
        else:
            pass
        return

    def _update_code(self):
        os.system("svn up")
        return

    def _parse_args(self, args):
        parser = argparse.ArgumentParser(description='Tools for auto update code and compile project')
        parser.add_argument('-u', '--update', action='store_true', help='update code by svn or git')
        parser.add_argument('-r', '--resource', action='store_true', help='rebuild resource:for example Qt qrc file.')
        parser.add_argument('-k', '--kill', action='store_true', help='kill target process')
        parser.add_argument('-s', '--start', action='store_true', help='start target process')
        parser.add_argument('-c', '--compile', action='store_true', help='compile project')
        parser.add_argument('-a', '--compileargs', default='Debug|Win32', type=str,
                            help='compile project with args, for example:"Debug|Win32". default value: "Debug|Win32"')
        args = parser.parse_args(args)
        self.update = args.update
        self.resource = args.resource
        self.is_kill = args.kill
        self.is_start = args.start
        self.is_compile = args.compile
        self.compile_args = args.compileargs


    def run(self, args):
        self._parse_args(args)

        if self.update:
            print('------------------------start update code----------------------------')
            self._update_code()
            print('------------------------end update code------------------------------')

        print('------------------------start init previous environment------------------------------')
        self._pre_env()
        print('------------------------end init previous environment------------------------------')

        if self.resource:
            print('------------------------start previous action------------------------------')
            self._pre_compile_code()
            print('------------------------end previous action------------------------------')

        kill_flag = 0
        if self.is_kill:
            print('------------------------start search process------------------------------')
            find_flag, process_id = self._search_process(process_name)
            print('------------------------end search process------------------------------')
            if find_flag == 1:
                kill_flag = self._kill_process(process_id)
        if kill_flag != 0:
            print('------------------------stop compile project------------------------------')
        else:
            if self.is_compile:
                print('------------------------start compile project------------------------------')
                self._compile_code()
                print('------------------------end compile project------------------------------')
            if self.is_start:
                print('------------------------start call application------------------------------')
                self._start_process()
                print('------------------------end call application------------------------------')


if __name__ == '__main__':
    colorama.init(autoreset=True)
    app = App()
    if not app.run(sys.argv[1:]):
        sys.exit(-1)
    sys.exit(0)
