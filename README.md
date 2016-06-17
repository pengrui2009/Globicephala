#  M03模块固件程序
##硬件平台:
###   高通AR9341/QCA9531

##系统平台:
###   openwrt

##工程代号：
###   Globicephala

##编译说明:
###   待补充

##源码说明：
###app： 应用程序子目录，用于存放系统的应用层程序源文件，主要为C源文件，包括VANET
中间件模块、无线网络通信模块、车辆安全应用模块、系统及管理模块等。
oam： agent：管理控制客户端代码，是独立的进程，包括cli进程，和后续要开发的web进程。
images： 应用程序镜像文件子目录，用于存放生成的应用程序文件和相关启动脚本。
drivers： 设备驱动层子目录，用于存放系统的接口与设备驱动源文件，主要为C源文件，
包括Mac收发驱动等。 这部分驱动指的是用户态设备驱动，接口驱动linux内核态完成。
drivers/vnet： 分两部分：WiFi、LTE等应用层驱动。
osal： 操作系统及公共功能子目录，用于存放操作系统及相关公共功能模块（如动态内存
管理、调试支持等），包括C源文件。
include： 头文件子目录，用于存放系统的公用头文件，主要为H源文件。
library： 库子目录，用于系统提供的库及相应头文件，主要为.a 和.so文件。
prj_si： 程序项目文件子目录，用于存放source insight工程文件等。
tools： 系统工具子目录，用于存放系统所需的软件工具，如编译控制、代码走查。

