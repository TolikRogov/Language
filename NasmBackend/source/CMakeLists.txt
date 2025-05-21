target_sources (${PROJECT_NAME}
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/NasmBackend.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Standard/Standard_reader.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Standard/Standard.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../IdNameTable/IdNameTable.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../KeyWords/KeyWords.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Tree/Tree.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Tree/Tree_dump.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Utils/Project_utils.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/../../Lexer/Lexer.cpp
)
