# CMakeLists.txt for Services_Dashboard library

aux_source_directory(Common COMMON_SRC)
aux_source_directory(MainUnit MAIN_SRC)
aux_source_directory(SecurityModule SECURITY_SRC)
aux_source_directory(CardReader CARD_READER_SRC)
aux_source_directory(ContactlessReader CLESS_READER_SRC)
aux_source_directory(Keypad KEYPAD_SRC)
aux_source_directory(Printer PRINTER_SRC)

add_library(Services_Dashboard  ${COMMON_SRC} ${MAIN_SRC} ${SECURITY_SRC} ${CARD_READER_SRC} ${CLESS_READER_SRC} ${KEYPAD_SRC} ${PRINTER_SRC})

add_clean_target(Services_Dashboard)

target_link_libraries(Services_Dashboard SPOT Common)
