find_package(BISON)
find_package(FLEX)

BISON_TARGET(MyParser iloc.y ${CMAKE_CURRENT_BINARY_DIR}/parser.cc)
FLEX_TARGET(MyScanner iloc.l ${CMAKE_CURRENT_BINARY_DIR}/scanner.cc)
ADD_FLEX_BISON_DEPENDENCY(MyScanner MyParser)


include_directories(${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
add_executable(iloc
    Main.cpp
    Operation.cpp
    Machine.cpp
    Program.cpp   
    ${BISON_MyParser_OUTPUTS}
    ${FLEX_MyScanner_OUTPUTS}
)
set_property(TARGET iloc PROPERTY CXX_STANDARD 11)
set_property(TARGET iloc PROPERTY CXX_STANDARD_REQUIRED ON)
