CROSS_COMPILE=mips-openwrt-linux-

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
_ENDIAN        = __BIG_ENDIAN

VNET_DRIVER_TYPE  = VNET_DRIVER_TYPE_WIFI

#control the simu interface:VNET_SIMU_OPEN or VNET_SIMU_CLOSE
VNET_SIMU_STATU	= VNET_SIMU_OPEN

OSFLAG = -DLINUX -D_GNU_SOURCE -DCONFIG_LIBNL30 -D$(_ENDIAN) -D_BYTE_ORDER=$(_ENDIAN) -D$(VNET_DRIVER_TYPE) -D$(VNET_SIMU_STATU)

ifeq ("$(release)", "y")
	OSFLAG += -D_NDEBUG
endif


INCLUDEFLAGS += -I$(TOPDIR)/include \
		-I$(TOPDIR)/osal/ \
		-I$(TOPDIR)/osal/linux \
		-I$(TOPDIR)/utils/ \
		-I$(TOPDIR)/library/libnl/include/libnl3/ \
		-I$(TOPDIR)/library/readline/include/ \
		-I$(TOPDIR)/drivers/vnet \
		-I$(TOPDIR)/drivers/vnet/simu \
		-I$(TOPDIR)/drivers/vnet/wifi \
		-I$(TOPDIR)/drivers/vnet/lte \
		-I$(TOPDIR)/app/oam/agent/cli/common \
		-I$(TOPDIR)/app/oam/agent/cli/lib \
		-I$(TOPDIR)/app/oam/agent/cli/main \
		-I$(TOPDIR)/app/oam/cmn \
		-I$(TOPDIR)/app/oam/external \
		-I$(TOPDIR)/app/oam/internal \
		-I$(TOPDIR)/app/oam/test \
		-I$(TOPDIR)/app/J2735

WARNING = -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common
#-Werror-implicit-function-declaration
CFLAGS = -g3 -O2 -Werror $(OSFLAG) $(WARNING) $(INCLUDEFLAGS)

DEPENDENCIES = dependence


