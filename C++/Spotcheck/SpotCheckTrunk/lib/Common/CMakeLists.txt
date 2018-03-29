# CMakeLists.txt for Common

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} COMMON_SRC)
aux_source_directory(ConfigFile CONFIG_FILE_SRC)
aux_source_directory(Logger LOGGER_SRC)
aux_source_directory(Util UTIL_SRC)
aux_source_directory(Xml XML_SRC)

add_library(Common
	${COMMON_SRC}
	${CONFIG_FILE_SRC}
	${LOGGER_SRC}
	${UTIL_SRC}
	${XML_SRC}
)

add_clean_target(Common)

# Unit tests
if (DEFINED UNIT_TEST)
	subdirs(
	ConfigFile/UnitTest
	Util/UnitTest
	Xml/UnitTest
	)
else(DEFINED UNIT_TEST)
	message("Skipping unit tests in ./app/spotcheck/common")
endif(DEFINED UNIT_TEST)

