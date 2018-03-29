aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(GContactlessReaderServiceUT
	${TEST_SRC}
)

target_link_libraries(GContactlessReaderServiceUT
	TestHelper
	Services
	Reports
	Menus
	SPOT
	Common
	Platform
	Version
	gtest
	${EXTRA_LIBS}
)

add_clean_target(GContactlessReaderServiceUT)

add_test(GContactlessReaderServiceUT GContactlessReaderServiceUT "")

