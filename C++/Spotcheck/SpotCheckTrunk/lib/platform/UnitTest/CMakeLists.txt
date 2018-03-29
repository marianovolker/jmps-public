aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} TEST_SRC)

add_executable(PlatformUT
	${TEST_SRC}
)

if(WIN32)
	set(EXTRA_LIBS ws2_32)
endif()

target_link_libraries(PlatformUT
	Platform
	gtest
	${EXTRA_LIBS}
)

# Copy test files to binary directory for running the test
file(COPY . DESTINATION .
	FILES_MATCHING PATTERN *.xml
	PATTERN .svn EXCLUDE
)

add_test(PlatformUT PlatformUT "")
