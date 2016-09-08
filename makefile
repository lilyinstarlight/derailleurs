#_______________________________________________________________________________
#
#			  ItOpen Energia makefile
#             (heavily based on edam's Arduino makefile)
#_______________________________________________________________________________
#                                                                    version 0.1
#
# Copyright (C) 2016 Foster McLane <fkmclane@gmail.com>
# Copyright (C) 2013 Alessandro Pasotti
# Copyright (C) 2011, 1012 Tim Marston <tim@ed.am>.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
#_______________________________________________________________________________
#
#
# This is a general purpose makefile for use with TI's MSP430 Launchpad hardware and
# and Energia Arduino clone software
#
# This makefile can be used as a drop-in replacement for the Energia IDE's
#
# The Energia software is required.  If you are using
#
#   export ENERGIADIR=~/somewhere/energia
#
# You will also need to set BOARD to the type of Arduino you're building for.
# Type `make boards` for a list of acceptable values.  You could set a default
# in your ~/.profile if you want, but it is suggested that you specify this at
# build time, especially if you work with different types of Arduino.  For
# example:
#
#   $ export ENERGIABOARD=MSP-EXP430FR6989-LP
#   $ make
#
# You may also need to set SERIALDEV if it is not detected correctly.
#
# The presence of a .ino (or .pde) file causes the arduino.mk to automatically
# determine values for SOURCES, TARGET and LIBRARIES.  Any .c, .cc and .cpp
# files in the project directory (or any "util" or "utility" subdirectories)
# are automatically included in the build and are scanned for Arduino libraries
# that have been #included. Note, there can only be one .ino (or .pde) file.
#
# Alternatively, if you want to manually specify build variables, create a
# Makefile that defines SOURCES and LIBRARIES and then includes arduino.mk.
# (There is no need to define TARGET).  Here is an example Makefile:
#
#   SOURCES := main.cc other.cc
#   LIBRARIES := EEPROM
#   include ~/src/energia.mk
#
# Here is a complete list of configuration parameters:
#
# ENERGIACONST The Energia/Arduino software version, as an integer, used to define the
#              ARDUINO/ENERGIA version constant. This defaults to 100 if undefined.
#
#
#
# ENERGIADIR   The path where the Arduino software is installed on your system.
#
#
# ENERGIABOARD Specify a target board type.  Run `make boards` to see available
#              board types.
#
# ENERGIASBOOK The path to the Energia sketchbook.
#
# LIBRARIES    A list of Energia libraries to build and include.  This is set
#              automatically if a .ino (or .pde) is found.
#
# SERIALDEV    The unix device name of the serial device that is the Arduino.
#              If unspecified, an attempt is made to determine the name of a
#              connected Arduino's serial device.
#
# SOURCES      A list of all source files of whatever language.  The language
#              type is determined by the file extension.  This is set
#              automatically if a .ino (or .pde) is found.
#
# TARGET       The name of the target file.  This is set automatically if a
#              .ino (or .pde) is found, but it is not necessary to set it
#              otherwise.
#
# This makefile also defines the following goals for use on the command line
# when you run make:
#
# all          This is the default if no goal is specified.  It builds the
#              target.
#
# target       Builds the target.
#
# upload       Uploads the last built target to an attached Launchpad.
#
# clean        Deletes files created during the build.
#
# boards       Display a list of available board names, so that you can set the
#              BOARD environment variable appropriately.
#
# monitor      Start `screen` on the serial device.  This is meant to be an
#              equivalent to the Arduino serial monitor.
#
# size         Displays size information about the built target.
#
#
# update       Updates the firmware on the launchpads programmer
#
#
# <file>       Builds the specified file, either an object file or the target,
#              from those that that would be built for the project.
#_______________________________________________________________________________
#

ENERGIABOARD := MSP-EXP430FR5969LP
ENERGIASBOOK := $(shell dirname $(realpath $(lastword $(MAKEFILE_LIST))))

# default arduino software directory, check software exists
ifndef ENERGIADIR
ENERGIADIR := $(firstword $(wildcard /opt/energia /opt/energia-bin /usr/share/energia))
endif
ifeq "$(wildcard $(ENERGIADIR)/hardware/energia/msp430/boards.txt)" ""
$(error ENERGIADIR is not set correctly; energia software not found)
endif

# sketchbook location
ifndef ENERGIASBOOK
ENERGIASBOOK := $(HOME)/Energia
endif

# default arduino version
ARDUINOCONST ?= 10610
ENERGIACONST ?= 18


# auto mode?
INOFILE := $(wildcard *.ino *.pde)
ifdef INOFILE
ifneq "$(words $(INOFILE))" "1"
$(error There is more than one .pde or .ino file in this directory!)
endif

# automatically determine sources and target
TARGET := $(basename $(INOFILE))
SOURCES := $(INOFILE) \
	$(wildcard *.c *.cc *.cpp) \
	$(wildcard $(addprefix util/, *.c *.cc *.cpp)) \
	$(wildcard $(addprefix utility/, *.c *.cc *.cpp))

# automatically determine included libraries
LIBRARIES := $(filter $(notdir $(wildcard $(ENERGIASBOOK)/libraries/*)), \
    $(shell sed -ne "s/^ *\# *include *[<\"]\(.*\)\.h[>\"]/\1/p" $(SOURCES)))

# automatically determine included libraries
LIBRARIES += $(filter $(notdir $(wildcard $(ENERGIADIR)/hardware/energia/msp430/libraries/*)), \
    $(shell sed -ne "s/^ *\# *include *[<\"]\(.*\)\.h[>\"]/\1/p" $(SOURCES)))
endif

# no serial device? make a poor attempt to detect an arduino
SERIALDEVGUESS := 0
ifeq "$(SERIALDEV)" ""
ifeq "$(ENERGIABOARD)" "MSP-EXP430F5529LP"
	SERIALDEV:= /dev/ttyACM1
else ifeq "$(ENERGIABOARD)" "MSP-EXP430FR5969LP"
	SERIALDEV:= /dev/ttyACM1
else
	SERIALDEV:= /dev/ttyACM0
endif

#SERIALDEV := $(firstword $(wildcard \
#	/dev/ttyACM? /dev/ttyUSB? /dev/tty.usbserial* /dev/tty.usbmodem*))
SERIALDEVGUESS := 1
endif

# software
#CC := msp430-gcc

COMPILER_PREFIX := $(ENERGIADIR)/hardware/tools/msp430/bin/

CC := $(COMPILER_PREFIX)msp430-gcc
CXX := $(COMPILER_PREFIX)msp430-g++
LD := $(COMPILER_PREFIX)msp430-ld
AR := $(COMPILER_PREFIX)msp430-ar
OBJCOPY := $(COMPILER_PREFIX)msp430-objcopy

UNAME_S := $(shell uname -s)
MSPDEBUG_PATH=$(ENERGIADIR)/hardware/tools/msp430/bin/
ifeq ($(UNAME_S),Darwin)
	MSPDEBUG_PATH=$(ENERGIADIR)/hardware/tools/msp430/mspdebug/
endif

MSPDEBUG := $(MSPDEBUG_PATH)/mspdebug
GDB := $(COMPILER_PREFIX)msp430-gdb
MSP430SIZE := $(COMPILER_PREFIX)msp430-size

DSLITE_PATH=$(ENERGIADIR)/hardware/tools/DSLite/
DSLITE := $(DSLITE_PATH)/DebugServer/bin/DSLite

# files
TARGET := $(if $(TARGET),$(TARGET),a.out)
OBJECTS := $(addsuffix .o, $(basename $(SOURCES)))
DEPFILES := $(patsubst %, .dep/%.dep, $(SOURCES))
ENERGIACOREDIR := $(ENERGIADIR)/hardware/energia/msp430/cores/msp430
ENERGIALIB := .lib/arduino.a
ENERGIALIBLIBSDIR := $(ENERGIADIR)/hardware/energia/msp430/libraries
ENERGIALIBLIBSPATH := $(foreach lib, $(LIBRARIES), \
	 $(ENERGIASBOOK)/libraries/$(lib)/ $(ENERGIASBOOK)/libraries/$(lib)/utility/ $(ENERGIADIR)/libraries/$(lib)/ $(ENERGIADIR)/libraries/$(lib)/utility/ $(ENERGIACOREDIR)/libraries/$(lib) $(ENERGIADIR)/hardware/energia/msp430/libraries/$(lib)/ $(ENERGIADIR)/hardware/energia/msp430/libraries/$(lib)/utility/)
ENERGIALIBOBJS := $(foreach dir, $(ENERGIACOREDIR) $(ENERGIALIBLIBSPATH), \
	$(patsubst %, .lib/%.o, $(wildcard $(addprefix $(dir)/, *.c *.cpp))))


# no board?
ifndef ENERGIABOARD
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error ENERGIABOARD is unset.  Type 'make boards' to see possible values)
endif
endif
endif

# obtain board parameters from the arduino boards.txt file
BOARDS_FILE := $(ENERGIADIR)/hardware/energia/msp430/boards.txt
BOARD_BUILD_MCU := \
	$(shell sed -ne "s/$(ENERGIABOARD).build.mcu=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_BUILD_FCPU := \
	$(shell sed -ne "s/$(ENERGIABOARD).build.f_cpu=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_BUILD_VARIANT := \
	$(shell sed -ne "s/$(ENERGIABOARD).build.variant=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_UPLOAD_SPEED := \
	$(shell sed -ne "s/$(ENERGIABOARD).upload.speed=\(.*\)/\1/p" $(BOARDS_FILE))
BOARD_UPLOAD_PROTOCOL := \
	$(shell sed -ne "s/$(ENERGIABOARD).upload.protocol=\(.*\)/\1/p" $(BOARDS_FILE))

# invalid board?
ifeq "$(BOARD_BUILD_MCU)" ""
ifneq "$(MAKECMDGOALS)" "boards"
ifneq "$(MAKECMDGOALS)" "clean"
$(error ENERGIABOARD is invalid.  Type 'make boards' to see possible values)
endif
endif
endif

# flags
UPLOAD_PROTOCOL := $(BOARD_UPLOAD_PROTOCOL)
UPLOAD_LD :=

ifeq "$(UPLOAD_PROTOCOL)" "dslite"
    UPLOAD := $(DSLITE)
    UPLOAD_FLAGS := 'load' '-c' '$(DSLITE_PATH)/$(ENERGIABOARD).ccxml' '-f' '$(TARGET).elf'
else
    UPLOAD := $(MSPDEBUG)
    UPLOAD_FLAGS := '$(UPLOAD_PROTOCOL)' 'erase' 'load $(TARGET).elf' 'exit'
endif

CPPFLAGS := -Os -Wall
CPPFLAGS += -ffunction-sections -fdata-sections
CPPFLAGS += -mmcu=$(BOARD_BUILD_MCU)
#CPPFLAGS += -DF_CPU=$(BOARD_BUILD_FCPU) -DARDUINO=$(ARDUINOCONST)  -DENERGIA=$(ENERGIACONST)
CPPFLAGS += -DF_CPU=8000000L -DARDUINO=$(ARDUINOCONST)  -DENERGIA=$(ENERGIACONST)
CPPFLAGS += -I. -Iutil -Iutility -I$(ENERGIACOREDIR)
CPPFLAGS += -I$(ENERGIADIR)/hardware/energia/msp430/variants/$(BOARD_BUILD_VARIANT)/
CPPFLAGS += -I$(ENERGIASBOOK)/hardware/energia/msp430/variants/$(BOARD_BUILD_VARIANT)/
CPPFLAGS += $(addprefix -I$(ENERGIASBOOK)/libraries/,  $(LIBRARIES))
CPPFLAGS += $(patsubst %, -I$(ENERGIASBOOK)/libraries/%/utility,  $(LIBRARIES))
CPPFLAGS += $(addprefix -I$(ENERGIADIR)/libraries/, $(LIBRARIES))
CPPFLAGS += $(patsubst %, -I$(ENERGIADIR)/libraries/%/utility, $(LIBRARIES))
CPPFLAGS += $(addprefix -I$(ENERGIADIR)/hardware/energia/msp430/libraries/, $(LIBRARIES))
CPPFLAGS += $(patsubst %, -I$(ENERGIADIR)/hardware/energia/msp430/libraries/%/utility, $(LIBRARIES))
CPPDEPFLAGS = -MMD -MP -MF .dep/$<.dep
CPPINOFLAGS := -x c++ -include $(ENERGIACOREDIR)/Arduino.h
LINKFLAGS := -mmcu=$(BOARD_BUILD_MCU) -Os -Wl,-gc-sections,-u,main -lm

# figure out which arg to use with stty
STTYFARG := $(shell stty --help > /dev/null 2>&1 && echo -F || echo -f)

# include dependencies
ifneq "$(MAKECMDGOALS)" "clean"
-include $(DEPFILES)
endif

# default rule
.DEFAULT_GOAL := all

#_______________________________________________________________________________
#                                                                          RULES

.PHONY:	all target upload clean boards monitor size

all: target

target: $(TARGET).elf

upload: target
	@echo "\nUploading to board..."
	$(UPLOAD_LD) $(UPLOAD) $(UPLOAD_FLAGS)


clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET).elf $(ENERGIALIB) *~
	rm -rf .lib .dep

boards:
	@echo Available values for BOARD:
	@echo $(BOARDS_FILE)
	@sed -ne '/^#/d; /^[^.]\+\.name=/p' $(BOARDS_FILE) | \
		sed -e 's/\([^.]\+\)\.name=\(.*\)/\1            \2/' \
			-e 's/\(.\{14\}\) *\(.*\)/\1 \2/'

monitor:
	stty raw 9600 -F $(SERIALDEV)
	cat $(SERIALDEV)

size: $(TARGET).elf
	echo && $(MSP430SIZE) $(TARGET).elf


debug:
	$(MSPDEBUG) $(MSPDEBUG_PROTOCOL) gdb
	cgdb  -d $(GDB) $(TARGET).elf


# building the target


$(TARGET).elf: $(ENERGIALIB) $(OBJECTS)
	$(CC) $(LINKFLAGS) $(OBJECTS) $(ENERGIALIB) -o $@

%.o: %.c
	mkdir -p .dep/$(dir $<)
	$(COMPILE.c) $(CPPDEPFLAGS) -o $@ $<

%.o: %.cpp
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.cc
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.C
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $<

%.o: %.ino
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ $(CPPINOFLAGS) $<

%.o: %.pde
	mkdir -p .dep/$(dir $<)
	$(COMPILE.cpp) $(CPPDEPFLAGS) -o $@ -x c++ -include $(ENERGIACOREDIR)/Arduino.h $<

# building the arduino library

$(ENERGIALIB): $(ENERGIALIBOBJS)
	$(AR) rcs $@ $?

.lib/%.c.o: %.c
	mkdir -p $(dir $@)
	$(COMPILE.c) -o $@ $<

.lib/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

.lib/%.cc.o: %.cc
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

.lib/%.C.o: %.C
	mkdir -p $(dir $@)
	$(COMPILE.cpp) -o $@ $<

update:
	$(MSPDEBUG_LD) $(MSPDEBUG) $(MSPDEBUG_PROTOCOL) --allow-fw-update
