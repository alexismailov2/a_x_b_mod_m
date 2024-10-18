cmake_minimum_required(VERSION 3.28)
project(oismailov_math LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)

add_library(${PROJECT_NAME}
        SHARED
        src/GCN_Test.cu)

set_target_properties(${PROJECT_NAME} PROPERTIES CUDA_SEPARABLE_COMPILATION ON)

message(STATUS "Boost_INCLUDE_DIRECTORIES=${Boost_INCLUDE_DIRS}")
target_include_directories(${PROJECT_NAME}
        PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/include
        ${CMAKE_CURRENT_LIST_DIR}/CGBN/include
        ${CMAKE_CURRENT_LIST_DIR}/CGBN/samples
        ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
        PRIVATE
        gmp)

include(GNUInstallDirs)

install(TARGETS oismailov_math EXPORT ${DMAS_PROJECT}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}
        RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR})
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/include DESTINATION ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_INCLUDEDIR})

add_subdirectory(test)
