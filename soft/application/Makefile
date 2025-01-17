#**************************************************************#
# COPY THIS FILE AS "Makefile" IN THE "src" DIR OF YOUR MODULE #
# AND CUSTOMIZE IT TO FIT YOUR NEEDS.                          #
#**************************************************************#


## ----------------------------------------------------------- ##
## Don't touch the next line unless you know what you're doing.##
## ----------------------------------------------------------- ##
include ${SOFT_WORKDIR}/env/compilation/compilevars.mk

## -------------------------------------- ##
## General information about this module. ##
## You must edit these appropriately.     ##
## -------------------------------------- ##

# Name of the module, with toplevel path, e.g. "phy/tests/dishwasher"
LOCAL_NAME := application

ifeq ($(findstring at/packed,${GLOBAL_BINARY_LIBS}),)

# include folder
LOCAL_ADD_INCLUDE := $(PLATFORM_SERVICE)/include \
	$(PLATFORM_SERVICE)/lv_lib_png \
	$(PLATFORM_SERVICE)/lvgl_800 \
	$(PLATFORM_SERVICE)/lvgl_800/core \
	$(PLATFORM_SERVICE)/lvgl_800/draw \
	$(PLATFORM_SERVICE)/lvgl_800/extra \
	$(PLATFORM_SERVICE)/lvgl_800/font \
	$(PLATFORM_SERVICE)/lvgl_800/hal \
	$(PLATFORM_SERVICE)/lvgl_800/misc \
	$(PLATFORM_SERVICE)/lvgl_800/widgets \
	application/casio_clock \
	application/analog_clock \

# Space-separated list of modules (libraries) your module depends upon.
# These should include the toplevel name, e.g. "phy/dishes ciitech/hotwater"

LOCAL_MODULE_DEPENDS := \
	application/casio_clock \
	application/analog_clock \


# Add includes from other modules we do not wish to link to
LOCAL_API_DEPENDS := \
                     ${API_PLATFORM_DEPENDS} \
                     ${PLATFORM_SYSTEM}/svc/imgs/include \
                     ${PLATFORM_SYSTEM}/svc/imgs/src \
                     ${PLATFORM_SERVICE}/lvgl_800 \

# Generate the revision (version) file automatically during the make process.
AUTO_GEN_REVISION_HEADER := yes

## --------------------------- ##
## Override toplevel settings  ##
## --------------------------- ##

# Set more dependencies as binary : they will not be compiled from source.
LOCAL_BINARY_LIBS ?=

# Set dependencies as source : the library will be generated from source
# This supersedes the BINARY (local+global) setting in case of conflict
LOCAL_SRC_LIBS ?=


ifeq "${GPRS_SUPPORT}" "y"
ifneq "${USE_KCONFIG}" "y"
LOCAL_EXPORT_FLAG += CFW_GPRS_SUPPORT
ifeq "${PPP_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += CFW_PPP_SUPPORT 
endif
endif
endif

ifeq "${CHIP_HAS_AP}" "1"
# CT_MODEM is forced to 1
override CT_MODEM := 1
endif

ifeq "${CT_MODEM}" "2"
LOCAL_EXPORT_FLAG += MMI_WITH_AT
endif

ifeq "${CT_MODEM}" "1"
LOCAL_EXPORT_FLAG += MMI_ONLY_AT
endif

ifeq "${CT_RELEASE}" "debug"
LOCAL_EXPORT_FLAG += AT_DEBUG_SUPPORT                                   
endif

ifeq "${IOT_SUPPORT}" "1"
LOCAL_EXPORT_FLAG += IOT_SUPPORT
endif

ifeq "${WOLFSSL_TEST}" "1"
LOCAL_EXPORT_FLAG += CRYPT_TEST
LOCAL_EXPORT_FLAG += CLIENT_TEST
endif
## ------------------------------------- ##
##	List all your sources here           ##
## ------------------------------------- ##

# Assembly / C code
S_SRC := $(notdir $(wildcard src/*.S))
C_SRC := $(notdir $(wildcard src/*.c))
# Simulation specific inputs
INPUT_FILES :=  

else # GLOBAL_BINARY_LIBS

SIM_STEMS := _singlesim _dualsim _threesim _foursim

PS_STEM :=
ifeq "${GPRS_SUPPORT}" "y"
PS_STEM := _egprs
endif
ifeq "${EGPRS_SUPPORT}" "y"
PS_STEM := _egprs
endif

PACKED_LIB := $(notdir ${LOCAL_NAME})$(word ${NUMBER_OF_SIM},${SIM_STEMS})${PS_STEM}

ifeq "${CHIP_HAS_AP}" "1"
PACKED_LIB_PATH := ${LOCAL_NAME}/packed/smartphone/${PACKED_LIB}
else
PACKED_LIB_PATH := ${LOCAL_NAME}/packed/featurephone/${PACKED_LIB}
endif

LOCAL_BINARY_LIBS    += ${PACKED_LIB_PATH}
LOCAL_MODULE_DEPENDS := ${PACKED_LIB_PATH}

endif # GLOBAL_BINARY_LIBS

# Set this to any non-null string to signal a module which 
# generates a binary (must contain a "main" entry point). 
# If left null, only a library will be generated.
IS_TOP_LEVEL := yes

## ------------------------------------- ##
##  Do Not touch below this line         ##
## ------------------------------------- ##
include ${SOFT_WORKDIR}/${PLATFORM_SYSTEM}/stack/stack.mk
include ${SOFT_WORKDIR}/env/compilation/compilerules.mk

