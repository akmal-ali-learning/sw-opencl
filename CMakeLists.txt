cmake_minimum_required(VERSION 3.0.0)
project(akmal-opencl VERSION 0.1.0)

set(OPENCL_ROOT $ENV{OCL_ROOT})
include(CTest)
enable_testing()

file(GLOB CPP_SOURCES 
    "src/*.cpp" "src/*.h"
    "src/*/*.cpp" "src/*/*.h"
)


add_executable(${PROJECT_NAME} ${CPP_SOURCES})
# OCL_ROOT
target_include_directories(${PROJECT_NAME} PUBLIC ${OPENCL_ROOT}/include)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/src )
target_link_libraries(${PROJECT_NAME} ${OPENCL_ROOT}/lib/x86_64/opencl.lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)