
# Set platform specific source
if ("${TARGET_PLATFORM}" MATCHES "windows")
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Windows MAIN_SRC)
elseif ("${TARGET_PLATFORM}" MATCHES "linux")
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/Linux MAIN_SRC)
elseif ("${TARGET_PLATFORM}" MATCHES "spotm3")
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/SpotM3 MAIN_SRC)
elseif ("${TARGET_PLATFORM}" MATCHES "spotm5")
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/SpotM5 MAIN_SRC)
elseif ("${TARGET_PLATFORM}" MATCHES "vgd")
    aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR}/SpotM5 MAIN_SRC)
endif()

set(MAIN_SRC ${MAIN_SRC}
	${CMAKE_CURRENT_SOURCE_DIR}/SPOTDashboard.cpp
	PARENT_SCOPE
)

