# CMakeLists.txt for Services library

aux_source_directory(CardReader CARD_READER_SRC)
aux_source_directory(ContactlessReader CLESS_READER_SRC)
aux_source_directory(BarcodeScanner BARCODE_SCANNER_SRC)
aux_source_directory(Keypad KEYPAD_SRC)
aux_source_directory(Printer PRINTER_SRC)
aux_source_directory(Trind TRIND_SRC)
aux_source_directory(Display DISPLAY_SRC)
aux_source_directory(Resource RESOURCE_SRC)
aux_source_directory(SessionTimer SESSION_TIMER_SRC)

message(${RESOURCE_SRC})

add_library(Services ${CARD_READER_SRC} ${BARCODE_SCANNER_SRC} ${CLESS_READER_SRC} ${KEYPAD_SRC} ${PRINTER_SRC} ${TRIND_SRC} ${DISPLAY_SRC} ${RESOURCE_SRC} ${SESSION_TIMER_SRC})

add_clean_target(Services)

target_link_libraries(Services
	SPOT
	Common
	Config_Spotcheck
	)

# Unit tests
if (DEFINED UNIT_TEST)
	subdirs(CardReader/UnitTest)
	subdirs(ContactlessReader/UnitTest)
	subdirs(Keypad/UnitTest)
endif()
