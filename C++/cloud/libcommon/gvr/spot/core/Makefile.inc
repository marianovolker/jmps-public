#!/bin/sh
##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file Makefile.inc
## @author mgrieco
## @date Friday, December 12, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##
MAKEFILE_INC_NAME := $(lastword ${MAKEFILE_LIST})

##-#######################################################################-##
# global variables set only for building without spot infrastructure:
ifndef GVR_MODULE_PATH
  GVR_MODULE_PATH := "${CURDIR}/build/cmake/module"
endif
ifndef GVR_TOOLCHAIN_PATH
  GVR_TOOLCHAIN_PATH := "${CURDIR}/build/cmake/toolchain"
endif
ifndef GVR_USE_REVISION
  GVR_USE_REVISION := ON
endif
ifndef TARGETS
  TARGETS := romfs
else
  TARGETS += romfs
endif
ifndef TARGETS_HELP
  TARGETS_HELP := \tromfs: Target voided specially for libraries.\n
else
  TARGETS_HELP += \tromfs: Target voided specially for libraries.\n
endif
##-#######################################################################-##

##-#######################################################################-##
# Rules:
# This target '${MAKEFILE_INC_NAME}' is a hack due to an issue when making '%' depend on a .PHONY target.
# Even though the '${MAKEFILE_INC_NAME}' is not counted in the ${MAKECMDGOALS} it is oddly requested while doing make.
${MAKEFILE_INC_NAME}: ; @true

# This target is voided specially for libraries.
romfs:
	@echo "Target romfs skipped for library builds" >&2

#.PHONY: ${MAKEFILE_INC_NAME} romfs
##-#######################################################################-##

MAKEFILE_INC_NAME :=
