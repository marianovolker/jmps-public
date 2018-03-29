
# Set platform specific source
if (WIN32)
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Windows MAIN_SRC)
else()
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Linux MAIN_SRC)
endif()

set(MAIN_SRC ${MAIN_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/SPOTCheck.cpp
	PARENT_SCOPE
)
if (DEFINED UNIT_TEST)
#        target_link_libraries(Menus Reports Services)
        subdirs(UnitTest)


endif()

