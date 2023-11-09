# AutoGenerator
  
## 介绍  
  
这个程序是为了方便STM32CubeMX用户将由Cubemx生成的Makefile工程转化为VSCode工程而设计的。它可以帮助您快速转换项目配置，以便在VSCode中进行开发。  
  
## 使用说明  
  
1. 下载并安装STM32CubeMX软件。  
2. 使用STM32CubeMX创建并配置您的STM32项目，并生成Makefile工程。  
3. 下载并安装VSCode软件。  
4. 打开VSCode，并安装C/C++插件（Extensions -> Search "C/C++" -> Install）。  
5. 下载本程序，并将其保存到您的计算机上，最好添加到用户变量Path中，否则您每次使用都必须复制本程序到工程根目录下。    
6. 在工程根目录，打开powerShell或者Cmd执行AutoGenerator.exe，程序会自动生成.vscode等一系列配置目录。    
7. 转换完成后，您可以在VSCode中打开生成的工程目录，并开始编写代码。
8. 点击菜单栏终端->运行任务->编译并下载  
  
注意：在转换过程中，可能会因项目配置和依赖关系的不同而遇到问题。如果遇到问题，请检查您的项目配置和依赖是否正确，并尝试手动解决。本程序仅提供转换功能，不保证完全解决所有问题。此外编译和下载功能需要自行安装gcc编译器和OpenOcd。 
  
## 示例工程目录结构  
  
转换后的VSCode工程目录结构将如下所示： 
```
* ./.vscode/            // VSCode配置文件夹  
	* ./settings.json   // VSCode配置文件  
	* ./launch.json     // VSCode调试配置文件  
	* ./tasks.json      // VSCode任务配置文件  
	* ./c_cpp_properties.json // C/C++语言配置文件
```
