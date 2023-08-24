cmake_minimum_required(VERSION 3.14...3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
set(CMAKE_FIND_LIBRARY_SUFFIXES .a .lib)

project(NnmcppStandalone LANGUAGES CXX)

# --- Import tools ----

include(../cmake/tools.cmake)

# ---- Dependencies ----

include(../cmake/CPM.cmake)

CPMAddPackage(
  GITHUB_REPOSITORY jarro2783/cxxopts
  VERSION 3.0.0
  OPTIONS "CXXOPTS_BUILD_EXAMPLES NO" "CXXOPTS_BUILD_TESTS NO" "CXXOPTS_ENABLE_INSTALL YES"
)

CPMAddPackage(NAME Nnmcpp SOURCE_DIR ${CMAKE_CURRENT_LIST_DIR}/..)

# ---- Create standalone executable ----

file(GLOB headers CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/**.cpp)
file(GLOB sources CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/source/**.hpp)
add_executable(${PROJECT_NAME} ${headers} ${sources})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/source)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_STANDARD 20 OUTPUT_NAME "Nnmcpp")

target_link_libraries(${PROJECT_NAME} Nnmcpp::Nnmcpp cxxopts)
