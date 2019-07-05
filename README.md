# MotorDriver

## 初次Clone

### 1、添加STM32F10X标准外设库

本项目使用STM32标准外设库STM32F10x_StdPeriph_Lib_V3.5.0。

NTFS分区下，可以使用符号链接来完成。复制粘贴link copy.bat并重命名为link.bat。右键编辑Link.bat，将X:\xxx\Libraries修改为本地外设库中Libraries文件夹的目录。

FAT32或其他分区不支持符号链接，直接复制外设库中的Libraries文件夹到项目中即可。

所需外设库大致目录结构如下：

Libraries
- CMSIS
	- CM3
		- CoreSupport
		- DeviceSupport
- STM32F10x_StdPeriph_Driver
	- inc
	- src

完成修改后运行link.bat即可完成外设库的链接。

### 2、使用vscode的设置

在.vscode文件夹下复制粘贴c_cpp_properties copy.json并重命名为c_cpp_properties.json，修改"compilerPath"参数为本地gcc编译器。

Create by ChnTamap 2019.7.5
