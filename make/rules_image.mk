
#######################
### 
#######################

IMAGE_AXF = $(PUBLIC_BIN_PATH)/$(IMAGE_PREFIX).axf
IMAGE_BIN = $(PUBLIC_BIN_PATH)/$(IMAGE_PREFIX).bin
IMAGE_LST = $(PUBLIC_BIN_PATH)/$(IMAGE_PREFIX).list
IMAGE_SYM = $(PUBLIC_BIN_PATH)/$(IMAGE_PREFIX).symbol
IMAGE_ASM = $(PUBLIC_BIN_PATH)/$(IMAGE_PREFIX).s

ifeq ($(MAKECMDGOALS),all)
#LIBS = $(shell cat $(LINKER_LIBS)) memcpy.a
LIBS = $(shell cat $(LINKER_LIBS)) 
endif


#######################
### all
#######################
all: $(IMAGE_BIN) $(IMAGE_ASM)

$(IMAGE_ASM) : $(IMAGE_AXF)
	@fromelf --text -c --output $@ $<
	@WAV.exe $(IMAGE_ASM)


$(IMAGE_BIN) : $(IMAGE_AXF)
	@fromelf --bin --output $@ $<
	#@SUM.exe $(IMAGE_BIN)


$(IMAGE_AXF) : $(OBJS) $(LIBS)
	@mkdir -p $(PUBLIC_BIN_PATH)
#	@mkdir -p $(RD_PATH)/$(TEMPDIR)
#	@mkdir -p $(RD_PATH)/$(TEMPDIR)/$(RD_M)
#	@mkdir -p $(RD_PATH)/$(TEMPDIR)/$(RD_M)/$(nf)
	@$(LINK) $(LINK_FLAGS) --entry $(IMAGE_ENTRY_POINT) --scatter $(IMAGE_LAYOUT_FILE) --list $(IMAGE_LST) --symdefs $(IMAGE_SYM) --output $@ $(LIBS) $(OBJS)


$(LIBS) : 
	echo "Compile done"
	@echo examine $@


#######################
### 
#######################

ifeq ($(MAKECMDGOALS),all)
    ifneq ($(wildcard $(OBJ_DIR)/depend.mk),)
        include $(OBJ_DIR)/depend.mk
    endif
endif

