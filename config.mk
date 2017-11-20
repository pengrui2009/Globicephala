
#V2X platform type definision(V2X_IMX6 / V2X_AR9341 / V2X_LTEV).
V2X_PLATFORM_TYPE = V2X_IMX6

#Set the cross compiler according to V2X platform type. 
ifeq ($(V2X_PLATFORM_TYPE), V2X_IMX6)

	#For IMX6 device.
	CROSS_COMPILE = arm-linux-gnueabihf-
	_ENDIAN       = ENDIAN_LITTLE

else ifeq ($(V2X_PLATFORM_TYPE), V2X_AR9341)

	#For AR9341 device.
	CROSS_COMPILE = mips-openwrt-linux-
 	_ENDIAN        = ENDIAN_BIG

else ifeq ($(V2X_PLATFORM_TYPE), V2X_LTEV)

	#For LTEV device.
	CROSS_COMPILE = arm-linux-gnueabi-
	_ENDIAN        = ENDIAN_LITTLE

endif

#THE type of J2735 protocal : J2735_CN or J2735_UN
J2735_PROTOCAL_TYPE	= J2735_CN

AS      = $(CROSS_COMPILE)as
LD      = $(CROSS_COMPILE)ld
CC      = $(CROSS_COMPILE)gcc
AR      = $(CROSS_COMPILE)ar
NM      = $(CROSS_COMPILE)nm
STRIP   = $(CROSS_COMPILE)strip
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
ECHO    = echo

CDEFS          = -pipe 

VNET_DRIVER_TYPE  = VNET_DRIVER_TYPE_LTE

#control the simu interface:VNET_SIMU_OPEN or VNET_SIMU_CLOSE
VNET_SIMU_STATU	= VNET_SIMU_OPEN

OSFLAG = -DLINUX -D_GNU_SOURCE -DCONFIG_LIBNL30 -D$(_ENDIAN) -D_BYTE_ORDER=$(_ENDIAN) -D$(VNET_DRIVER_TYPE) -D$(VNET_SIMU_STATU) -DASN_DISABLE_OER_SUPPORT -D__STDC_VERSION__=199901L -DASN_EMIT_DEBUG=0

ifeq ("$(release)", "y")
	OSFLAG += -D_NDEBUG
endif

INCLUDEFLAGS +=	-I$(TOPDIR)/include \
		-I$(TOPDIR)/osal \
		-I$(TOPDIR)/osal/linux \
		-I$(TOPDIR)/library/alsa/include \
		-I$(TOPDIR)/library/libnl/include/libnl3 \
		-I$(TOPDIR)/library/curl/include \
		-I$(TOPDIR)/drivers/audio \
		-I$(TOPDIR)/drivers/barometer \
		-I$(TOPDIR)/drivers/cmn \
		-I$(TOPDIR)/drivers/gps \
		-I$(TOPDIR)/drivers/gsensor \
		-I$(TOPDIR)/drivers/led \
		-I$(TOPDIR)/drivers/pps \
		-I$(TOPDIR)/drivers/tbox \
		-I$(TOPDIR)/drivers/vguest \
		-I$(TOPDIR)/drivers/vhost \
		-I$(TOPDIR)/drivers/vnet \
		-I$(TOPDIR)/protocols/ehmh \
		-I$(TOPDIR)/protocols/nmea0183 \
		-I$(TOPDIR)/protocols/ntrip \
		-I$(TOPDIR)/protocols/wsmp \
		-I$(TOPDIR)/protocols/trafficlamp
		 
ifeq ($(J2735_PROTOCAL_TYPE), J2735_UN)
INCLUDEFLAGS +=	-I$(TOPDIR)/protocols/J2735/SAE       \
		-I$(TOPDIR)/protocols/J2735/SAE/codec
else ifeq ($(J2735_PROTOCAL_TYPE), J2735_CN)
INCLUDEFLAGS +=	-I$(TOPDIR)/protocols/J2735/CSAE       \
		-I$(TOPDIR)/protocols/J2735/CSAE/codec
endif

WARNING = -Wall -Wundef -Wno-trigraphs -fno-strict-aliasing -fno-common
#-Werror-implicit-function-declaration
CFLAGS = -g3 -O2 -Werror $(OSFLAG) $(WARNING) $(INCLUDEFLAGS)

DEPENDENCIES = dependence


