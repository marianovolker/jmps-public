## Environment vars
################################################################################################
QMAKE_MAKEFILE=Makefile.qt
DM_RELEASE=$$(DM_RELEASE)
DM_INSTALL_DIR=$$(DM_INSTALL_DIR)
DM_RPATH_X86=$$(RPATH_X86)
DM_RPATH_ARM=$$(RPATH_ARM)
DM_PATH_ARMLIBS=$$(PATH_ARMLIBS)
################################################################################################

## GENERIC COMPILATION AND LINKING FLAGS
################################################################################################
QMAKE_CXXFLAGS				= -pipe -Werror -fno-rtti
QMAKE_CXXFLAGS_DEPS			= -M
QMAKE_CXXFLAGS_WARN_ON		= -Wall -W
QMAKE_CXXFLAGS_WARN_OFF		= -w
QMAKE_CXXFLAGS_RELEASE		= -O3 
QMAKE_CXXFLAGS_DEBUG		= -g
QMAKE_CXXFLAGS_SHLIB  		= -fPIC
QMAKE_CXXFLAGS_STATIC_LIB	= -fPIC
QMAKE_CXXFLAGS_HIDESYMS		= -fvisibility=hidden

QMAKE_CFLAGS				= $$QMAKE_CXXFLAGS
QMAKE_CFLAGS_DEPS			= $$QMAKE_CXXFLAGS_DEPS
QMAKE_CFLAGS_WARN_ON		= $$QMAKE_CXXFLAGS_WARN_ON
QMAKE_CFLAGS_WARN_OFF		= $$QMAKE_CXXFLAGS_WARN_OFF
QMAKE_CFLAGS_RELEASE		= $$QMAKE_CXXFLAGS_RELEASE 
QMAKE_CFLAGS_DEBUG			= $$QMAKE_CXXFLAGS_DEBUG
QMAKE_CFLAGS_SHLIB  		= $$QMAKE_CXXFLAGS_SHLIB
QMAKE_CFLAGS_STATIC_LIB		= $$QMAKE_CXXFLAGS_STATIC_LIB
QMAKE_CFLAGS_HIDESYMS		= $$QMAKE_CXXFLAGS_HIDESYMS

QMAKE_LFLAGS				=
QMAKE_LFLAGS_RELEASE		= -Wl,-O3
QMAKE_LFLAGS_SHLIB			= -shared
QMAKE_LFLAGS_SONAME			= -Wl,-soname,
QMAKE_LFLAGS_NOUNDEF		= -Wl,-O1,--no-undefined
QMAKE_RPATH					= ""
QMAKE_RPATHDIR				= ""
QMAKE_LFLAGS_RPATH			= ""

linux-x86-g++{
QMAKE_LFLAGS 			   += -Wl,-rpath,$$DM_RPATH_X86
}

linux-armv7-g++{
QMAKE_LFLAGS			   += -L$$DM_PATH_ARMLIBS -Wl,-rpath,$$DM_RPATH_ARM
}
################################################################################################

########################################################################
# Use install rule to run test application.
# This lets you do 'make install' on a test to both build and run it,
# and lets you easily build and run all tests from the parent directory.
# ----------------------------------------------------------------------

CONFIG  += qtestlib
QT -= gui

contains (TEMPLATE, subdirs) {
	for( _subdir_, SUBDIRS ) {
		testtarget = $$join(_subdir_,1,,-unit-tests)
		unittest.target += $$testtarget
		utests.depends += $$testtarget
	}
	unittest.depends = all
	unittest.commands = @cd $(subst -unit-tests,,$@) && $(MAKE) -f $(MAKEFILE) utests
	QMAKE_EXTRA_TARGETS += unittest

	utests.target = utests
	QMAKE_EXTRA_TARGETS += utests
}

!contains (TEMPLATE, subdirs) {
	contains (TEMPLATE, app) {
		## Add test target. It will just call runme target and only run unit tests.
		utests.depends = runme
		utests.target = utests
		QMAKE_EXTRA_TARGETS *= utests

		runme_exec.target = runme_exec
		runme_exec.depends = $(QMAKE_TARGET)
		!isEmpty(DESTDIR): runme_exec.commands = cd ./$(DESTDIR) &&
		macx:      runme_exec.commands += ./$(QMAKE_TARGET).app/Contents/MacOS/$(QMAKE_TARGET)
		else:unix: runme_exec.commands += @export QTEST_COLORED=1; ./$(QMAKE_TARGET)
		else:      runme_exec.commands += $(QMAKE_TARGET)
		embedded:  runme_exec.commands += -qws

		runme.files =
		runme.path = .
		runme.target = runme
		runme.depends = runme_exec
		runme.commands = @echo -n "\"********* UnitTest PASSED ********\\n\""

		QMAKE_EXTRA_TARGETS *= runme_exec runme
		INSTALLS *= runme
	}
}

## Get release build from env, if not present assume debug build
!isEmpty( DM_RELEASE ) {
        CONFIG = $$replace(CONFIG,debug,)
	CONFIG += release;
} else {
        CONFIG = $$replace(CONFIG,release,)
	CONFIG += debug;
}
