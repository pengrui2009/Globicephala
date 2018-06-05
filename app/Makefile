include $(TOPDIR)/config.mk

TARGET := Globicephala

SUBDIRS += $(TOPDIR)/osal/linux
SUBDIRS += $(TOPDIR)/osal
SUBDIRS	+= $(TOPDIR)/protocol 
SUBDIRS += $(TOPDIR)/drivers/cmn

ifeq ($(VNET_DRIVER_TYPE), VNET_DRIVER_TYPE_WIFI)
    SUBDIRS += $(TOPDIR)/drivers/vnet/wifi
endif

ifeq ($(VNET_DRIVER_TYPE), VNET_DRIVER_TYPE_LTE)
    SUBDIRS += $(TOPDIR)/drivers/vnet/lte
endif
	SUBDIRS += $(TOPDIR)/drivers/vnet/simu
	
SUBDIRS += \
	$(TOPDIR)/drivers/tbox/uart \
	$(TOPDIR)/drivers/tbox/net \
	$(TOPDIR)/app/sys \
    $(TOPDIR)/app/ehm \
    $(TOPDIR)/app/vam \
    $(TOPDIR)/app/wnet \
    $(TOPDIR)/app/vsa \
    $(TOPDIR)/app/oam/cmn \
    $(TOPDIR)/app/oam/internal \
    $(TOPDIR)/app/oam/test \
    $(TOPDIR)/app/oam/test/internal \
    $(TOPDIR)/app/J2735 \
    $(TOPDIR)/app/msg 
    

OBJ_FILES=$(SUBDIRS:%=%/*.o)
LDFLAGS = -lm -lrt -pthread -L$(LIBDIR)/libnl/lib -lnl-3 -lnl-genl-3

.PHONY: all clean app install
all: app install 

app:
	@for subdir in $(SUBDIRS); do \
		(cd $$subdir && $(MAKE) all) || exit 1; \
	done

	@$(CC) $(OBJ_FILES) $(LDFLAGS) -o $(TARGET)
	#$(STRIP) --strip-unneeded  --remove-section=.comment --remove-section=.note $(TARGET)

install:
	cp -f $(TARGET) $(IMAGES_DIR)/

clean:
	-rm -rf $(TARGET)
	@for subdir in $(SUBDIRS); do \
		(cd $$subdir && $(MAKE) $@) || exit 1; \
	done
