#!/bin/sh
##-#######################################################################-##
# Copyright © 2014, 2015 Gilbarco Inc.
# Confidential and Proprietary.
##
## @file Makefile
## @author pcantarini
## @date Tuesday, April 01, 2014
## @copyright Copyright © 2014, 2015 Gilbarco Inc. Confidential and Proprietary.
##-#######################################################################-##
#__CORE-BUILD-SCRIPT__#
MAKEFILE_NAME := $(lastword ${MAKEFILE_LIST})

ifndef .SHELLFLAGS
  .SHELLFLAGS := -c
endif

# This target 'default' is a hack due to an issue with including other Makefiles.
# Force the configure target to be the default one.
default: configure; @true

##-#######################################################################-##
# Specific variables and targets for this module:
-include $(wildcard ${CURDIR}/Makefile.inc)
##-#######################################################################-##

##-#######################################################################-##
# Default directories used this module:
ifndef SOURCE_DIR
  SOURCE_DIR := ${CURDIR}
endif
override SOURCE_DIR := ${SOURCE_DIR:%/=%}
ifndef ESSENTIAL_DIR
  ESSENTIAL_DIR := ${CURDIR}/.essential
endif
override ESSENTIAL_DIR := ${ESSENTIAL_DIR:%/=%}
ifndef BUILD_DIR
  BUILD_DIR := ${CURDIR}/.build
endif
override BUILD_DIR := ${BUILD_DIR:%/=%}
ifndef IMPORT_DIR
  IMPORT_DIR := ${CURDIR}/.import
endif
override IMPORT_DIR := ${IMPORT_DIR:%/=%}
ifndef IMPORT_DIRS
  IMPORT_DIRS := ${IMPORT_DIR}
endif
override IMPORT_DIRS := $(patsubst %/,%,$(subst :, ,${IMPORT_DIRS}))
ifndef EXPORT_DIR
  EXPORT_DIR := ${IMPORT_DIR}
endif
override EXPORT_DIR := ${EXPORT_DIR:%/=%}
ifndef REPOSITORY_DIR
  REPOSITORY_DIR := ${CURDIR}/.repository
endif
override REPOSITORY_DIR := ${REPOSITORY_DIR:%/=%}
ifndef IGNORE_DIRS
  IGNORE_DIRS := .essential .build .svn .import .repository
endif
override IGNORE_DIRS := $(patsubst %/,%,$(subst :, ,${IGNORE_DIRS}))
ifndef IMPORT_IGNORE_DIRS
  IMPORT_IGNORE_DIRS := .svn
endif
override IMPORT_IGNORE_DIRS := $(patsubst %/,%,$(subst :, ,${IMPORT_IGNORE_DIRS}))
##-#######################################################################-##

##-#######################################################################-##
# Commands to be used this module:
ifndef WHICH
  WHICH := $(shell which which)
  ifeq ($(strip ${WHICH}),)
    $(error Command which not found)
  endif
endif
ifndef CD
  CD := cd
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
ifndef TEST
  TEST := $(shell ${WHICH} test)
  ifeq ($(strip ${TEST}),)
    $(error Command test not found)
  endif
endif
ifndef TOUCH
  TOUCH := $(shell ${WHICH} touch)
  ifeq ($(strip ${TOUCH}),)
    $(error Command touch not found)
  endif
endif
ifndef CMAKE
  CMAKE := $(shell ${WHICH} cmake)
  ifeq ($(strip ${CMAKE}),)
    $(error Command cmake not found)
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
ifndef FIND
  FIND := $(shell ${WHICH} find)
  ifeq ($(strip ${FIND}),)
    $(error Command find not found)
  endif
endif
ifndef ZSORT
  ZSORT := $(shell ${WHICH} sort)
  ifneq ($(strip ${ZSORT}),)
    ZSORT += -z
  else
    $(error Command sort not found)
  endif
endif
ifndef CZSORT
  ifneq ($(strip ${ZSORT}),)
    CZSORT := LC_ALL=C ${ZSORT}
  else
    $(error Command sort not found)
  endif
endif
ifndef TAR
  TAR := $(shell ${WHICH} tar)
  ifeq ($(strip ${TAR}),)
    $(error Command tar not found)
  endif
endif
ifndef MD5SUM
  MD5SUM := $(shell ${WHICH} md5sum)
  ifeq ($(strip ${MD5SUM}),)
    $(error Command md5sum not found)
  endif
endif
ifndef SVN
  SVN := $(shell ${WHICH} svn)
  ifeq ($(strip ${SVN}),)
    $(warning Command svn not found)
  endif
endif
ifndef SVNEXP
  ifneq ($(strip ${SVN}),)
    SVNEXP := ${SVN} export
  else
    $(warning Command svn not found)
  endif
endif
##-#######################################################################-##

##-#######################################################################-##
# Build Essential default import definitions if needed:
ifndef SVNEXP_ESSENTIAL_REPO_BASE
  SVNEXP_ESSENTIAL_REPO_BASE := http://svn.gso.gilbarco.com/dhr/gvr/eg/pay/spot_common/libcommon
endif
override SVNEXP_ESSENTIAL_REPO_BASE := ${SVNEXP_ESSENTIAL_REPO_BASE:%/=%}
ifndef SVNEXP_ESSENTIAL_REPO_TAG
  SVNEXP_ESSENTIAL_REPO_TAG := trunk
endif
override SVNEXP_ESSENTIAL_REPO_TAG := ${SVNEXP_ESSENTIAL_REPO_TAG:%/=%}
ifndef SVNEXP_ESSENTIAL_REPO_RELPATH
  SVNEXP_ESSENTIAL_REPO_RELPATH := essential
endif
override SVNEXP_ESSENTIAL_REPO_RELPATH := ${SVNEXP_ESSENTIAL_REPO_RELPATH:%/=%}
ifndef SVNEXP_ESSENTIAL_URI
  SVNEXP_ESSENTIAL_URI := ${SVNEXP_ESSENTIAL_REPO_BASE}/${SVNEXP_ESSENTIAL_REPO_TAG}/${SVNEXP_ESSENTIAL_REPO_RELPATH}
endif
override SVNEXP_ESSENTIAL_URI := ${SVNEXP_ESSENTIAL_URI:%/=%}
ifndef SVNEXP_ESSENTIAL_REVISION
  SVNEXP_ESSENTIAL_REVISION := HEAD 
endif

ifdef SVNEXP
  SVNEXP_ESSENTIAL_COMMAND := ${SVNEXP} -r ${SVNEXP_ESSENTIAL_REVISION} "${SVNEXP_ESSENTIAL_URI}"
else
  SVNEXP_ESSENTIAL_COMMAND := { ${SHELL} ${.SHELLFLAGS} 'printf "Error: No command available to import build essential from respository - ${SVNEXP_ESSENTIAL_URI}@${SVNEXP_ESSENTIAL_REVISION}."' && false ; }
endif
##-#######################################################################-##

##-#######################################################################-##
# Spread Makefile infromation:
ifndef SPREAD_MAKEFILE_NAME
  SPREAD_MAKEFILE_NAME := ${MAKEFILE_NAME}
endif

ifndef SPREAD_IGNORE_DIRS
  SPREAD_IGNORE_DIRS := ${IGNORE_DIRS}
else
  SPREAD_IGNORE_DIRS += ${IGNORE_DIRS}
endif
override SPREAD_IGNORE_DIRS := $(patsubst %/,%,$(subst :, ,${SPREAD_IGNORE_DIRS}))

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
# Configuration conditions:
CONDITION_TEST := test check
CONDITION_NOVALGRIND := disable-valgrind release
CONDITION_RELEASE := release
CONDITION_NATIVE := native
CONDITION_NOBUILD := clean-all clean-repository clean-export clean-export-% spread-all spread-check help
CONDITION_NOIMPORT := disable-import
##-#######################################################################-##

##-#######################################################################-##
# Platform information:
ifndef CROSS_PLATFORMS
  CROSS_PLATFORMS := m3 m5 m7 win32
else
  CROSS_PLATFORMS += m3 m5 m7 win32
endif

PLATFORMS := $(filter native ${CROSS_PLATFORMS},${MAKECMDGOALS})

# The idea here is to pre-set the 'native' platform if there is no platform provided
# or if there is at least one platform provided which is not 'native' and the test
# target requested, add the 'native' target in order to run unittests.
#
# Also checks whether no platform provided and the 'clean' target one requested as it
# has a special behaviour cleaning all platforms.
ifeq ($(strip ${PLATFORMS}),)
  ifneq ($(filter-out ${CONDITION_NOBUILD},${MAKECMDGOALS}),)
    PLATFORMS += native
  else
    ifeq ($(strip ${MAKECMDGOALS}),)
      PLATFORMS += native
    endif
  endif
else ifeq ($(filter ${CONDITION_NATIVE},${PLATFORMS}),)
  ifneq ($(filter ${CONDITION_TEST},${MAKECMDGOALS}),)
    PLATFORMS += native
  endif
endif
##-#######################################################################-##

##-#######################################################################-##
# Release/Development status information
ifndef CNFSTAMP
  CNFSTAMP := .stamp
endif

ifneq ($(filter ${CONDITION_RELEASE},${MAKECMDGOALS}),)
  CNFMODE := rls
else
  CNFMODE := dbg
endif

IMPORT_HASH := 
ifeq ($(filter ${CONDITION_NOIMPORT},${MAKECMDGOALS}),)
  ifdef IMPORT_DIRS
    ifneq ($(strip $(wildcard ${IMPORT_DIRS})),)
      ifndef IMPORT_IGNORE_DIRS
        IMPORT_IGNORE_DIRS := .svn
      endif
      override IMPORT_IGNORE_DIRS := $(patsubst %/,%,$(subst :, ,${IMPORT_IGNORE_DIRS}))
      IMPORT_HASH := $(firstword $(shell ${FIND} ${IMPORT_DIRS}$(foreach IMPORT_IGNORE_DIR,IMPORT_IGNORE_DIRS, -not \( -name ${IMPORT_IGNORE_DIR} -prune \)) -print0 | ${CZSORT} | ${TAR} -P -cf - --null -T - --no-recursion | ${MD5SUM}))
    endif
  endif
endif

ifneq ($(filter ${CONDITION_NATIVE},${PLATFORMS}),)
  ifeq ($(filter ${CONDITION_NOVALGRIND},${MAKECMDGOALS}),)
    CNFVALGRIND := vlgrnd
  endif
endif

$(foreach PLATFORM,${PLATFORMS},$(eval CNFSTAMP_${PLATFORM} := ${CNFSTAMP}$(if ${CNFMODE},_${CNFMODE})$(if ${IMPORT_HASH},_${IMPORT_HASH})$(if ${CNFVALGRIND},_${CNFVALGRIND})))
##-#######################################################################-##

##-#######################################################################-##
# Configuration flags for CMAKE:
ifndef GVR_MODULE_PATH
  ifdef GVR_ESSENTIAL_RELATIVE_MODULE_PATH
    GVR_MODULE_PATH := "${ESSENTIAL_DIR}/${GVR_ESSENTIAL_RELATIVE_MODULE_PATH}"
  endif
endif
override GVR_MODULE_PATH := ${GVR_MODULE_PATH:%/=%}
ifndef GVR_TOOLCHAIN_PATH
  ifdef GVR_ESSENTIAL_RELATIVE_TOOLCHAIN_PATH
    GVR_TOOLCHAIN_PATH := "${ESSENTIAL_DIR}/${GVR_ESSENTIAL_RELATIVE_TOOLCHAIN_PATH}"
  endif
endif
override GVR_TOOLCHAIN_PATH := ${GVR_TOOLCHAIN_PATH:%/=%}

ifndef CMAKE_FLAGS
  CMAKE_FLAGS := 
endif

ifneq ($(filter ${CONDITION_NOIMPORT},${MAKECMDGOALS}),)
  CMAKE_FLAGS += -DGVR_DISABLE_IMPORT=ON
else
  CMAKE_FLAGS += -DGVR_AUTO_IMPORTED_EXTERNAL_PREFIX_LOCATION=ON
endif

ifeq ($(filter ${CONDITION_RELEASE},${MAKECMDGOALS}),)
  CMAKE_FLAGS += -DGVR_DEBUG=ON -DCMAKE_BUILD_TYPE=DEBUG
else
  CMAKE_FLAGS += -DGVR_DEBUG=OFF -DCMAKE_BUILD_TYPE=RELEASE
endif

$(foreach PLATFORM,${PLATFORMS},$(eval CMAKE_${PLATFORM}_FLAGS := ))

ifdef IMPORT_DIRS
  $(foreach PLATFORM,${PLATFORMS},$(eval CMAKE_${PLATFORM}_FLAGS += -DGVR_REMOTE_IMPORT_LOCATIONS="$(patsubst %,;%/${PLATFORM};,${IMPORT_DIRS})"))
endif
ifdef EXPORT_DIR
  $(foreach PLATFORM,${PLATFORMS},$(eval CMAKE_${PLATFORM}_FLAGS += -DGVR_REMOTE_EXPORT_LOCATION=${EXPORT_DIR}/${PLATFORM}))
endif
ifdef REPOSITORY_DIR
  $(foreach PLATFORM,${PLATFORMS},$(eval CMAKE_${PLATFORM}_FLAGS += -DGVR_REPOSITORY_REMOTE=${REPOSITORY_DIR}/${PLATFORM}))
endif

ifneq ($(filter ${CONDITION_NATIVE},${PLATFORMS}),)
  CMAKE_native_FLAGS += -DGVR_RUN_TEST=ON
  ifeq ($(filter ${CONDITION_NOVALGRIND},${MAKECMDGOALS}),)
    CMAKE_native_FLAGS += -DGVR_RUN_VALGRIND_TEST=ON
  endif
endif

ifdef GVR_MODULE_PATH
  CMAKE_FLAGS += -DGVR_MODULE_PATH=${GVR_MODULE_PATH}
endif

ifdef GVR_TOOLCHAIN_PATH
  $(foreach PLATFORM,$(filter ${CROSS_PLATFORMS},${PLATFORMS}),$(eval CMAKE_${PLATFORM}_FLAGS += -DCMAKE_TOOLCHAIN_FILE="${GVR_TOOLCHAIN_PATH}/GvrToolchain_${PLATFORM}.cmake"))
endif

ifdef GVR_USE_REVISION
  ifeq ($(filter ${CONDITION_RELEASE},${MAKECMDGOALS}),)
    CMAKE_FLAGS += -DGVR_USE_REVISION="${GVR_USE_REVISION}"
  endif
endif

##-#######################################################################-##

##-#######################################################################-##
# Rules:
ifndef TARGETS_HELP
  TARGETS_HELP :=
else
  TARGETS_HELP := ${TARGETS_HELP}
endif
TARGETS_HELP += \tdefault: Default target.\n

##-####################-##
#  * Targets Dummy (This targets mostly tweaks configurations):
TARGETS_DUMMY := release debug ${MAKEFILE_NAME} disable-valgrind disable-import

# Target 'release' is a hack to force rebuild specified platforms if previously built as debug.
# Target 'debug' is a hack to force rebuild specified platforms if previously built as release.
# Target '${MAKEFILE_NAME}' is a hack due to an issue when making '%' depend on a .PHONY target. Even though the '${MAKEFILE_NAME}' is not counted in the ${MAKECMDGOALS} it is oddly requested while doing make.
# Target 'disable-valgrind' is a hack to force test specified native platform without valgrind support.
# Target 'disable-import' is a hack to force build specified platforms without external build imports.
$(foreach TARGET_DUMMY, ${TARGETS_DUMMY}, $(eval ${TARGET_DUMMY}: ; @true))
TARGETS_HELP += \trelease: Force specified platform to be configured in release mode. Takes precedence over debug.\n
TARGETS_HELP += \tdebug: Force specified platform to be configured in debug mode.\n
TARGETS_HELP += \tdisable-valgrind: Disable valgrind usage in tests.\n
TARGETS_HELP += \tdisable-import: Force build specified platforms without external build imports\n
##-####################-##
##-####################-##
#  * Targets Extra (This targets does not require build or configuration steps):
TARGETS_EXTRA := clean-all clean-repository clean-export clean-export-% spread-all spread-check help

# This target cleans an specific platform only if provided. Cleans all platforms if no specific one is given.
clean-all:
	@$(if $(strip ${BUILD_DIR}),${RRM} $(or $(foreach PLATFORM,${PLATFORMS},"${BUILD_DIR}/${PLATFORM}"),"${BUILD_DIR}"),true)
TARGETS_HELP += \tclean-all: Cleans the build cache.\n

# This target cleans an specific platform repository only if provided. Cleans all platform repositories if no specific one is given.
clean-repository:
	@$(if $(strip ${REPOSITORY_DIR}),${RRM} $(or $(foreach PLATFORM,${PLATFORMS},"${REPOSITORY_DIR}/${PLATFORM}"),"${REPOSITORY_DIR}"),true)
TARGETS_HELP += \tclean-repository: Cleans the repository location.\n

# This target cleans an specific platform export only if provided. Cleans all platform exports if no specific one is given.
clean-export:
	@$(if $(strip ${EXPORT_DIR}),${RRM} $(or $(foreach PLATFORM,${PLATFORMS},"${EXPORT_DIR}/${PLATFORM}"),"${EXPORT_DIR}"),true)
TARGETS_HELP += \tclean-export: Cleans the export location for the specified platforms.\n

# This target cleans a specific project on a specific platform export only if provided. Cleans a specific project on all platform exports if no specific one is given.
clean-export-%:
	@$(if $(strip ${EXPORT_DIR}),${TEST} -n "$*" && ${RRM} $(foreach PLATFORM,$(or ${PLATFORMS},native ${CROSS_PLATFORMS}),"${EXPORT_DIR}/${PLATFORM}/$*"),true)
TARGETS_HELP += \tclean-export-*: Cleans the export location for the specified project in the specified platforms.\n

# This target enforces Makefiles integrity in subdirectories named after this makefile with the same SPREAD_TAG.
spread-all:
	@$(or ${SPREAD_COMMAND},false)
TARGETS_HELP += \tspread-all: Enforces Makefiles integrity in subdirectories named after this makefile with the same spread '${SPREAD_TAG}' tag.\n

# This target checks Makefiles integrity in subdirectories named after this makefile with the same SPREAD_TAG.
spread-check spread-test:
	@echo
	@echo "Checking Makefiles integrity for spread '${SPREAD_TAG}' tag..."
	@echo
	@$(or ${SPREAD_CHECK_COMMAND},false)
TARGETS_HELP += \tspread-check: Checks Makefiles integrity in subdirectories named after this makefile with the same spread '${SPREAD_TAG}' tag.\n
TARGETS_HELP += \tspread-test: Test Makefiles integrity in subdirectories named after this makefile with the same spread '${SPREAD_TAG}' tag.\n

help:
	@${SHELL} ${.SHELLFLAGS} 'printf "Targets:\n${TARGETS_HELP}"'
TARGETS_HELP += \thelp: Print this message.\n
##-####################-##
##-####################-##
#  * Targets Build (This targets requires build or configuration steps):
###########
# This replaces the svn call to bring the essential directory for a local copy from another directory.
#
#ifndef CP
#  CP := $(shell ${WHICH} cp)
#  ifneq ($(strip ${CP}),)
#    CP += -r
#  else
#    $(error Command cp not found)
#  endif
#endif
#override SVNEXP_ESSENTIAL_COMMAND := ${CP} /local/path/to/essential
###########
# This target checks the existance of the essential directory or download it if not found.
${ESSENTIAL_DIR}/.essential:
	@{ ${TEST} -f "${ESSENTIAL_DIR}/.essential" || { { ${TEST} ! -e "${ESSENTIAL_DIR}" || ${RRM} "${ESSENTIAL_DIR}" ; } && ${SVNEXP_ESSENTIAL_COMMAND} "${ESSENTIAL_DIR}" ; } ; }

# This template provides the generic target and configuration rules for the to be specified platform.
define CONFIGURATION_RULE
$${BUILD_DIR}/${1}/$${CNFSTAMP_${1}}: ${ESSENTIAL_DIR}/.essential
	@{ $${TEST} ! -e "$${BUILD_DIR}/${1}" || { $${TEST} -d "$${BUILD_DIR}/${1}" && $${RRM} "$${BUILD_DIR}/${1}" ; } ; } && $${MKDIR} "$${BUILD_DIR}/${1}" && $${CD} "$${BUILD_DIR}/${1}" && $${CMAKE} $${CMAKE_FLAGS} $${CMAKE_${1}_FLAGS} "$${SOURCE_DIR}"
	@$${TEST} -e "$${BUILD_DIR}/${1}/Makefile" && $${TOUCH} $$@
endef

# This line provides the targets and configuration rules of each specified platform.
$(foreach PLATFORM,${PLATFORMS},$(if $(and $(strip ${BUILD_DIR}),$(strip ${SOURCE_DIR})),$(eval $(call CONFIGURATION_RULE,${PLATFORM}))))

# This line is simply making empty targets from the platforms taking into account that if no other target is included
# besides the extra ones (which not require configuring or building), they should depend on the configuration stamp in order to force the configuration step.
$(foreach PLATFORM,${PLATFORMS},$(if $(and $(strip ${BUILD_DIR}),$(strip ${CNFSTAMP_${PLATFORM}})),$(eval ${PLATFORM}: $(if $(or $(filter-out ${TARGETS_EXTRA},${MAKECMDGOALS}),$(if ${MAKECMDGOALS},,${PLATFORMS})),${BUILD_DIR}/${PLATFORM}/${CNFSTAMP_${PLATFORM}}); @true)))
TARGETS_HELP += \tnative ${CROSS_PLATFORMS}: Optional. Perform make target over specified platform. Default: native\n

# This target forces the configuration of each specified platform.
configure: ${PLATFORMS}; @true
TARGETS_HELP += \tconfigure: Perform configuration step.\n

# This generic target pattern is a pass-through target to the configured platform build tree.
%: configure
	@$(or $(if $(strip ${BUILD_DIR}),$(foreach PLATFORM,${PLATFORMS},${MAKE} -C "${BUILD_DIR}/${PLATFORM}" -f Makefile $@ || exit $$?;)),false)

# This target is a pass-through target matching the 'all' target. The 'all' target will be handled by the generic target pattern.
build: all; @true
TARGETS_HELP += \tall: Performs the build.\n
TARGETS_HELP += \tbuild: Performs the build.\n

# Target 'export' forces the build of each specified platform in order to perform the export of all its projects.
export: build
	@$(or $(if $(strip ${BUILD_DIR}),$(foreach PLATFORM,${PLATFORMS},${MAKE} -C "${BUILD_DIR}/${PLATFORM}" -f Makefile $@ || exit $$?;)),false)
TARGETS_HELP += \texport: Performs a build export.\n

# This target pattern forces the build of each specified platform in order to perform the export of the specified project.
export-%: build
	@$(or $(if $(strip ${BUILD_DIR}),$(foreach PLATFORM,${PLATFORMS},${MAKE} -C "${BUILD_DIR}/${PLATFORM}" -f Makefile $@ || exit $$?;)),false)
TARGETS_HELP += \texport-*: Performs a build export for the specified project.\n

# This target forces the build of each specified platform in order to perform the unittests.
test: build
	@$(if $(strip ${BUILD_DIR}),${MAKE} -C "${BUILD_DIR}/native" -f Makefile $@,false)
TARGETS_HELP += \ttest: Run unittests.\n

# This target is a pass-through target matching the standard 'check' target to the 'test' target, which is the one cmake uses to run unittests.
check: test; @true
TARGETS_HELP += \tcheck: Run unittests.\n

tarball: install
	$(or $(if $(strip ${REPOSITORY_DIR}),$(foreach PLATFORM,${PLATFORMS}, ${CD} "${REPOSITORY_DIR}/${PLATFORM}/install" && echo 'UNTAR_PATH=`pwd`;export PATH=$$PATH:$$UNTAR_PATH/opt/gilbarco/bin;export LD_LIBRARY_PATH=$$UNTAR_PATH/opt/gilbarco/lib:$$UNTAR_PATH/opt/gilbarco/3rdparty/lib' > set_lib_path.sh;)),false)
	$(or $(if $(strip ${REPOSITORY_DIR}),$(foreach PLATFORM,${PLATFORMS}, ${CD} "${REPOSITORY_DIR}/${PLATFORM}/install" && tar -cvjf ../../libcommon_${PLATFORM}.tar.bz2 ./;)),false)
TARGETS_HELP += \ttarball: Generate tarball package that contains binaries for QA team to run tests.\n

##-####################-##
##-#######################################################################-##

##-#######################################################################-##
# Targets:
MAKEFILE_TARGETS := default ${TARGETS_DUMMY} ${TARGETS_EXTRA} ${PLATFORMS} configure build export test check

ifndef TARGETS
  TARGETS := ${MAKEFILE_TARGETS} $(filter-out ${PLATFORMS},native ${CROSS_PLATFORMS})
else
  TARGETS += ${MAKEFILE_TARGETS} $(filter-out ${PLATFORMS},native ${CROSS_PLATFORMS})
endif
##-#######################################################################-##

# Enable dependency checking
.KEEP_STATE:
.KEEP_STATE_FILE:.make.state.SPOT

.PHONY: ${MAKEFILE_TARGETS}
##-#######################################################################-##
