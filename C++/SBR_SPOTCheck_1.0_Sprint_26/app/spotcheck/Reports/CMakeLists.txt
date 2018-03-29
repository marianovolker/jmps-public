# CMakeLists.txt for Reports library

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} REPORTS_SRC)
aux_source_directory(Header REPO_HEADER_SRC)
aux_source_directory(HardwareInfo HW_INFO_SRC)
aux_source_directory(TestInfo TEST_INFO_SRC)

add_library(Reports ${REPORTS_SRC} ${REPO_HEADER_SRC} ${HW_INFO_SRC} ${TEST_INFO_SRC})

add_clean_target(Reports)

# Unit tests
if (DEFINED UNIT_TEST)
	subdirs(TestInfo/UnitTest)
endif()

