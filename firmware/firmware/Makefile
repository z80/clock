<<<<<<< HEAD
##############################################################################
# Build global options
# NOTE: Can be overridden externally.
#

# Compiler options here.
ifeq ($(USE_OPT),)
  #USE_OPT = -O2 -ggdb -fomit-frame-pointer -falign-functions=16
  USE_OPT = -O0 -g -ggdb -fomit-frame-pointer -falign-functions=16
endif

# C specific options here (added to USE_OPT).
ifeq ($(USE_COPT),)
  USE_COPT = 
endif

# C++ specific options here (added to USE_OPT).
ifeq ($(USE_CPPOPT),)
  USE_CPPOPT = -fno-rtti
endif

# Enable this if you want the linker to remove unused code and data
ifeq ($(USE_LINK_GC),)
  USE_LINK_GC = yes
endif

# If enabled, this option allows to compile the application in THUMB mode.
ifeq ($(USE_THUMB),)
  USE_THUMB = yes
endif

# Enable this if you want to see the full log while compiling.
ifeq ($(USE_VERBOSE_COMPILE),)
  USE_VERBOSE_COMPILE = no
endif

#
# Build global options
##############################################################################

##############################################################################
# Architecture or project specific options
#

# Enable this if you really want to use the STM FWLib.
ifeq ($(USE_FWLIB),)
  USE_FWLIB = no
endif

#
# Architecture or project specific options
##############################################################################

##############################################################################
# Project, sources and paths
#

# Define project name here
PROJECT = ch

# Imported source files and paths
CHIBIOS = ../chibios_2.6.3
include board/board.mk
include $(CHIBIOS)/os/hal/platforms/STM32F1xx/platform.mk
include $(CHIBIOS)/os/hal/hal.mk
include $(CHIBIOS)/os/ports/GCC/ARMCMx/STM32F1xx/port.mk
include $(CHIBIOS)/os/kernel/kernel.mk
include $(CHIBIOS)/os/various/fatfs_bindings/fatfs.mk
#include $(CHIBIOS)/test/test.mk

# Define linker script file here
LDSCRIPT= linker/STM32F103.ld

# C sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CSRC = $(PORTSRC) \
       $(KERNSRC) \
       $(TESTSRC) \
       $(HALSRC) \
       $(PLATFORMSRC) \
       $(BOARDSRC) \
       $(FATFSSRC) \
       $(CHIBIOS)/os/various/evtimer.c \
       $(CHIBIOS)/os/various/syscalls.c \
       src/main.c \
       src/dac.c \
       src/playback.c \
       src/3310.c \
       src/3310-backend.c

# C++ sources that can be compiled in ARM or THUMB mode depending on the global
# setting.
CPPSRC =

# C sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACSRC =

# C++ sources to be compiled in ARM mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
ACPPSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCSRC =

# C sources to be compiled in THUMB mode regardless of the global setting.
# NOTE: Mixing ARM and THUMB mode enables the -mthumb-interwork compiler
#       option that results in lower performance and larger code size.
TCPPSRC =

# List ASM source files here
ASMSRC = $(PORTASM)

INCDIR = $(PORTINC) $(KERNINC) $(TESTINC) \
         $(HALINC) $(PLATFORMINC) $(BOARDINC) \
         $(CHIBIOS)/os/various $(FATFSINC) \
         src

#
# Project, sources and paths
##############################################################################

##############################################################################
# Compiler settings
#

MCU  = cortex-m3

#TRGT = arm-elf-
TRGT = arm-none-eabi-
CC   = $(TRGT)gcc
CPPC = $(TRGT)g++
# Enable loading with g++ only if you need C++ runtime support.
# NOTE: You can use C++ even without C++ support if you are careful. C++
#       runtime support makes code size explode.
LD   = $(TRGT)gcc
#LD   = $(TRGT)g++
CP   = $(TRGT)objcopy
AS   = $(TRGT)gcc -x assembler-with-cpp
OD   = $(TRGT)objdump
HEX  = $(CP) -O ihex
BIN  = $(CP) -O binary

# ARM-specific options here
AOPT =

# THUMB-specific options here
TOPT = -mthumb -DTHUMB

# Define C warning options here
CWARN = -Wall -Wextra -Wstrict-prototypes

# Define C++ warning options here
CPPWARN = -Wall -Wextra

#
# Compiler settings
##############################################################################

##############################################################################
# Start of default section
#

# List all default C defines here, like -D_DEBUG=1
DDEFS =

# List all default ASM defines here, like -D_DEBUG=1
DADEFS =

# List all default directories to look for include files here
DINCDIR =

# List the default directory to look for the libraries here
DLIBDIR =

# List all default libraries here
DLIBS =

#
# End of default section
##############################################################################

##############################################################################
# Start of user section
#

# List all user C define here, like -D_DEBUG=1
UDEFS =

# Define ASM defines here
UADEFS =

# List all user directories here
UINCDIR =

# List the user directory to look for the libraries here
ULIBDIR =

# List all user libraries here
ULIBS =

#
# End of user defines
##############################################################################

ifeq ($(USE_FWLIB),yes)
  include $(CHIBIOS)/ext/stm32lib/stm32lib.mk
  CSRC += $(STM32SRC)
  INCDIR += $(STM32INC)
  USE_OPT += -DUSE_STDPERIPH_DRIVER
endif

include $(CHIBIOS)/os/ports/GCC/ARMCMx/rules.mk




=======
###############################################################################
#
#  	    File        : Makefile
#
#       Abstract    : Example Makefile for a C Project
#
#       Environment : Atollic TrueSTUDIO(R)
#
###############################################################################

OPTIMIZATION = s
CROSS_PATH   = c:/programs/yagarto
#CROSS_PATH   = c:/programs/codesourcery/arm-2011.03-41
#CROSS_PATH   = ~/programs/code-sourcery/2011.03-42
#CROSS_PREFIX = arm-none-linux-gnueabi-
CROSS_PREFIX = arm-none-eabi-
LINKER_LD    = cmsis/linker/stm32_flash.ld

# Define output directory
OBJECT_DIR = target
BIN_DIR = $(OBJECT_DIR)

#F_XTAL   = 8000000
#SYSCLOCK_CL = SYSCLK_FREQ_72MHz=72000000

DEFS=
DEFS += -DHSE_VALUE=8000000UL
DEFS += -DSYSCLK_FREQ_48MHz=48000000
DEFS += -DUSE_STDPERIPH_DRIVER -DSTM32F10X_MD
# Parameters for tuning SDC card access.
#BOARD    = EK_STM32F
#BOARD    = MINI_STM32
DEFS += -DUSE_EK_STM32F
#DEFS += -DSTM32_SD_USE_DMA
DEFS += -DSTARTUP_DELAY

#
CROSS = $(CROSS_PATH)/bin/$(CROSS_PREFIX)

# System configuration
CC = $(CROSS)gcc
OBJCOPY = $(CROSS)objcopy
SIZE = $(CROSS)size
RM=rm -rf

# Assembler, Compiler and Linker flags and linker script settings
LINKER_FLAGS=
LINKER_FLAGS+=-lm -mthumb -mcpu=cortex-m3 
LINKER_FLAGS+=-Wl,--gc-sections -T$(LINKER_LD) 
LINKER_FLAGS+=-static  -Wl,-cref 
LINKER_FLAGS+="-Wl,-Map=$(BIN_DIR)/target.map" 
LINKER_FLAGS+=-Wl,--defsym=malloc_getpagesize_P=0x1000

ASSEMBLER_FLAGS=
ASSEMBLER_FLAGS+=-c -g -O$(OPTIMIZATION) -mcpu=cortex-m3 -mthumb 
ASSEMBLER_FLAGS+=-x assembler-with-cpp 
ASSEMBLER_FLAGS+=-Isrc 
ASSEMBLER_FLAGS+=-Istdperiph
ASSEMBLER_FLAGS+=-Icmsis/src 
ASSEMBLER_FLAGS+=-Iusb

COMPILER_FLAGS=
COMPILER_FLAGS+=-c -g -mcpu=cortex-m3 -O$(OPTIMIZATION) 
COMPILER_FLAGS+=-Wall -ffunction-sections 
COMPILER_FLAGS+=-fdata-sections -mthumb 
COMPILER_FLAGS+=$(DEFS) 
COMPILER_FLAGS+=-Isrc 
COMPILER_FLAGS+=-Istdperiph 
COMPILER_FLAGS+=-Icmsis/src 
COMPILER_FLAGS+=-I3310 
COMPILER_FLAGS+=-Iusb 
COMPILER_FLAGS+=-Ifreertos
COMPILER_FLAGS+=-Ifreertos/cm3
COMPILER_FLAGS+=-Ifatfs

# Define sources and objects
SRC := $(wildcard */*/*/*/*/*/*/*.c) \
	$(wildcard */*/*/*/*/*/*.c) \
	$(wildcard */*/*/*/*/*.c) \
	$(wildcard */*/*/*/*.c) \
	$(wildcard */*/*/*.c) \
	$(wildcard */*/*.c) \
	$(wildcard */*.c)
SRCSASM := $(wildcard */*/*/*/*/*/*/*/*.s) \
	$(wildcard */*/*/*/*/*/*/*.s) \
	$(wildcard */*/*/*/*/*/*.s) \
	$(wildcard */*/*/*/*/*.s) \
	$(wildcard */*/*/*/*.s) \
	$(wildcard */*/*/*.s) \
	$(wildcard */*/*.s) \
	$(wildcard */*.s)
OBJS := $(SRC:%.c=$(OBJECT_DIR)/%.o) $(SRCSASM:%.s=$(OBJECT_DIR)/%.o)
OBJS := $(OBJS:%.S=$(OBJECT_DIR)/%.o)  

###############
# Build project
# Major targets
###############
all: buildelf

buildelf: $(OBJS) 
	@$(CC) -o "$(BIN_DIR)/target.elf" $(OBJS) $(LINKER_FLAGS)
	@$(OBJCOPY) -O ihex "$(BIN_DIR)/target.elf" "$(BIN_DIR)/target.hex"
	@$(OBJCOPY) -O binary "$(BIN_DIR)/target.elf" "$(BIN_DIR)/target.bin"
	$(SIZE) "$(BIN_DIR)/target.elf"


clean:
	$(RM) $(OBJS) "$(BIN_DIR)/target.elf" "$(BIN_DIR)/target.map"


##################
# Specific targets
##################
$(OBJECT_DIR)/src/main.o: src/main.c
	@echo make directory "./$(dir $@)"
	@mkdir -p ./$(dir $@)
	$(CC) $(COMPILER_FLAGS) src/main.c -o $(OBJECT_DIR)/src/main.o 


##################
# Implicit targets
##################
$(OBJECT_DIR)/%.o: %.c
	@echo make directory "./$(dir $@)"
	@mkdir -p ./$(dir $@)
	$(CC) $(COMPILER_FLAGS) $< -o $@

$(OBJECT_DIR)/%.o: %.s
	@echo make directory "./$(dir $@)"
	@mkdir -p ./$(dir $@)
	$(CC) $(ASSEMBLER_FLAGS) $< -o $@
	
$(OBJECT_DIR)/%.o: %.S
	@echo make directory "./$(dir $@)"
	@mkdir -p ./$(dir $@)
	$(CC) $(ASSEMBLER_FLAGS) $< -o $@


>>>>>>> parent of b886b3f... Just compiled old file.
