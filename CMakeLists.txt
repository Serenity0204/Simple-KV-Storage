cmake_minimum_required(VERSION 3.14) # Because we are using FetchContent which is available in 3.14+
project(SimpleKVStorage)

# Adding interface library (header-only)
add_library(SimpleKVStorage INTERFACE)

# Specify include directories
target_include_directories(SimpleKVStorage INTERFACE 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/includes>
    $<INSTALL_INTERFACE:include>  # <prefix>/include
)

include(GNUInstallDirs)
if(MSVC)
    add_compile_options(/W4)
    add_link_options(/W4)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_BINARY_DIR}/bin)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})

if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
endif()

## Change ON <-> OFF for testing
option(BUILD_TESTS "Build tests" OFF)

if(BUILD_TESTS)
    include(FetchContent)
    FetchContent_Declare(
      googletest
      URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
    )

    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()

    set(INCLUDES_FOLDER includes)
    FILE(GLOB_RECURSE SOURCE_FILES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "${INCLUDES_FOLDER}/*.cpp")


    ADD_EXECUTABLE(test_fileIO
        _tests/_test_files/test_fileIO.cpp
        ${SOURCE_FILES}
    )

    ADD_EXECUTABLE(test_serializer
        _tests/_test_files/test_serializer.cpp
        ${SOURCE_FILES}
    )

    ADD_EXECUTABLE(test_bplustree
        _tests/_test_files/test_bplustree.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(test_map
        _tests/_test_files/test_map.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(test_simpleKV
        _tests/_test_files/test_simpleKV.cpp
        ${SOURCE_FILES}
    )
    ADD_EXECUTABLE(main
        main.cpp
        ${SOURCE_FILES}
    )
    
    TARGET_LINK_LIBRARIES(test_fileIO gtest)
    TARGET_LINK_LIBRARIES(test_serializer gtest)
    TARGET_LINK_LIBRARIES(test_bplustree gtest)
    TARGET_LINK_LIBRARIES(test_map gtest)
    TARGET_LINK_LIBRARIES(test_simpleKV gtest)
    TARGET_LINK_LIBRARIES(main gtest)
endif()

set(CMAKE_CXX_STANDARD 14)

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
