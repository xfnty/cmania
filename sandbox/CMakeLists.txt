cmake_minimum_required(VERSION 3.5)
include("${CMAKE_SOURCE_DIR}/CMakeHelpers.cmake")

list_sources(SANDBOX_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src")

add_subdirectory("thirdparty/raylib")

add_executable("sandbox" ${SANDBOX_SOURCES})
message(STATUS "Linking sandbox to \"${PROJECT_LIBRARY_NAME}\"")

target_include_directories("sandbox" PRIVATE "thirdparty/raylib/src")
target_link_libraries("sandbox" PRIVATE "raylib" "${PROJECT_LIBRARY_NAME}")

add_compile_options("-W")

set_target_properties(
    "sandbox"
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin-sandbox"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin-sandbox"
)
