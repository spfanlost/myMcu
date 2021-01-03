
#######################
### 
#######################

all : $(TARGET_LIB_PATH)/$(LIB_NAME)
ifneq ($(SUB_DIRS),)
	@for x in $(SUB_DIRS); do cd $$x; make $@; cd $(CURDIR); done
endif


$(TARGET_LIB_PATH)/$(LIB_NAME) : $(TARGET_LIB_PATH) $(OBJS)
	@$(AR) $(AR_FLAGS) $@ $(OBJS)
	@echo $@ >> $(LINKER_LIBS)

    
$(TARGET_LIB_PATH)::
	@mkdir -p $(TARGET_LIB_PATH)
	@#if [ -f $(TARGET_LIB_PATH)/$(LIB_NAME) ]; then $(DEL) $(TARGET_LIB_PATH)/$(LIB_NAME); fi
	@if [ -f $(LINKER_LIBS) ]; then sed -i -e "/$(LIB_NAME)/d" $(LINKER_LIBS); fi


#######################
### 
#######################

ifeq ($(MAKECMDGOALS),all)
    ifneq ($(wildcard $(OBJ_DIR)/depend.mk),)
        include $(OBJ_DIR)/depend.mk
    endif
endif
