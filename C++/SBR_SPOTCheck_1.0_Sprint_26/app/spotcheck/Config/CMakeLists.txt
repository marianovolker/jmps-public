# CMakeLists.txt for Config_Spotcheck library

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} FILES_SRC)

add_library(Config_Spotcheck ${FILES_SRC})

add_clean_target(Config_Spotcheck)

target_link_libraries(Config_Spotcheck Common)
