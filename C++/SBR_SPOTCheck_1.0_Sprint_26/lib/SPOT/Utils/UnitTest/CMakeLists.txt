aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(LicFileUtilsUT
	${TEST_SRC}
)

target_link_libraries(LicFileUtilsUT
	TestHelper
	SPOT
	Common
	Platform
	gtest
	${EXTRA_LIBS}
)


add_clean_target(LicFileUtilsUT)

# Copy test files to binary directory for running the test
file(COPY TestFiles DESTINATION .)

add_test(LicFileUtilsUT LicFileUtilsUT "")

