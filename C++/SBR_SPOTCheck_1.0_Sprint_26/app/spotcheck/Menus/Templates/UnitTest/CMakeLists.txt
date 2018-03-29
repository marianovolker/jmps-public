# Unit tests
if (DEFINED UNIT_TEST)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

	add_executable(ConfigFileUT ${TEST_SRC})

	target_link_libraries(ConfigFileUT
	TestHelper
	Common
	Platform
	pugixml
	gtest
	${EXTRA_LIBS}
	)

	add_clean_target(ConfigFileUT)

	# Copy test files to binary directory for running the test
	file(COPY TestFiles DESTINATION .)

	add_test(ConfigFileUT ConfigFileUT "")
endif(DEFINED UNIT_TEST)

