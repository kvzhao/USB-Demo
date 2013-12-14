# Sources
SRCS = main.c stm32f4xx_it.c system_stm32f4xx.c syscalls.c

# USB
SRCS += usbh_usr.c usb_bsp.c

# Periph
SRCS += ./lib/StdPeriph/src/stm32f4xx_rcc.c \
	 ./lib/StdPeriph/src/stm32f4xx_gpio.c \
	 ./lib/StdPeriph/src/stm32f4xx_tim.c \
	 ./lib/StdPeriph/src/stm32f4xx_spi.c \
	 ./lib/StdPeriph/src/stm32f4xx_exti.c \
	 ./lib/StdPeriph/src/stm32f4xx_adc.c \
	 ./lib/StdPeriph/src/stm32f4xx_syscfg.c \
	 ./lib/StdPeriph/src/stm32f4xx_dma.c \
	 ./lib/StdPeriph/src/stm32f4xx_dac.c \
	 ./lib/StdPeriph/src/stm32f4xx_i2c.c \
	 ./lib/StdPeriph/src/stm32f4xx_flash.c \
	 ./lib/StdPeriph/src/misc.c

# Project name
PROJ_NAME=stm32f4_hackin_usb
OUTPATH=build

###################################################

# Check for valid float argument
# NOTE that you have to run make clan after
# changing these as hardfloat and softfloat are not
# binary compatible
ifneq ($(FLOAT_TYPE), hard)
ifneq ($(FLOAT_TYPE), soft)
#override FLOAT_TYPE = hard
override FLOAT_TYPE = soft
endif
endif

###################################################

CC=arm-none-eabi-gcc
OBJCOPY=arm-none-eabi-objcopy
SIZE=arm-none-eabi-size

CFLAGS  = -std=gnu99 -g -O2 -Wall -Tstm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mthumb-interwork -nostartfiles -mcpu=cortex-m4

ifeq ($(FLOAT_TYPE), hard)
CFLAGS += -fsingle-precision-constant -Wdouble-promotion
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
else
CFLAGS += -msoft-float
endif

###################################################

vpath %.c src
vpath %.a lib

ROOT=$(shell pwd)

# Includes
CFLAGS += -Iinc -Ilib/Core/cmsis -Ilib/Core/stm32
CFLAGS += -Ilib/Conf

# Library paths
LIBPATHS = -Llib/StdPeriph -Llib/USB_OTG
LIBPATHS += -Llib/USB_Host/Core -Llib/USB_Host/Class/MSC
LIBPATHS += -Llib/fat_fs
#LIBPATHS += -Linc

# lib to link
LIBS = -lm -lfatfs -lstdperiph -lusbhostcore -lusbhostmsc -lusbcore
# Extra includes
CFLAGS += -Ilib/USB_Host/Core/inc
CFLAGS += -Ilib/USB_Host/Class/MSC/inc
CFLAGS += -Ilib/fat_fs/inc
CFLAGS += -Ilib/StdPeriph/inc
CFLAGS += -Ilib/USB_OTG/inc

# add startup file to build
SRCS += lib/startup_stm32f4xx.s

OBJS = $(SRCS:.c=.o)

###################################################

.PHONY: lib proj

all: lib proj
	$(SIZE) $(OUTPATH)/$(PROJ_NAME).elf

lib:
	$(MAKE) -C lib FLOAT_TYPE=$(FLOAT_TYPE)

proj: 	$(OUTPATH)/$(PROJ_NAME).elf

$(OUTPATH)/$(PROJ_NAME).elf: $(SRCS)
	$(CC) $(CFLAGS) $^ -o $@ $(LIBPATHS) $(LIBS)
	$(OBJCOPY) -O ihex $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).hex
	$(OBJCOPY) -O binary $(OUTPATH)/$(PROJ_NAME).elf $(OUTPATH)/$(PROJ_NAME).bin

flash:
	st-flash write $(OUTPATH)/$(PROJ_NAME).bin 0x8000000

clean:
	rm -f *.o
	rm -f $(OUTPATH)/$(PROJ_NAME).elf
	rm -f $(OUTPATH)/$(PROJ_NAME).hex
	rm -f $(OUTPATH)/$(PROJ_NAME).bin
	$(MAKE) clean -C lib # Remove this line if you don't want to clean the libs as well
	
