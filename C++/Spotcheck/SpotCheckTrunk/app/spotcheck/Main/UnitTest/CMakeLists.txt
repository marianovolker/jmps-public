aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(GSelfDeleteUT
	${TEST_SRC}
)

target_link_libraries(GSelfDeleteUT
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

add_clean_target(GSelfDeleteUT)

file(COPY TestFiles DESTINATION .)

add_test(GSelfDeleteUT GSelfDeleteUT "")

