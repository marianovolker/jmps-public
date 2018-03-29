#!/bin/sh
##-#######################################################################-##
# Copyright © 2014 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file Makefile
##  @author mgrieco
##  @date Friday, December 12, 2014
## @copyright Copyright © 2014 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##
#__SPOT-BUILD-SCRIPT__#
MAKEFILE_NAME := $(lastword ${MAKEFILE_LIST})

ifndef .SHELLFLAGS
  .SHELLFLAGS := -c
endif

##-#######################################################################-##
# Default directories used this module:
ifndef CORE_DIR
  CORE_DIR := ${CURDIR}/core
endif
ifndef DOCS_DIR
  DOCS_DIR := ${CURDIR}/docs/output
endif
##-#######################################################################-##

##-#######################################################################-##
# Commands to be used this module:
ifndef WHICH
  WHICH := $(shell which which)
  ifeq ($(strip ${WHICH}),)
    $(error Command which not found)
  endif
endif
ifndef RCP
  RCP := $(shell ${WHICH} cp)
  ifneq ($(strip ${RCP}),)
    RCP += -r
  else
    $(error Command cp not found)
  endif
endif
ifndef RRM
  RRM := $(shell ${WHICH} rm)
  ifneq ($(strip ${RRM}),)
    RRM += -rf
  else
    $(error Command rm not found)
  endif
endif
ifndef MKDIR
  MKDIR := $(shell ${WHICH} mkdir)
  ifneq ($(strip ${MKDIR}),)
    MKDIR += -p
  else
    $(error Command mkdir not found)
  endif
endif
ifndef EGREP
  EGREP := $(shell ${WHICH} grep)
  ifneq ($(strip ${EGREP}),)
    EGREP += -E
  else
    $(error Command grep not found)
  endif
endif
ifndef ZXARGS
  ZXARGS := $(shell ${WHICH} xargs)
  ifneq ($(strip ${ZXARGS}),)
    ZXARGS += -0 --no-run-if-empty
  else
    $(error Command xargs not found)
  endif
endif
ifndef NUDIFF
  NUDIFF := $(shell ${WHICH} diff)
  ifneq ($(strip ${NUDIFF}),)
    NUDIFF += -Nu
  else
    $(error Command diff not found)
  endif
endif
ifndef SNUDIFF
  ifneq ($(strip ${NUDIFF}),)
    SNUDIFF := ${NUDIFF} -s
  else
    $(error Command diff not found)
  endif
endif
##-#######################################################################-##

##-#######################################################################-##
# Spread Makefile infromation:
ifndef SPREAD_MAKEFILE_NAME
  SPREAD_MAKEFILE_NAME := ${MAKEFILE_NAME}
endif

ifndef SPREAD_IGNORE_DIRS
  SPREAD_IGNORE_DIRS := .build .svn .import .repository
else
  SPREAD_IGNORE_DIRS += .build .svn .import .repository
endif

ifndef SPREAD_TAG_REGEX
  SPREAD_TAG_REGEX := '^\#__\w+-BUILD-SCRIPT__\#'
endif

SPREAD_COMMAND_GET_TAG := ${EGREP} -m 1 -Iixoe ${SPREAD_TAG_REGEX}
SPREAD_COMMAND_GET_FILES_TAG_REGEX := ${EGREP} -m 1 --include $(notdir ${SPREAD_MAKEFILE_NAME}) $(addprefix --exclude-dir=,${SPREAD_IGNORE_DIRS}) -ZIlrixe ${SPREAD_TAG_REGEX}
SPREAD_TAG := $(shell ${SPREAD_COMMAND_GET_TAG} ${MAKEFILE_NAME})
SPREAD_COMMAND_CHECK_TAG := ${EGREP} -ZIixqe '^${SPREAD_TAG}'

SPREAD_COMMAND := ${SPREAD_COMMAND_GET_FILES_TAG_REGEX} . \
                    | ${ZXARGS} -I path ${SHELL} ${.SHELLFLAGS} "${SPREAD_COMMAND_GET_TAG} path \
                    | ${SPREAD_COMMAND_CHECK_TAG} && printf 'path\0'" \
                    | ${ZXARGS} -I path ${SHELL} ${.SHELLFLAGS} \
                        "test ${MAKEFILE_NAME} -ef path || \
                          { { cp -p path path.bak || { printf 'Unable to create backup path.bak.\n' && false; } ; } && \
                            { { cp -fp ${MAKEFILE_NAME} path && printf 'Updated makefile path [Backup: path.bak].\n' ; } || \
                              { printf 'Unable to update makefile path [Backup: path.bak].\n' && false; } ; } ; }"

SPREAD_CHECK_COMMAND := ${SPREAD_COMMAND_GET_FILES_TAG_REGEX} . \
                          | ${ZXARGS} -I path ${SHELL} ${.SHELLFLAGS} "${SPREAD_COMMAND_GET_TAG} path \
                          | ${SPREAD_COMMAND_CHECK_TAG} && printf 'path\0'" \
                          | ${ZXARGS} -I path ${SHELL} ${.SHELLFLAGS} \
                              "test ${MAKEFILE_NAME} -ef path || \
                              ${SNUDIFF} ${MAKEFILE_NAME} path"
##-#######################################################################-##

##-#######################################################################-##
# Configuration flags for CMAKE:
GVR_TOOLCHAIN_PATH := "${ROOTDIR}/user/cmake/toolchain"
GVR_MODULE_PATH := "${ROOTDIR}/user/cmake/module"
##-#######################################################################-##

##-#######################################################################-##
# Rules:
all:
	@${TEST} -d ${CORE_DIR} && ${CD} ${CORE_DIR} && ${MAKE} -f Makefile $@

romfs:
	@${TEST} -d ${CORE_DIR} && ${CD} ${CORE_DIR} && ${MAKE} -f Makefile $@

clean:
	@${TEST} -d ${CORE_DIR} && ${CD} ${CORE_DIR} && ${MAKE} -f Makefile $@

doc:
	@${TEST} -d ${CORE_DIR} && ${CD} ${CORE_DIR} && ${MAKE} -f Makefile $@ && ${RRM} ${DOCS_DIR}/documentation && ${MKDIR} ${DOCS_DIR} && ${RCP} ${CORE_DIR}/.repository/documentation ${DOCS_DIR}/documentation

# This target enforces Makefiles integrity in subdirectories named after this makefile with the same SPREAD_TAG.
spread:
	@${SPREAD_COMMAND}

# This target checks Makefiles integrity in subdirectories named after this makefile with the same SPREAD_TAG.
spread-check:
	@echo
	@echo "Checking Makefiles integrity for spread '${SPREAD_TAG}' tag..."
	@echo
	@${SPREAD_CHECK_COMMAND}

##-#######################################################################-##
