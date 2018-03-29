# Unit tests
if (DEFINED UNIT_TEST)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)
	
	add_executable(GConfigUT ${TEST_SRC})
	
	target_link_libraries(	GConfigUT
							TestHelper
							Config_Dashboard
							Common
							Platform
							SPOT
							gtest
							${EXTRA_LIBS}
						 )
	
	add_clean_target(GConfigUT)
	
	# Copy test files to binary directory for running the test
	file(COPY ini DESTINATION .)
	
	add_test(GConfigUT GConfigUT "")
endif()
