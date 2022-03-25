cmake_minimum_required(VERSION 3.0.0)
project(akmal-opencl VERSION 0.1.0)
set(CMAKE_CXX_STANDARD 17)


################################
# GTest
################################
ADD_SUBDIRECTORY(ext/googletest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

file(GLOB CPP_SOURCES 
    "src/framework/*.cpp" "src/framework/*.h"
    "src/framework/*/*.cpp" "src/framework/*/*.h"
)

## OPENCL
if( DEFINED ENV{OCL_ROOT} )
### OCL_ROOT
set(OPENCL_ROOT $ENV{OCL_ROOT})
set(OPENCL_SUPPORT 1)
file(GLOB OPEN_CL_SOURCES 
    "src/opencl/*.cpp" "src/opencl/*.h"
    "src/opencl/*/*.cpp" "src/opencl/*/*.h"
)
list(APPEND CPP_SOURCES ${OPEN_CL_SOURCES})
else()
set(OPENCL_SUPPORT 0)
endif()

add_executable(${PROJECT_NAME} ${CPP_SOURCES})

target_link_libraries(${PROJECT_NAME} gtest_main)


## OPENCL
if( ${OPENCL_SUPPORT} )
### OCL_ROOT
set(OPENCL_ROOT $ENV{OCL_ROOT})
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCL_ROOT}/include)
target_link_libraries(${PROJECT_NAME} ${OPENCL_ROOT}/lib/x86_64/opencl.lib)
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
