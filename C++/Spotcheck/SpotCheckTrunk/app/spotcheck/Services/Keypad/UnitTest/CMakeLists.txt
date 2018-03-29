aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(GKeypadServiceUT
	${TEST_SRC}
)

target_link_libraries(GKeypadServiceUT
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

add_clean_target(GKeypadServiceUT)

add_test(GKeypadServiceUT GKeypadServiceUT "")

