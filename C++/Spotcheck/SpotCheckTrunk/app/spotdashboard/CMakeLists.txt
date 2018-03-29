
# Main chooses the correct platform and sets MAIN_SRC.
add_subdirectory(Main)

set(WXWIDGETS_LIBRARIES) 
#set a flag to know if Gui has to be included
set(GUI_LIBRARY) 

#Check for now if the target is linux, this will be changed to also configure for windows wxWidgets libraries.
if ( "${TARGET_PLATFORM}" MATCHES "linux" OR "${TARGET_PLATFORM}" MATCHES "windows" )
	subdirs( Version Messaging Services Config Gui)

	SET(wxWidgets_USE_LIBS)
	FIND_PACKAGE(wxWidgets)

	#The following settings are for Linux platform.
	if ( "${TARGET_PLATFORM}" MATCHES "linux" )
		IF(wxWidgets_FOUND)
			INCLUDE("${wxWidgets_USE_FILE}")

			MESSAGE("wxWidgets use inc wxWidgets_USE_FILE = ${wxWidgets_USE_FILE}")
			MESSAGE("wxWidgets use inc wxWidgets_CPP_FLAGS = ${wxWidgets_CPP_FLAGS}")
			MESSAGE("wxWidgets use inc wxWidgets_CXX_FLAGS = ${wxWidgets_CXX_FLAGS}")
			MESSAGE("wxWidgets use inc wxWidgets_INCLUDE_DIRS = ${wxWidgets_INCLUDE_DIRS}")
			MESSAGE("wxWidgets use inc wxWidgets_LIBRARY_DIRS = ${wxWidgets_LIBRARY_DIRS}")
			MESSAGE("CMAKE_CPP_FLAGS = ${CMAKE_CPP_FLAGS}")
			MESSAGE("CMAKE_CXX_FLAGS = ${CMAKE_CXX_FLAGS}")
			MESSAGE("CMAKE_EXTRA_INCS = ${CMAKE_EXTRA_INCS}")
			MESSAGE("CMAKE_EXTRA_LIBS = ${CMAKE_EXTRA_LIBS}")

			#ADD_EXECUTABLE(spotdashboard_linux WIN32 HelloWorld.cpp)
			# and for each of your dependant executable/library targets:
			#TARGET_LINK_LIBRARIES(spotdashboard  ${wxWidgets_LIBRARIES})

			set(WXWIDGETS_LIBRARIES ${wxWidgets_LIBRARIES} ) 
			MESSAGE("wxWidgets use lib wxWidgets_LIBRARIES= ${wxWidgets_LIBRARIES}")
			MESSAGE("wxWidgets found. ${WXWIDGETS_LIBRARIES}")
		ELSE(wxWidgets_FOUND)
			# For convenience. When we cannot continue, inform the user
			MESSAGE(FATAL_ERROR "wxWidgets not found. ABORTED COMPILATION!")
		ENDIF(wxWidgets_FOUND)

	#The following settings are for Windows platform
	else()
		include_directories("/usr/local/wxpack-w32/include")
		set(WXWIDGETS_LIBRARIES "-L/usr/local/wxpack-w32/lib/gcc_dll -lwxbase28 -lwxmsw28_core -lwxbase28_xml")
		#SET(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} ???")
	endif()	

	# Build spotdashboard executable for linux or windows with wxWidgets
	include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}
		Main
		Gui
	)

	add_executable(spotdashboard
		
		${MAIN_SRC}
	)

	#set library Gui only for Linux or Windows
	set(GUI_LIBRARY Gui)

# M3 and M5 embedded application
else()

	subdirs( Version Messaging Services Config)

	# Build spotdashboard executable
	include_directories(
		${CMAKE_CURRENT_SOURCE_DIR}
		Main
	)


	add_executable(spotdashboard
		 ${MAIN_SRC}
	)


endif()


if("${CMAKE_TOOLCHAIN_FILE}" MATCHES "uclinux")
	set(CMAKE_EXE_LINKER_FLAGS "-Wl,-elf2flt='-s 32768 -z' -Wl,-move-rodata -lc -m5307 -static -lpthread")
endif()

target_link_libraries(spotdashboard 
	SPOT
	Common
	Platform
	Messaging
	Version_Dashboard	
	Services_Dashboard
	Config_Dashboard
	${GUI_LIBRARY}
	${EXTRA_LIBS}
	${WXWIDGETS_LIBRARIES}
)

install( TARGETS spotdashboard RUNTIME DESTINATION opt/spotdashboard/bin )


# Unit tests
if (DEFINED UNIT_TEST)
	subdirs(
	Config/UnitTest
	)
else(DEFINED UNIT_TEST)
	message("Skipping unit tests in ./app/spotdashboard/")
endif(DEFINED UNIT_TEST)


