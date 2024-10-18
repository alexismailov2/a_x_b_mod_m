cmake_minimum_required(VERSION 3.28)
project(oismailov_math_test LANGUAGES C)

add_executable(${PROJECT_NAME}
        main.c)

target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/../include)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        oismailov_math)

install(TARGETS oismailov_math_test EXPORT ${DMAS_PROJECT}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
