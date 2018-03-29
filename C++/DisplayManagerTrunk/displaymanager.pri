## Environment vars
###############################################################################################
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

## Do not include unit test for cross compilation, only for native x86
contains (TEMPLATE, subdirs) {
	linux-g++ {		
		exists($$OUT_PWD/test/test.pro) {
			SUBDIRS += test   
		}
	}	
	linux-x86-g++ {		
		exists($$OUT_PWD/test/test.pro) {
			SUBDIRS += test   
		}
	}	
	## Add dummy utests target for running unit tests
	for( _subdir_, SUBDIRS ) {
		!contains( _subdir_, src ) {
			testtarget = $$join(_subdir_,1,,-unit-tests)
			unittest.target += $$testtarget
			utests.depends += $$testtarget
		}
	}
	unittest.depends = all
	unittest.commands = @cd $(subst -unit-tests,,$@) && $(MAKE) -f $(MAKEFILE) utests
	QMAKE_EXTRA_TARGETS += unittest

	utests.target = utests
	QMAKE_EXTRA_TARGETS += utests	
}

!contains (TEMPLATE, subdirs) {
	## Handle application install
	!isEmpty( DM_INSTALL_DIR ) {
		contains (TEMPLATE, app) {
		INSTALL_DIR = $$DM_INSTALL_DIR
		target.path = $$INSTALL_DIR
		INSTALLS += target
		}
	}
	## Handle library install.
	## Static libraries will not be installed by default.
	contains (TEMPLATE, lib) {
		!isEmpty( DM_INSTALL_DIR ) {
			!contains (CONFIG, static) {
				INSTALL_DIR = $$DM_INSTALL_DIR
				target.path = $$INSTALL_DIR
				INSTALLS += target
			}
			contains (CONFIG, static) {
				install.depends = "$(TARGET)"
				QMAKE_EXTRA_TARGETS += install
			}	  
		}
	}

	## Add dummy utests target
	## Do nothing -- avoid makefile warnings
	utests.target = utests
	QMAKE_EXTRA_TARGETS += utests	
}

## Make sure even for install target, we'll be building libraries
contains (TEMPLATE, lib) {
	build_all.target = build_all
	build_all.depends = $(QMAKE_TARGET)
	build_all.path = .

	QMAKE_EXTRA_TARGETS *= build_all
	INSTALLS *= build_all
}

## Get release build from env, if not present assume debug build
!isEmpty( DM_RELEASE ) {
        CONFIG = $$replace(CONFIG,debug,)
	CONFIG += release;
} else {
        CONFIG = $$replace(CONFIG,release,)
	CONFIG += debug;
}

