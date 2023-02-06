cmake_minimum_required(VERSION 3.15)
project(simExtGeom)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_MACOSX_RPATH 1)

find_package(Boost REQUIRED)

if(NOT COPPELIASIM_INCLUDE_DIR)
    if(DEFINED ENV{COPPELIASIM_ROOT_DIR})
        set(COPPELIASIM_INCLUDE_DIR $ENV{COPPELIASIM_ROOT_DIR}/programming/include)
    else()
        set(COPPELIASIM_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../include)
    endif()
endif()
list(APPEND CMAKE_MODULE_PATH ${COPPELIASIM_INCLUDE_DIR}/cmake)
find_package(CoppeliaSim 4.5.0.0 REQUIRED)

coppeliasim_add_plugin(
    simExtGeom
    LEGACY
    SOURCES
    simExtGeom.cpp
    ../coppeliaGeometricRoutines/geom.cpp
    ../coppeliaGeometricRoutines/calcUtils.cpp
    ../coppeliaGeometricRoutines/obbNode.cpp
    ../coppeliaGeometricRoutines/obbStruct.cpp
    ../coppeliaGeometricRoutines/ocNode.cpp
    ../coppeliaGeometricRoutines/ocStruct.cpp
    ../coppeliaGeometricRoutines/pcNode.cpp
    ../coppeliaGeometricRoutines/pcStruct.cpp
    ../coppeliaGeometricRoutines/kdNode.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simMath/MyMath.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simMath/3Vector.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simMath/4Vector.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simMath/7Vector.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simMath/3X3Matrix.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simMath/4X4Matrix.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simMath/MMatrix.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simLib/scriptFunctionData.cpp
    ${COPPELIASIM_INCLUDE_DIR}/simLib/scriptFunctionDataItem.cpp
)
target_compile_definitions(simExtGeom PRIVATE SIM_MATH_DOUBLE)
target_include_directories(simExtGeom PRIVATE ../coppeliaGeometricRoutines)
target_include_directories(simExtGeom PRIVATE ${COPPELIASIM_INCLUDE_DIR}/simMath)
coppeliasim_add_helpfile(simGeom.htm)
coppeliasim_add_helpfile(simGeom.json SUBDIR index)
