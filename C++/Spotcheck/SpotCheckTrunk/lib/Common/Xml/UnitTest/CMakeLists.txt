# Unit tests
if (DEFINED UNIT_TEST)
	aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

	add_executable(XmlUT ${TEST_SRC})

	target_link_libraries(XmlUT
	TestHelper
	Common
	Platform
	pugixml
	gtest
	${EXTRA_LIBS}
	)

	add_clean_target(XmlUT)

	add_test(XmlUT XmlUT "")
endif()

