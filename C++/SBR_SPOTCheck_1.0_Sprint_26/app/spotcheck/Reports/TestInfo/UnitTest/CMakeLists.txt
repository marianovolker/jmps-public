aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(GTestInfoReportUT
	${TEST_SRC}
)

target_link_libraries(GTestInfoReportUT
	TestHelper
	Menus
	Reports
	Services
	Menus
	SPOT
	Common
	Platform
	Version
	gtest
	${EXTRA_LIBS}
)

add_clean_target(GTestInfoReport)

add_test(GTestInfoReportUT GTestInfoReportUT "")

