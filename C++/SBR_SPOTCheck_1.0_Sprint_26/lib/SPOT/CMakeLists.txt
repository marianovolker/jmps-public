# CMakeLists.txt for SPOT interface

aux_source_directory(SPOTProtocolInterface SPOT_PROTOCOL_INTERFACE_SRC)
aux_source_directory(SystemInformation SYSTEM_INFORMATION_SRC)
aux_source_directory(Utils SPOT_UTILS_SRC)

add_library(SPOT
	${SPOT_PROTOCOL_INTERFACE_SRC}
	${SYSTEM_INFORMATION_SRC}
	${SPOT_UTILS_SRC}
)

add_clean_target(SPOT)

# Unit tests
if (DEFINED UNIT_TEST)
	subdirs(
		SystemInformation/UnitTest
		Utils/UnitTest
		)
	
endif()

