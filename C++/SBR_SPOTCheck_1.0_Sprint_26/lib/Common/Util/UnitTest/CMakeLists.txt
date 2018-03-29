# Unit tests
if (DEFINED UNIT_TEST)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

	add_executable(UtilUT ${TEST_SRC})

	target_link_libraries(UtilUT
	TestHelper
	Common
	Platform
	gtest
	${EXTRA_LIBS}
	)

	add_clean_target(UtilUT)

	add_test(UtilUT UtilUT "")
endif()

