'''
Author        : Retuze 
Date          : 2024-01-11 04:04:52
LastEditors   : Retuze 
LastEditTime  : 2024-01-11 04:28:24
Description   : 
'''
import os
import shutil

os.system("pyinstaller AutoGenerator.spec")

if os.path.exists(r"C:\Users\Retuze\App\project\AutoGenerator.exe"):
    os.remove(r"C:\Users\Retuze\App\project\AutoGenerator.exe")
    print("remove OK1 !")
if os.path.exists(r"C:\Users\Retuze\App\project\AG.exe"):
    os.remove(r"C:\Users\Retuze\App\project\AG.exe")
    print("remove OK2 !")
shutil.copyfile("dist/AutoGenerator.exe",r"C:\Users\Retuze\App\project\AutoGenerator.exe")
print("copy OK1 !")
shutil.copyfile("dist/AutoGenerator.exe",r"C:\Users\Retuze\App\project\AG.exe")
print("copy OK2 !")