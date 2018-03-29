# CMakeLists.txt for Gui

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} GUI_SRC)

add_library(Gui ${GUI_SRC})

add_clean_target(Gui)

MESSAGE("GUI_SRC =. ${GUI_SRC}")
