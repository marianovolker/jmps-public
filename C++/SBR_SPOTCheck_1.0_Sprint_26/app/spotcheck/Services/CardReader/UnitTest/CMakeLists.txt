aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(GCardReaderServiceUT
	${TEST_SRC}
)

target_link_libraries(GCardReaderServiceUT
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

add_clean_target(GCardReaderServiceUT)

add_test(GCardReaderServiceUT GCardReaderServiceUT "")

