aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(SystemInformationUT
	${TEST_SRC}
)

target_link_libraries(SystemInformationUT
	TestHelper
	SPOT
	Common
	Platform
	gtest
	${EXTRA_LIBS}
)


add_clean_target(SystemInformationUT)

add_test(SystemInformationUT SystemInformationUT "")

