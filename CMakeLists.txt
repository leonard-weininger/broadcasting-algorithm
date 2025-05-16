cmake_minimum_required(VERSION 3.16)
project(BroadcastingProject LANGUAGES CXX)

# Build type warning
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type specified. Use -DCMAKE_BUILD_TYPE=Release or Debug.")
endif()

# C++17 standard setup
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set output directory for all configurations
if(CMAKE_CONFIGURATION_TYPES)
  foreach(OUTPUTCONFIG IN LISTS CMAKE_CONFIGURATION_TYPES)
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG_UPPER)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG_UPPER} ${CMAKE_BINARY_DIR})
  endforeach()
else()
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

# Debug flags
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  message(STATUS "Enabling debug flags")
  add_compile_options(-Wall -Wextra -Wshadow -g)

  if (CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(STATUS "Enabling sanitizers for GCC/Clang")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
  else()
    message(WARNING "Sanitizers not supported by ${CMAKE_CXX_COMPILER_ID}")
  endif()
endif()

# Gather all source files (including subdirs)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Create the core library
add_library(broadcast-core ${SOURCES})
target_include_directories(broadcast-core PUBLIC
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Automatically create executables for all .cpp files in the apps folder
file(GLOB APP_SOURCES "apps/*.cpp")

foreach(APP_SRC ${APP_SOURCES})
  # Extract the name of the executable (using the base filename without extension)
  get_filename_component(APP_NAME ${APP_SRC} NAME_WE)

  # Create executable for each source file
  add_executable(${APP_NAME} ${APP_SRC})

  # Link the core library to each executable
  target_link_libraries(${APP_NAME} PRIVATE broadcast-core)
endforeach()

# Optional tests
option(ENABLE_TESTS "Build test targets" OFF)

if(ENABLE_TESTS)
  include(CTest)

  include(FetchContent)
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
  )
  FetchContent_MakeAvailable(Catch2)

  file(GLOB TEST_SOURCES tests/*.cpp)

  foreach(TEST_SRC ${TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_SRC})
    target_link_libraries(${TEST_NAME} PRIVATE broadcast-core Catch2::Catch2WithMain)
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
  endforeach()
endif()
