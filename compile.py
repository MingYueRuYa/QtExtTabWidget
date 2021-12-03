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
import json

'''
- √ 配置json数据 配置文件可以指定路径再加载
- √ 编译处理，Debug|Release
- √ 能够杀掉进程，考虑多进程的问题
- √ 资源编译
'''


class App:
    def __init__(self):
        pass

    def _search_process(self):
        list_process_id = []
        for proc in psutil.process_iter(['name', 'pid']):
            if proc.info['name'] == self.process_name:
                find_flag = 1
                list_process_id.append(proc.info['pid'])
                break
        if len(list_process_id) == 0:
            print(colorama.Fore.GREEN + "Not find target process")
        return list_process_id


    def _kill_process(self, list_process_id):
        for process_id in list_process_id:
            target_process = psutil.Process(process_id)
            try:
                target_process.kill()
                print(colorama.Fore.GREEN + "kill target process successful.")
            except psutil.ZombieProcess:
                print(colorama.Fore.RED + "Error: this is zombie process")
                return 1
            except psutil.AccessDenied:
                print(colorama.Fore.RED + "Error: Access denied")
                return 1
            except psutil.TimeoutExpired:
                print(colorama.Fore.RED + "Error: Time out expired")
                return 1
        return 0


    def _pre_compile_code(self):
        # json配置
        os.system(self.build_resource)
        print(colorama.Fore.GREEN + "build resource finished.")
        return

    def _compile_code(self):
        compile_tool = 'devenv'
        compile_parameter = '/build "' + self.compile_args + '"'
        os.system('devenv ' + self.compile_file + ' ' + compile_parameter)

    def _pre_env(self):
        python_dir = "C:/"
        strewn = os.getenv("path")
        os.putenv("path", python_dir + ";" + strewn + ";" + self.compile_tool_dir)
        return

    def _start_process(self):
        try:
            list_process_args = [self.start_process_path]
            for args in self.start_process_args:
                list_process_args.append(args)
            subprocess.Popen(list_process_args, shell=False)
            print(colorama.Fore.GREEN + "start target process successful.")
        except Exception as error:
            print(colorama.Fore.GREEN + str(error))
        else:
            pass
        return

    def _update_code(self):
        os.system(self.update_code_command)
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
        parser.add_argument('-p', '--configpath', default='./config.json', type=str, help='load config json path')
        args = parser.parse_args(args)
        self.update = args.update
        self.resource = args.resource
        self.is_kill = args.kill
        self.is_start = args.start
        self.is_compile = args.compile
        self.compile_args = args.compileargs
        self.config_path = args.configpath

    def _parse_config(self, config_path):
        with open(config_path) as f:
            data = json.load(f)
        self.process_name = data['process_name']
        self.update_code_command = data['update_code_command']
        self.build_resource = data['build_resource']
        self.compile_file = data['compile_file']
        self.compile_tool_dir = data['compile_tool_dir']
        self.start_process_path = data['start_process_path']
        self.start_process_args = data['start_process_args']


    def run(self, args):
        self._parse_args(args)
        self._parse_config(self.config_path)

        if self.update:
            print('------------------------start update code----------------------------')
            self._update_code()
            print('------------------------end update code------------------------------')
        if self.resource:
            print('-------------------start previous action--------------------------')
            self._pre_compile_code()
            print('--------------------end previous action---------------------------')

        kill_flag = 0
        if self.is_kill:
            print('--------------------start search process--------------------------')
            list_process_id = self._search_process()
            if len(list_process_id) != 0:
                kill_flag = self._kill_process(list_process_id)
            print('---------------------end search process---------------------------')
        if kill_flag != 0:
            print('--------------------stop compile project--------------------------')
        else:
            if self.is_compile:
                print('--------------------start compile project--------------------------')
                
                print('--------------start init previous environment--------------------')
                self._pre_env()
                print('---------------end init previous environment---------------------')

                self._compile_code()
                print(colorama.Fore.GREEN + 'compile project finished')
                print('--------------------end compile project----------------------------')

            if self.is_start:
                print('--------------------start call application-------------------------')
                self._start_process()
                print('--------------------end call application---------------------------')


if __name__ == '__main__':
    colorama.init(autoreset=True)
    app = App()
    if not app.run(sys.argv[1:]):
        sys.exit(-1)
    sys.exit(0)
