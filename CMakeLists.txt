cmake_minimum_required(VERSION 3.1)
project(snowkolov)

include(CheckCXXCompilerFlag)

function(enable_cxx_compiler_flag_if_supported flag)
    string(FIND "${CMAKE_CXX_FLAGS}" "${flag}" flag_already_set)
    if(flag_already_set EQUAL -1)
        check_cxx_compiler_flag("${flag}" flag_supported)
        if(flag_supported)
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${flag}" PARENT_SCOPE)
        endif()
        unset(flag_supported CACHE)
    endif()
endfunction()



if (WIN32)
    # Vérifie si le compilateur est MSVC
    if (MSVC)
        # Activer le support OpenMP pour MSVC
        enable_cxx_compiler_flag_if_supported("/openmp")
        add_compile_options(/O2)
    endif()
endif()

if(UNIX AND NOT APPLE)
    # for Linux, BSD, Solaris, Minix
    enable_cxx_compiler_flag_if_supported("-Wall")
    enable_cxx_compiler_flag_if_supported("-Wextra")
    enable_cxx_compiler_flag_if_supported("-pedantic")
    enable_cxx_compiler_flag_if_supported("-std=c++17")
    enable_cxx_compiler_flag_if_supported("-O3")
    enable_cxx_compiler_flag_if_supported("-fopenmp")
endif()

file(GLOB SOURCES *.hpp *.cpp src/*.hpp src/*.cpp)

add_subdirectory(lib/glm)
include_directories(lib/glm)

add_executable(${PROJECT_NAME} ${SOURCES})