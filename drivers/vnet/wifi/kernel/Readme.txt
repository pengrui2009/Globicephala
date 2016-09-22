Ath9000无线网卡驱动说明：
    因内核3.0不支持9300系列网卡，需要修改内核，比较麻烦。故直接使用compat开源驱动代码，因为最新的4.1, 4.2版本有bug编译运行后出现问题，
目前使用的时4.0.1-1版本backports-4.0.1-1。
一、编译：
1、make defconfig-ath9k-debug
2、make menuconfig

3、make KLIB=/mnt/hgfs/myzr/linux-3.0.35 KLIB_BUILD=/mnt/hgfs/myzr/linux-3.0.35
4、./myinstall.sh  //拷贝所有ko到./_install目录

二、代码修改
1、功率修改

2、
