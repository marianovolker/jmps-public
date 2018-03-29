################################################################################################
## ENVIRONMENT VARIABLES THAT CONTROLLING THE BUILD   
##     
## DM_DIR_BIN = /opt/displaymanager/bin
## DM_DIR_CFG = /etc/opt/displaymanager
## DM_DIR_PER = /opt/displaymanager/personality
## DM_DIR_LOG = /var/opt/displaymanager/log
## DM_DIR_XML = /var/opt/displaymanager/xml
## DM_DIR_IMG = /var/opt/displaymanager/images
## DM_DIR_RUN = /var/opt/displaymanager/run
## DM_DIR_IND = /etc/init.d/
## NGP_DIR_CFG= /etc/config/
##
## USE_FREE_ARM_COMPILER	If set, free ARM toolchain will be used
## RELEASE					Build binaries with no debug information
## DM_BUILD_TYPE			Build type.
##
################################################################################################
## USEFUL TARGETS SUPPORTED BY THIS MAKEFILE
##
## (Platform specific makefile targets)
##
## x86					Builds x86 binaries.
##
## x86-test				Builds x86 binaries, runs all unit tests and install application.
##
## x86-makefile			Generates Qt makefile for x86.
##
## x86-clean			Calls a distclean on Qt makefiles.
##
## x86-deb				Builds x86 displaymanager debian package.
##
## arm					Builds arm binaries.
##
## arm-makefile			Generates Qt makefile for arm.
##     
## arm-clean			Calls a distclean on Qt makefiles.
##
## arm-deb				Builds arm displaymanager debian package.
##
##
##
## (Platform independent makefile targets)
##
## test-all-builds		Builds for all architectures. First for x86, runs unit tests and then builds for arm.
##
## cm-build				Same as test-all-builds target. Prints a message after finishing successfully.
##
## install				Calls install on Qt makefiles.
##
## unit-tests			Builds for requested architecture and then runs unit tests.
##
## clean				Calls a clean on Qt makefiles.
##
## distclean-nodep		Calls a distclean on Qt makefiles. It doesn't depend on Qt make file existence.
##
## distclean			Calls a distclean on Qt makefiles.
##
## complete_install		Calls install on ./config/ makefile to copy config files into debian package.
##						Do the complete install.
##
################################################################################################
     
## Export new environment variables. 
################################################################################################
DM_DIR_PKG=../debian/displaymanager
export DM_DIR_PKG

DM_INSTALL_DIR=${DM_DIR_PKG}/${DM_DIR_BIN}
export DM_INSTALL_DIR
   
DM_RELEASE=${RELEASE}
ifneq ($(strip $(DM_RELEASE)),)
	export DM_RELEASE
endif

QTMAKEFILE=Makefile.qt
QTPRO=displaymanager.pro

QT_PKG_BASE=4.5.2
X86_QT_PKG_REQ_VER=${QT_PKG_BASE}-2
QT_PKG_X86=gvr-qt-embedded-lgpl
QMAKE_X86=/usr/local/Trolltech/Qt-embedded-${QT_PKG_BASE}-qvfb/bin/qmake
QFONTS_X86=/usr/local/Trolltech/Qt-embedded-${QT_PKG_BASE}-qvfb/lib/fonts
RPATH_X86=/usr/local/Trolltech/Qt-embedded-${QT_PKG_BASE}-qvfb/lib/
BUILD_FILE_FLAG_X86=.X86-build
export QFONTS_X86
export RPATH_X86
export BUILD_FILE_FLAG_X86

ARM_QT_PKG_REQ_VER=${QT_PKG_BASE}-0
QT_PKG_ARM_CROSS=arm-gvr-qt-embedded-lgpl
QMAKE_ARM_CROSS=/usr/arm/Qt-embedded-${QT_PKG_BASE}-armel/bin/qmake
PATH_ARMLIBS=/usr/arm/Qt-embedded-${QT_PKG_BASE}-armel/ARMLIBS/lib/
QFONTS_ARM=/usr/local/Trolltech/Qt-embedded-${QT_PKG_BASE}-armel/lib/fonts
RPATH_ARM=/usr/local/Trolltech/Qt-embedded-${QT_PKG_BASE}-armel/lib/
BUILD_FILE_FLAG_ARM=.ARM-build
export QFONTS_ARM
export RPATH_ARM
export PATH_ARMLIBS
export BUILD_FILE_FLAG_ARM

## Choose free / nonfree options - always use the nonfree version if
## available unless USE_FREE_ARM_COMPILER is set with any value.
ARM_USE_FREE = $(if $(USE_FREE_ARM_COMPILER),YES,$(if $(wildcard $(ARM_NONFREE_CROSS_COMPILE)*),,YES))

## Cross compile options (depending on free/nonfree compiler)
ARM_FREE_CROSS_COMPILE=/opt/arm_lite/bin/qt/
ARM_NONFREE_CROSS_COMPILE=/opt/arm_pro/bin/qt/
ARM_CROSS_COMPILE=$(if $(ARM_USE_FREE),$(ARM_FREE_CROSS_COMPILE),$(ARM_NONFREE_CROSS_COMPILE))
PATH:=${ARM_CROSS_COMPILE}:${PATH}
export PATH

ifneq ($(strip $(DM_RELEASE)),)
	export CPPFLAGS	= -pipe -Werror -Wall -W -O3 -fPIC -fvisibility=hidden -fno-rtti
	export CXXFLAGS	= $(CPPFLAGS)
	export CFLAGS	= $(CPPFLAGS)
	export FFLAGS	= $(CPPFLAGS)	
	export LDFLAGS 	= -Wl,-O3
else
	export CPPFLAGS	= -pipe -Werror -Wall -W -fPIC -g -fno-rtti
	export CXXFLAGS	= $(CPPFLAGS)
	export CFLAGS	= $(CPPFLAGS)
	export FFLAGS	= $(CPPFLAGS)	
	export LDFLAGS 	= -Wl,-O1,-Bsymbolic-functions
endif
################################################################################################

################################################################################################
.PHONY: all
all:
	@echo "   --------------------------------------------------------------------"
	@echo "   Doing nothing for target 'all' or default. Choose one of the following targets:"
	@echo "   --------------------------------------------------------------------"	
	@echo "   x86"
	@echo "   x86-test"
	@echo "   test"
	@echo "   x86-makefile"
	@echo "   x86-clean"
	@echo "   x86-deb"
	@echo "   deb"
	@echo "   --------------------------------------------------------------------"	
	@echo "   arm"
	@echo "   arm-makefile"
	@echo "   arm-clean"
	@echo "   arm-deb"
	@echo "   deb-arm"
	@echo "   --------------------------------------------------------------------"
	@echo "   test-all-builds"
	@echo "   cm-build"
	@echo "   unit-tests"
	@echo "   clean"
	@echo "   distclean"
	@echo "   --------------------------------------------------------------------"
################################################################################################

################################################################################################
.PHONY: test
test: x86-test

.PHONY: deb
deb: x86-deb
   
.PHONY: deb-arm
deb-arm: arm-deb
################################################################################################

## Platform x86 specific makefile targets.
################################################################################################
.PHONY: x86
x86: x86-makefile
	@echo "TARGET====>x86<====(${DM_BUILD_TYPE})" 
	@$(MAKE) -f ${QTMAKEFILE}

.PHONY: x86-makefile
x86-makefile: x86-dependency-check
	@echo "TARGET====>x86-makefile<====(${DM_BUILD_TYPE})"
	@touch ${BUILD_FILE_FLAG_X86}

.PHONY: x86-test
x86-test: x86
	@echo "TARGET====>x86-test<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE} install

.PHONY: x86-clean
x86-clean: x86-makefile
	@echo "TARGET====>x86-clean<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE} clean
	@rm ${BUILD_FILE_FLAG_X86}

.PHONY: x86-dependency-check
x86-dependency-check: x86-dependency-check-pkg x86-dependency-check-qmake x86-dependency-check-qtmakefile
	@echo "TARGET====>x86-dependency-check<====(${DM_BUILD_TYPE})"

.PHONY: x86-dependency-check-pkg
x86-dependency-check-pkg:
	@echo "TARGET====>x86-dependency-check-pkg<====(${DM_BUILD_TYPE})"
	@CUR_VER=`dpkg-query -W -f='$${Version}' ${QT_PKG_X86}` && \
	dpkg --compare-versions "$$CUR_VER" ge "${X86_QT_PKG_REQ_VER}" || \
	(echo "ERROR expected ${QT_PKG_X86} minimum version ${X86_QT_PKG_REQ_VER} but have only version $$CUR_VER" && \
	exit 1)

.PHONY: x86-dependency-check-qmake
x86-dependency-check-qmake:
	@echo "TARGET====>x86-dependency-check-qmake<====(${DM_BUILD_TYPE})"
	@if [ ! -f ${QMAKE_X86} ] ; then echo "ERROR could not find qmake at ${QMAKE_X86}"; exit 1; fi

.PHONY: x86-dependency-check-qtmakefile
x86-dependency-check-qtmakefile:
	@echo "TARGET====>x86-dependency-check-qtmakefile<====(${DM_BUILD_TYPE})"
	@[ -f ${QTMAKEFILE} ] || [ "`${QMAKE_X86} ${QTPRO}`" = "" ] 

.PHONY: x86-deb
x86-deb: distclean
	@echo "TARGET====>x86-deb<====(X86)"
	@chmod +x debian/rules
	@dpkg-buildpackage -b -uc -us -rfakeroot
	@rm -rf ../*.dsc
	@rm -rf ../*.tar.gz
	@rm -rf ../*.changes
	@-rm -rf debs
	@mkdir debs
	@mv ../displaymanager*.deb debs/
################################################################################################

## Platform arm specific makefile targets.
################################################################################################
.PHONY: arm
arm: arm-makefile
	@echo "TARGET====>arm<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE}

.PHONY: arm-makefile
arm-makefile: arm-dependency-check
	@echo "TARGET====>arm-makefile<====(${DM_BUILD_TYPE})"
	@touch ${BUILD_FILE_FLAG_ARM}

.PHONY: arm-clean
arm-clean: arm-makefile
	@echo "TARGET====>arm-clean<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE} clean
	@rm ${BUILD_FILE_FLAG_ARM}

.PHONY: arm-dependency-check
arm-dependency-check: arm-dependency-check-pkg arm-dependency-check-qmake arm-dependency-check-qtmakefile
	@echo "TARGET====>arm-dependency-check<====(${DM_BUILD_TYPE})"

.PHONY: arm-dependency-check-pkg
arm-dependency-check-pkg:
	@echo "TARGET====>arm-dependency-check-pkg<====(${DM_BUILD_TYPE})"
	@CUR_VER=`dpkg-query -W -f='$${Version}' ${QT_PKG_ARM_CROSS}` && \
	dpkg --compare-versions "$$CUR_VER" ge "${ARM_QT_PKG_REQ_VER}" || \
	(echo "ERROR expected ${QT_PKG_ARM_CROSS} minimum version ${ARM_QT_PKG_REQ_VER} but have only version $$CUR_VER" && \
	exit 1)

.PHONY: arm-dependency-check-qmake
arm-dependency-check-qmake:
	@echo "TARGET====>arm-dependency-check-qmake<====(${DM_BUILD_TYPE})"
	@[ -f ${QMAKE_ARM_CROSS} ]
	
.PHONY: arm-dependency-check-qtmakefile
arm-dependency-check-qtmakefile:
	@echo "TARGET====>arm-dependency-check-qtmakefile<====(${DM_BUILD_TYPE})"
	@[ -f ${QTMAKEFILE} ] || [ "`${QMAKE_ARM_CROSS} ${QTPRO}`" = "" ]

.PHONY: arm-deb
arm-deb: 
	@echo "TARGET====>arm-deb<====(ARM)"
	@dpkg-buildpackage -b -uc -us -rfakeroot -aarmel
	@-rm -rf ../*.dsc
	@-rm -rf ../*.tar.gz
	@-rm -rf ../*.changes
	@-rm -rf debs
	@mkdir debs
	@mv ../displaymanager*.deb debs/
################################################################################################

## Platform independent makefile targets.
################################################################################################
${QTMAKEFILE}:
	@echo "TARGET====>QTMAKEFILE<====(${DM_BUILD_TYPE})"
	$(error Missing '${QTMAKEFILE}'. Need to generate it first)

.PHONY: test-all-builds
test-all-builds: distclean-nodep x86 unit-tests distclean arm
	@echo "TARGET====>test-all-builds<====(${DM_BUILD_TYPE})"

.PHONY: cm-build
cm-build: test-all-builds
	@echo "TARGET====>cm-build<====(${DM_BUILD_TYPE})"
	@echo ""
	@echo "+++++++++++++++++++++++++++++++"
	@echo "+++ Build done successfully +++"
	@echo "+++++++++++++++++++++++++++++++"
	@echo ""
	
.PHONY: install
install: ${QTMAKEFILE}
	@echo "TARGET====>install<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE} $@

.PHONY: unit-tests
unit-tests: x86-makefile ${QTMAKEFILE}
	@echo "TARGET====>unit-tests<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE} utests

.PHONY: clean
clean: x86-makefile ${QTMAKEFILE}
	@echo "TARGET====>clean<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE} $@

.PHONY: distclean
distclean: x86-makefile ${QTMAKEFILE}
	@echo "TARGET====>distclean<====(${DM_BUILD_TYPE})"
	@$(MAKE) -f ${QTMAKEFILE} clean
	@$(MAKE) -f ${QTMAKEFILE} distclean
	@-rm ${BUILD_FILE_FLAG_X86} 2>/dev/null
	@-rm ${BUILD_FILE_FLAG_ARM} 2>/dev/null
	@-rm -rf ./debian/displaymanager 2>/dev/null
ifneq ($(strip ${DM_INSTALL_DIR}),)
	@-rm -rf ${DM_INSTALL_DIR}
endif

.PHONY: distclean-nodep
distclean-nodep:
	@echo "TARGET====>distclean-nodep<====(${DM_BUILD_TYPE})"
	@find ./ -name "${QTMAKEFILE}" -exec rm -rf '{}' \; 2>/dev/null;echo "done."
	@find ./ -name "*.o" -exec rm -rf '{}' \; 2>/dev/null;echo "done."
	@find ./ -name "*~" -exec rm -rf '{}' \; 2>/dev/null;echo "done."
	@find ./ -name "core" -exec rm -rf '{}' \; 2>/dev/null;echo "done."
	@find ./ -name "*.core" -exec rm -rf '{}' \; 2>/dev/null;echo "done."
	@find ./ -name "core.*" -exec rm -rf '{}' \; 2>/dev/null;echo "done."
	@-rm ${BUILD_FILE_FLAG_X86} 2>/dev/null
	@-rm ${BUILD_FILE_FLAG_ARM} 2>/dev/null
	@-rm -rf ./debian/displaymanager 2>/dev/null
ifneq ($(strip ${DM_INSTALL_DIR}),)
	@-rm -rf ${DM_INSTALL_DIR}
endif

.PHONY: complete_install
complete_install: install
	@echo "TARGET====>complete_install<====(${DM_BUILD_TYPE})"
	@$(MAKE) -C config install_config
################################################################################################
