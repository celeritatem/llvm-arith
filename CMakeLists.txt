cmake_minimum_required (VERSION 3.20.0)
project (llvmarith)

# find and load llvm package
find_package(LLVM REQUIRED CONFIG)
message("LLVM Version: ${LLVM_PACKAGE_VERSION}, build type ${LLVM_BUILD_TYPE}, dir ${LLVM_DIR}")

# add LLVM cmake modules to the search path
list(APPEND CMAKE_MODULE_PATH ${LLVM_DIR})

# add LLVM def/ns and include path
separate_arguments(LLVM_DEFINITIONS_LIST NATIVE_COMMAND ${LLVM_DEFINITIONS})
add_definitions(${LLVM_DEFINITIONS_LIST})
include_directories(SYSTEM ${LLVM_INCLUDE_DIRS})
# map LLVM components to library names
llvm_map_components_to_libnames(llvm_libs Core)

# set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# add project
include_directories(include/)
include_directories(src/)
add_subdirectory(src)
