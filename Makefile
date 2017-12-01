include ./config.mk

export TOPDIR = $(shell pwd)
export LIBDIR = $(TOPDIR)/library
export IMAGEDIR = $(TOPDIR)/image

NOW = `date +"%Y%m%d"`
#DEVICENAME = `echo $(DEVICEDIR)|tr a-z A-Z`
DEVICENAME = CARSMART_DEVICE
MAJOR_VER = `grep MAJOR $(TOPDIR)/include/version.h | awk '{print $$3}'  | sed -e 's/(//'  -e 's/)//'`
MINOR_VER = `grep MINOR $(TOPDIR)/include/version.h | awk '{print $$3}'  | sed -e 's/(//'  -e 's/)//'`
BUILD_VER = `grep BUILD $(TOPDIR)/include/version.h | awk '{print $$3}'  | sed -e 's/(//'  -e 's/)//'`
VER=$(MAJOR_VER).$(MINOR_VER).$(BUILD_VER)
IMGNAME=$(DEVICENAME)_$(VER)_$(NOW)

SUBDIRS = \
          $(TOPDIR)/osal/linux \
	  $(TOPDIR)/osal \
	  $(TOPDIR)/drivers \
	  $(TOPDIR)/drivers/audio \
	  $(TOPDIR)/drivers/barometer \
	  $(TOPDIR)/drivers/cmn \
	  $(TOPDIR)/drivers/gps \
	  $(TOPDIR)/drivers/led \
	  $(TOPDIR)/drivers/ltev \
	  $(TOPDIR)/drivers/pps \
	  $(TOPDIR)/drivers/rtc \
	  $(TOPDIR)/drivers/vguest \
	  $(TOPDIR)/drivers/vhost \
	  $(TOPDIR)/drivers/vnet \
	  $(TOPDIR)/drivers/weather \
	  $(TOPDIR)/protocols/cmn \
	  $(TOPDIR)/protocols/ehmh \
	  $(TOPDIR)/protocols/nmea0183 \
	  $(TOPDIR)/protocols/ntrip \
	  $(TOPDIR)/protocols/wsmp \
	  $(TOPDIR)/protocols/trafficlamp \
	  $(TOPDIR)/app

ifeq ($(J2735_PROTOCAL_TYPE), J2735_UN)
	SUBDIRS +=$(TOPDIR)/protocols/J2735/SAE $(TOPDIR)/protocols/J2735/SAE/codec
else ifeq ($(J2735_PROTOCAL_TYPE), J2735_CN)
	SUBDIRS +=$(TOPDIR)/protocols/J2735/CSAE $(TOPDIR)/protocols/J2735/CSAE/codec
endif

OBJ_FILES = $(SUBDIRS:%=%/*.o)
TARGET := ProjectExe

#Include the device library according to platform type.
ifeq ($(V2X_PLATFORM_TYPE), V2X_IMX6)

	#Include IMX6 device library.
	LDFLAGS = -lrt -pthread -L$(LIBDIR)/libnl/lib-imx6 -L$(LIBDIR)/curl/lib -L$(LIBDIR)/alsa/lib -lnl-3 -lnl-genl-3 -lcurl -lasound -ldl -lm -static

else ifeq ($(V2X_PLATFORM_TYPE), V2X_AR9341)

	#Include AR9341 device library. Compile error when add -static. have no reason.
	LDFLAGS = -lm -lrt -pthread -L$(LIBDIR)/libnl/lib-ar9341 -lnl-3 -lnl-genl-3
endif

.PHONEY: all clean image install 

all:
	sh ./format_file.sh
	@for subdir in $(SUBDIRS); do \
		(cd $$subdir && $(MAKE) all) || exit 1; \
	done
	$(CC) $(OBJ_FILES) $(LDFLAGS) -o $(TARGET)
	$(STRIP) --strip-unneeded  --remove-section=.comment --remove-section=.note $(TARGET)
	
	mv $(TARGET) $(IMAGEDIR)
clean:
	@for subdir in $(SUBDIRS); do \
		(cd $$subdir && $(MAKE) $@) || exit 1; \
	done
	
	rm $(IMAGEDIR)/* -rf
