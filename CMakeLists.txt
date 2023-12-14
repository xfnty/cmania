cmake_minimum_required(VERSION 3.5)
include("${CMAKE_SOURCE_DIR}/CMakeHelpers.cmake")


# ===== Build options ===== #
set(PROJECT_BUILD_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")
option(BUILD_SHARED_LIBS "Build shared libs" OFF)
option(BUILD_TESTS "Build tests" ON)


# ===== Project ===== #
set(CMAKE_CXX_STANDARD 17)
project("cmania" LANGUAGES C CXX)


# ===== Third Party libraries ===== #
include_thirdparty("klib" "src" "klib")


# ===== Source files ===== #
set(SOURCE_MAIN "src/main.c")

list_sources(SOURCES "src")
list(FILTER SOURCES EXCLUDE REGEX ${SOURCE_MAIN})


# ===== Include directories ===== #
list(APPEND INCLUDES "src")
include_directories(${INCLUDES})


# ===== Link libraries ===== #
list(APPEND LINK_LIBRARIES "m")
link_libraries(${LINK_LIBRARIES})


# ===== Targets ===== #
if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
    add_compile_options("-Wall" "-pg")
    add_link_options("-pg")
endif()

set(PROJECT_LIBRARY_NAME "${PROJECT_NAME}-shared")
if (SOURCES)
    add_library("${PROJECT_LIBRARY_NAME}" STATIC ${SOURCES})
else()
    add_library("${PROJECT_LIBRARY_NAME}" INTERFACE)
endif()

add_executable(${PROJECT_NAME} ${SOURCE_MAIN})
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_LIBRARY_NAME})
set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
    OUTPUT_NAME "${PROJECT_NAME}"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG "${PROJECT_BUILD_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY_RELEASE "${PROJECT_BUILD_DIRECTORY}"
)


# ===== Tests ===== #
if (BUILD_TESTS)
    enable_testing()
    add_subdirectory("tests")
endif()


# ===== Sandbox ===== #
add_subdirectory("sandbox")
