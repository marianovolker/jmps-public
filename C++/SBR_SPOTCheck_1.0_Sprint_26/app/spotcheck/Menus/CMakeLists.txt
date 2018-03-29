# CMakeLists.txt for Menus 

aux_source_directory(${CMAKE_CURRENT_SOURCE_DIR} MENUS_SRC)
aux_source_directory(BNA BNAMENUS_SRC)
aux_source_directory(CardReader CARDREADER_SRC)
aux_source_directory(ContactlessReader CONTACTLESSREADER_SRC)
aux_source_directory(BarcodeScanner BARCODESCANNER_SRC)
aux_source_directory(Display DISPLAY_SRC)
aux_source_directory(Keypad KEYPAD_SRC)
aux_source_directory(MainMenus MAINMENUS_SRC)
aux_source_directory(Printer PRINTER_SRC)
aux_source_directory(Templates TEMPLATES_SRC)
aux_source_directory(TestSegments TESTSEGMENTS_SRC)

add_library(Menus
            ${MENUS_SRC}
            ${BNAMENUS_SRC}
            ${CARDREADER_SRC}
            ${CONTACTLESSREADER_SRC}
            ${BARCODESCANNER_SRC}
            ${DISPLAY_SRC}
            ${KEYPAD_SRC}
            ${MAINMENUS_SRC}
            ${PRINTER_SRC}
            ${TEMPLATES_SRC}
            ${TESTSEGMENTS_SRC}
)

add_clean_target(Menus)

# Unit tests
if (DEFINED UNIT_TEST)
	target_link_libraries(Menus Reports Services)
	subdirs(Keypad/UnitTest)
	subdirs(CardReader/UnitTest)
	
	
endif()

