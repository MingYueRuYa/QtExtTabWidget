#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @Date    : 2021-11-28 17:45:33
# @Author  : shixiong.Liu (liushixiong@corp.netease.com)

import os
import subprocess

'''
- 配置json数据
- 编译处理，Debug|Release
- 能够杀掉进程
- 资源编译
'''


def compile_code():
    compile_tool_dir = "C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE"
    strenv = os.getenv("path")
    os.putenv("path", strenv+";" + compile_tool_dir)
    print(os.getenv("path"))
    # print(os.environ["VS120COMNTOOLS"])
    # compile_tool = "C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE/devenv.exe"
    # compile_tool = "C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE/devenv.com"
    compile_tool = 'devenv'

    compile_parameter = '/build \"Debug|Win32\"'
    compile_file = 'D:/code/HearthstoneBox/HearthstoneBox.sln'
    # os.system('cmd &&  call "C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/vcvars32.bat" && devenv /?')
    # os.system('"C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE/devenv.exe" /HearthstoneBox/HearthstoneBox.sln /Build \"Debug|Win32\"')
    # os.system('"C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE/devenv.exe" /Build "Debug|Win32" D:/code/HearthstoneBox/HearthstoneBox.sln ')
    # subprocess.run(args=[compile_tool, compile_file, compile_parameter], shell=False, stdout=subprocess.PIPE,
    #               stderr=subprocess.PIPE)

    # subprocess.run(args=[compile_tool, compile_file, compile_parameter], shell=True, stdout=subprocess.PIPE,
    #                 stderr=subprocess.PIPE, check=True)
    # subprocess.run(args=["dir"], shell=True)

    # os.system('D:\\code\\update_code.bat')

    os.system('devenv ' + compile_file + ' ' + compile_parameter)


    # temp = "devenv /?"
    # p = subprocess.Popen('cmd', stdin=subprocess.PIPE,
    #                      stdout=subprocess.PIPE, stderr=subprocess.PIPE, shell=True)
    # cmds = ["C:/Program Files (x86)/Microsoft Visual Studio 12.0/VC/bin/vcvars32.bat", 'D:\\code\\update_code.bat']
    # for cmd in cmds:
    #     p.stdin.write((cmd + "\n").encode('utf-8'))
    # p.stdin.close()
    # print(p.stdout.read())


# import win32process, win32event

# def CreateMyProcess2(cmd):
#     ''' create process width no window that runs a command with arguments
#     and returns the process handle'''
#     si   = win32process.STARTUPINFO()
#     info = win32process.CreateProcess(
#         None,      # AppName
#         cmd,       # Command line
#         None,      # Process Security
#         None,      # Thread Security
#         0,         # inherit Handles?
#         win32process.NORMAL_PRIORITY_CLASS,
#         None,      # New environment
#         None,      # Current directory
#         si)        # startup info
#     # info is tuple (hProcess, hThread, processId, threadId)
#     return info[0]

# def test():
#     p = subprocess.Popen('cmd.exe', stdin=subprocess.PIPE,
#                          stdout=subprocess.PIPE, stderr=subprocess.PIPE)
#     cmds = ["dir"]
#     for cmd in cmds:
#         p.stdin.write((cmd + "\n").encode('utf-8'))
#     p.stdin.close()
#     print(str(p.stdout.read(),'utf-8'))

if __name__ == '__main__':
    compile_code()
    ''' create/run a process for each list element in "cmds"
    output may be out of order because processes run concurrently '''

    # cmds=["echo my","echo heart","echo belongs","echo to","echo daddy"]
    # handles    = []
    # for i in range(len(cmds)):
    #     cmd    = 'cmd /c ' + cmds[i]
    #     handle = CreateMyProcess2(cmd)
    #     handles.append(handle)
    #
    # rc = win32event.WaitForMultipleObjects( handles, 1, -1)  # 1 wait for all, -1 wait infinite
    # # print 'return code ',rc

