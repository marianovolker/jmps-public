# CMakeLists.txt for Config_Dashboard library

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} FILES_SRC)

add_library(Config_Dashboard ${FILES_SRC})

add_clean_target(Config_Dashboard)

target_link_libraries(Config_Dashboard Common)
