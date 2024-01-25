'''
Author        : Retuze 
Date          : 2023-11-05 17:19:53
LastEditors   : Retuze 
LastEditTime  : 2024-01-11 04:03:53
Description   : 通过cubemx生成的Makefile生成一个适合vscode的工程,需要提前安装arm-none-eabi-gcc,STM32CubeMX
'''
import os
import shutil
import glob
from os import path
def get_real_path(file):
    return path.abspath(path.join(path.dirname(__file__), file))
def getcwd_by_name(name):
    for path in os.environ.get("path").split(";"):
        if glob.glob(path+f"\\{name}"):
            flag = 1
            return glob.glob(path+f"\\{name}")[0]
    if flag != 1:
        print(f"error :{name} 没有找到")
        exit(-1)

def init_file():
    if not os.path.exists("Makefile"):
        print("Makefile不存在,请先用CubeMX生成Makefile工程")
        exit(-1)
    # 遍历当前路径寻找链接脚本
    ld = None
    LDSCRIPT_LIST = glob.glob(r"./**/*.ld", recursive=True)
    if LDSCRIPT_LIST:
        for LDSCRIPT in LDSCRIPT_LIST:
            ld = LDSCRIPT
    else:
        print("No matching .ld files found.")
        exit(-1)
    shutil.copyfile(ld,"ldscript.ld")

    if not os.path.exists("docs"):
        os.mkdir("docs")
        with open("docs/readme.md", "w" , encoding="utf-8") as f:
            f.write("# hello")
    if not os.path.exists("components"):
        shutil.copytree(get_real_path('template/components'), 'components')
    if os.path.exists('.vscode'):
        shutil.rmtree(".vscode")
    os.mkdir(".vscode")
    class Info:
        cpu_define = ''
        openocd_cpu_model = ''
        target = ''
    with open('Makefile', 'r', encoding="utf-8") as f:
        data = f.read().replace("\\\n", " ")  # 读取文本,把文本中的换行符去掉
        lines = data.splitlines()  # 按行分割
        # 遍历所有的行
        for line in lines:
            # 拿到 cpu_define
            if not line.find("C_DEFS =") == -1:
                Info.cpu_define = line.split()[3][2:]
                Info.openocd_cpu_model = line.split()[3][2:9].lower()+"x.cfg"
            if not line.find("TARGET =") == -1:
                Info.target = line[9:]
    with open(get_real_path('template/launch.json.template'), 'r', encoding="utf-8") as f:
        launch_str = f.read()
    with open('.vscode/launch.json', "w", encoding="utf-8") as f:
        launch_str = launch_str.replace(
            "#interface.cfg#", f'{getcwd_by_name("openocd.exe")[:-16]}/share/openocd/scripts/interface/cmsis-dap.cfg')
        launch_str = launch_str.replace(
            "#target.cfg#", f'{getcwd_by_name("openocd.exe")[:-16]}/share/openocd/scripts/target/{Info.openocd_cpu_model}')
        launch_str = launch_str.replace("\\", '/')
        f.write(launch_str)
    with open(get_real_path('template/c_cpp_properties.json.template'), "r", encoding="utf-8") as f:
        cpp_str = f.read()
    with open('.vscode/c_cpp_properties.json', "w", encoding="utf-8") as f:
        cpp_str = cpp_str.replace("#cpu_define#", Info.cpu_define)
        cpp_str = cpp_str.replace(
            "#compiler_path#", getcwd_by_name('arm-none-eabi-gcc.exe'))
        cpp_str = cpp_str.replace("\\", '/')
        f.write(cpp_str)
    with open(get_real_path('template/tasks.json.template'), "r", encoding="utf-8") as f:
        tasks_str = f.read()
    with open('.vscode/tasks.json', "w", encoding="utf-8") as f:
        tasks_str = tasks_str.replace(
            "#interface.cfg#", f'{getcwd_by_name("openocd.exe")[:-16]}/share/openocd/scripts/interface/cmsis-dap.cfg')
        tasks_str = tasks_str.replace(
            "#target.cfg#", f'{getcwd_by_name("openocd.exe")[:-16]}/share/openocd/scripts/target/{Info.openocd_cpu_model}')
        tasks_str = tasks_str.replace('#target#',Info.target)
        tasks_str = tasks_str.replace("\\", '/')
        f.write(tasks_str)
    with open(get_real_path('template/Build-Analyzer.py'), "r", encoding="utf-8") as f:
        build_analyzer_str = f.read()
    with open('Build-Analyzer.py', "w", encoding="utf-8") as f:
        f.write(build_analyzer_str)
def edit_mk():
    def extract_text(start_string, end_string):
        with open("Makefile", 'r',encoding="utf-8") as f:
            text = f.read()

        start_index = text.find(start_string)
        end_index = text.find(end_string)

        if start_index != -1 and end_index != -1:
            return text[start_index + len(start_string):end_index]
        else:
            return "未找到指定的字符串"
    str1 = extract_text("# target","# source")
    str2 = extract_text("# CFLAGS","# AS includes")
    str3 = extract_text("# link script","# libraries")
    with open(get_real_path('template/makefile.template'), "r", encoding="utf-8") as f:
        make_data = f.read()
    with open('mk.mk', "w", encoding="utf-8") as f:
        make_data = make_data.replace("#str1#",str1)
        make_data = make_data.replace("#str2#",str2)
        make_data = make_data.replace("#str3#",str3)
        f.write(make_data)
init_file()
edit_mk()

