
#################################################################
### MAKE_DEBUG = 1 : make debug version program (with least
###                  optimization)
### MAKE_DEBUG = 0 : make release version program with most
###                  optimization) (default)
#################################################################
MAKE_DEBUG = 0

#################################################################
### MAKE_MULTI_BIN = 0 for ATCM only
### MAKE_MULTI_BIN = 1 for ATCM and B0TCM 
### MAKE_MULTI_BIN = 2 for ATCM and SRAM
#################################################################
MAKE_MULTI_BIN = 2

#################################################################
# MAKE_TARGET = 0: make firmware for ASIC
#               1: make firmware for FPGA
#               2: make firmware for COSIM
#################################################################
ifeq ($(T), 0)
	MAKE_TARGET = 0
else ifeq ($(T), 2)
	MAKE_TARGET = 2
	MAKE_DEBUG = 0
else
	MAKE_TARGET = 0
endif

#################################################################
### Select module
#################################################################
C_MODULE_FLAG        = -D'MAKE_RDISK=0'


#################################################################
### DEFAULT_IMAGE_PREFIX
#################################################################
PROJECT_NAME         = mymcu
DEFAULT_IMAGE_PREFIX = $(PROJECT_NAME)_CPU0


#################################################################
### RELEASE_DIR
#################################################################
PUBLIC_BIN_PATH       = $(ROOT_PATH)/bin


#################################################################
### ROOT_PATH
#################################################################
ROOT_PATH = $(CURDIR)


#################################################################
### PUBLIC_INC_PATH
#################################################################
PUBLIC_INC_PATH =


#################################################################
### PUBLIC_LIB_PATH
#################################################################
PUBLIC_LIB_PATH = $(ROOT_PATH)/lib


#################################################################
### MAKE_RULES
#################################################################
MAKE_RULES = $(ROOT_PATH)/make/rules.mk


#################################################################
### compiler commands and options
#################################################################
CC         = armcc
ASM        = armasm
AR         = armar
LINK       = armlink
MAKE       = make -f mk # --no-print-directory
AR_FLAGS   = --create
LINK_FLAGS = --map --info totals --debug --userlibpath $(PUBLIC_LIB_PATH)


#################################################################
### compiler MODULE/NAND TYPE options and select cfg
###############################################################

m 	=
nf 	=	B17A#default NAND TYPE
OUT	=

NAND_PATH  = src/fc/nand/
NAND_TYPE  = 	B27A

SYS_MODULE_TYPE  = 	intc

MODULE_TYPE	= $(SYS_MODULE_TYPE)	\
				dram

#################################################################
###  Target output file & path name set
#################################################################
ifneq ($(m),)
ifeq ($(strip $(OUT)),cpu1)
	DEFAULT_IMAGE_PREFIX = $(PROJECT_NAME)_CPU1
	RD_M = $(m)_cpu1
endif
ifeq ($(strip $(OUT)),cpu0)
	DEFAULT_IMAGE_PREFIX = $(PROJECT_NAME)_CPU0
	RD_M = $(m)_cpu1
endif
ifeq ($(strip $(OUT)),)
	OUT = cpu0
	RD_M = $(m)_cpu0
endif
else
ifeq ($(strip $(OUT)),cpu1)
	DEFAULT_IMAGE_PREFIX = $(PROJECT_NAME)_CPU1
	RD_M = cpu1
endif
ifeq ($(strip $(OUT)),cpu0)
	DEFAULT_IMAGE_PREFIX = $(PROJECT_NAME)_CPU0
	RD_M = cpu1
endif

ifeq ($(strip $(OUT)),)
	OUT = cpu0
	RD_M = cpu0
endif

endif

#################################################################
###  SUB_DIRS select
#################################################################
ifeq ($(m),)
# top compiler
	PUBLIC_BIN_PATH = $(ROOT_PATH)/bin/$(RD_M)
	SUB_DIRS += src/bsp/myf407                                              \
				src/cmsis/cmsis/Device/STM32F4xx/Source/Templates           

else
# other system modules compiler
	PUBLIC_BIN_PATH = $(ROOT_PATH)/bin/ut/$(RD_M)
	SUB_DIRS += src/bsp/myf401                                              \
				src/cmsis/cmsis/Device/STM32F4xx/Source/Templates           
endif

# common part driver
SUB_DIRS += src/app			\
			src/bus			\
			src/cmsis		\
			src/driver		\
			src/mcu			\
			src/middlewares	\
			src/msic		\
			src/res


#################################################################
### 
#################################################################
# Based on the extern MACRO to select different startup.s
CTRL_MACRO	=	0
# cache case 1
ifeq ($(strip $(EXTRA_C_DEFINES)),-DSRAM0_AREA_FOR_WBWA)
	CTRL_MACRO = 1
ifeq ($(MAKE_TARGET),2)	
	E = 3
endif
endif
# cache case 2
ifeq ($(strip $(EXTRA_C_DEFINES)),-DSRAM0_AREA_FOR_WTNWA)
	CTRL_MACRO = 2
ifeq ($(MAKE_TARGET),2)	
	E = 3
endif
endif
# cache case 3
ifeq ($(strip $(EXTRA_C_DEFINES)),-DSRAM0_AREA_FOR_CACHE)
	CTRL_MACRO = 3
endif

CURDATE	=	`date "+%Y%m%d"`
TEMPDIR	=	$(PROJECT_NAME)$(CURDATE)

RD_PATH		= Y:/upload

svnV=`svn info | grep "Last Changed Rev" | grep "[0-9]*" | grep -o '[0-9]\+'`

#################################################################
### commands provide by the shell
#################################################################
DEL  = rm -f
COPY = cp -f
MV   = mv -f

AUTO_HEADER		= src/include/auto_header.h

export m
export nf
export OUT
export MAKE_MULTI_BIN
export MAKE_DEBUG
export MAKE_TARGET
export MAKE_RELEASE
export MAKE_HW_SIM
export ROOT_PATH
export PUBLIC_INC_PATH
export PUBLIC_LIB_PATH
export PUBLIC_BIN_PATH
export MAKE_RULES
export CC ASM AR LINK
export AR_FLAGS LINK_FLAGS
export DEL COPY MV
export C_MODULE_FLAG
export RD_PATH
export TEMPDIR
export RD_M
export EXTRA_C_DEFINES
export IMAGE_ENTRY_POINT

.NOTPARALLEL : auto_cfg

ifeq ($(CONFIGURE),)
CONFIGURE = $(DEFAULT_IMAGE_PREFIX)
endif


all : clean pre pre_cp 
	@echo "==> Compiling codes"
	@for x in $(SUB_DIRS); do \
		cd $$x; $(MAKE) $@ IMAGE_PREFIX=$(CONFIGURE); \
		cd $(CURDIR); \
	done
	@if [ $(MAKE_TARGET) == 0 ]; then  echo "---------------- make firmware for ASIC $(m) ------------------"; fi

	@if [ $(MAKE_MULTI_BIN) == 2 ]; then \
		if [ -e $(PUBLIC_BIN_PATH)/BEAGLE_CPU0.bin/INIT ] && [ -e $(PUBLIC_BIN_PATH)/BEAGLE_CPU0.bin/CODE ]; then \
			echo "Create CP.bin to WriteFlash_Tool.exe"; \
			easy2bin_BF.exe Two \
				$(PUBLIC_BIN_PATH)/BEAGLE_CPU0.bin/INIT \
				$(PUBLIC_BIN_PATH)/BEAGLE_CPU0.bin/CODE NULL NULL NULL NULL NULL NULL \
				0 e00c0000 0 0 0 0 0 0; \
			cp fwcode.bin CP.bin; \
			rm fwcode.bin; \
		fi; \
	fi
	
	@echo "#############################"
	
pre:
	@rm -rf $(AUTO_HEADER)
	@echo "#ifndef _AUTO_HEADER_H_" >> $(AUTO_HEADER)
	@echo "#define _AUTO_HEADER_H_" >> $(AUTO_HEADER)
	@echo "MAKE for ASIC"
	@echo "#define ASIC_EN	1" >> $(AUTO_HEADER)
	@echo "#define SVN_VERSION $(svnV)0" >> $(AUTO_HEADER)
	@echo "#define RLS_DATE $(CURDATE)" >> $(AUTO_HEADER)
	@echo "#endif /*SRC_INCLUDE_AUTO_HEADER_H_*/" >> $(AUTO_HEADER)
	@echo "MAKE_DEBUG = $(MAKE_DEBUG)"
	cat $(AUTO_HEADER)

pre_cp:	
	@if [ $(MAKE_MULTI_BIN) == 2 ]; \
	then \
		cp -rf src/scatter/scatter_atcm_sram_$(OUT).scat  src/scatter.scat; \
		cp -rf src/cpu/startup_simple.s  src/cpu/startup.s;	\
		if [ $(MAKE_TARGET) == 2 ]; \
		then \
			cp -rf src/cpu/startup_simple_cosim.s  src/cpu/startup.s;\
		fi;\
	fi

clean :
	@echo "==> Clean environment"
	rm -rf src/scatter.scat 
	rm -rf src/cpu/startup.s 
	rm -rf $(PUBLIC_BIN_PATH)/$(DEFAULT_IMAGE_PREFIX).*
	rm -rf $(PUBLIC_LIB_PATH)/*.*
	@for x in $(SUB_DIRS); do cd $$x; $(MAKE) $@; cd $(CURDIR); done
	
cleanall : clean
	@echo "==> All Clean environment"
	rm -rf bin/*

