# CMakeLists.txt for Messaging 

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MESSAGING_SRC)

add_library(Messaging ${MESSAGING_SRC})

add_clean_target(Messaging)

