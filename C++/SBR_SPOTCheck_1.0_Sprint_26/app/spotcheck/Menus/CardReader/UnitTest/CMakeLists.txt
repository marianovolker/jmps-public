aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(GCardReaderManagerUT
	${TEST_SRC}
)

target_link_libraries(GCardReaderManagerUT
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

add_clean_target(GCardReaderManagerUT)

add_test(GCardReaderManagerUT GCardReaderManagerUT "")

