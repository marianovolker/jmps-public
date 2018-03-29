# CMakeLists.txt for TestHelper

if (DEFINED UNIT_TEST) 
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_HELPER_SRC)
	add_library( TestHelper ${TEST_HELPER_SRC} )
	add_clean_target(TestHelper)
endif () 
