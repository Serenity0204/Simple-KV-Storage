cmake_minimum_required(VERSION 3.14) # Because we are using FetchContent which is available in 3.14+
project(SimpleKVStorage)

# Adding interface library (header-only)
add_library(SimpleKVStorage INTERFACE)

# Specify include directories
target_include_directories(SimpleKVStorage INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>  # <prefix>/include
)

# Install library target
install(TARGETS SimpleKVStorage
    EXPORT SimpleKVStorageTargets
    INCLUDES DESTINATION include
)

# This makes project importable from the install directory
# Put config file into the CMake user package registry
install(EXPORT SimpleKVStorageTargets
    FILE SimpleKVStorageTargets.cmake
    NAMESPACE SimpleKVStorage::
    DESTINATION lib/cmake/SimpleKVStorage
)

# This makes project importable from the build directory
export(TARGETS SimpleKVStorage
    NAMESPACE SimpleKVStorage::
    FILE ${CMAKE_CURRENT_BINARY_DIR}/SimpleKVStorageTargets.cmake
)
