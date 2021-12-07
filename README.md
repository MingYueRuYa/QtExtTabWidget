## 这个仓库主要记录自己在工作中能够提升效率的工具集合

### compile.py
这个脚本的主要目的就是能够在命令行自动编译程序，杀掉程序，启动程序，更新代码。
可以帮我节省几分钟的时间。剩下的这几分钟时间，可以让我思考刚才的修改的代码有没有问题。
这个脚本通过`config.json`配置一些属性

| 支持功能           |
| ------------------ |
| 命令行自动编译程序 |
| 杀掉程序           |
| 启动程序           |
| 更新代码           |
| 支持命令行字体颜色高亮          |

**用法：**

```bat
PS D:\code>.\compile.py -h
usage: compile.py [-h] [-u] [-pre] [-k] [-s] [-c] [-a COMPILEARGS] [-p CONFIGPATH]

Tools for auto update code and compile project

optional arguments:
  -h, --help            show this help message and exit
  -u, --update          update code by svn or git
  -pre, --precompileaction
                        build code previous action,for example Qt qrc file.
  -k, --kill            kill target process
  -s, --start           start target process
  -c, --compile         compile project
  -a COMPILEARGS, --compileargs COMPILEARGS
                        compile project with args, for example:"Debug|Win32" or "Release|Win32". default value: "Debug|Win32"
  -p CONFIGPATH, --configpath CONFIGPATH
                        load config json path
```

**config.json  **配置文件可以在命令行指定，默认就是当前的路径下

```json
{
	# 进程的名称
	"process_name":"demo.exe",
	# 更新代码的命令,比如: git pull origin master
	"update_code_command":"svn up",
	# 编译之前的动作，因为使用的是Qt，所以需要先编译资源，可以放到bat文件执行
	"pre_compile_command":"E:\xxx.bat",
	# 解决方案的名称
	"compile_file":"D:/code/Demo.sln",
	# IDE的路径，这样就不用修改系统环境变量
	"compile_tool_dir":"C:/Program Files (x86)/Microsoft Visual Studio 12.0/Common7/IDE",
	# 启动进程的路径
	"start_process_path":"D:/code/bin/Debug/Demo.exe",
	# 启动进程的参数
	"start_process_args":["-enable", "-start"]
}
```



TODO list:

- [ ] 未能识别编译错误，给出提示
- [ ] build resource 不能叫着名字，给人造成歧义

