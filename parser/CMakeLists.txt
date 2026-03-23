
file(GLOB PARSER_SRC "*.cpp")
file(GLOB PARSER_H "*.h")

add_library (parser STATIC ${PARSER_SRC} ${PARSER_H})

include_directories(${ANTLR_INCLUDE_DIR})

target_link_libraries(parser ${ANTLR_LIBRARY} tpoly)
