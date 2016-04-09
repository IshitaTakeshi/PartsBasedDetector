project(PartsBasedDetector)
# PartsBasedDetector CMakeLists.txt
cmake_minimum_required(VERSION 2.8.5)

# -----------------------------------------------
# USER DEFINED VARIABLES
# -----------------------------------------------
option(BUILD_EXECUTABLE "Build as executable to test functionality"    OFF)
option(BUILD_DOC        "Build documentation with Doxygen"             OFF)
option(WITH_OPENMP      "Build with OpenMP support for multithreading" ON)

# find cvmatio manually
include_directories(./cvmatio/include)
find_library(cvmatio_LIBRARIES cvmatio PATHS ./cvmatio/lib)

# -----------------------------------------------
# COMPILER FLAGS
# -----------------------------------------------

# include all warning types
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter -g -pedantic")

set(CMAKE_C_FLAGS
    "${CMAKE_C_FLAGS} -Wall -Wextra -Werror -Wno-unused-parameter -g -pedantic -std=c99")


# set the build type
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
        "Choose the type of build. Options are: None, Debug, Release, RelWithDebInfo, MinSizeRel"
        FORCE)
endif()

# add OpenMP support
if (WITH_OPENMP)
    find_package(OpenMP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   ${OpenMP_C_FLAGS}")
endif()

# add vectorization support
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.1")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -msse4.1")

# use highest level of optimization in Release mode
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
    set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -O3")
endif ()

# -----------------------------------------------
# DEPENDENCIES
# -----------------------------------------------
# find the dependencies
include(cmake/FindEigen.cmake)
find_package(Boost COMPONENTS system filesystem signals REQUIRED)
find_package(OpenCV 2.4.9 REQUIRED)
#find_package(Eigen REQUIRED)
#include_directories(${EIGEN_INCLUDE_DIRS})
add_definitions(-DEIGEN_USE_NEW_STDVECTOR
    -DEIGEN_YES_I_KNOW_SPARSE_MODULE_IS_NOT_STABLE_YET)

# include the dependencies
include_directories(SYSTEM  ${OPENCV_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIRS}
    )
include_directories(include)

# check if the MatlabIO dependency was successfully found
if (cvmatio_LIBRARIES)
    set(cvmatio_FOUND ON)
else()
    set(cvmatio_FOUND OFF)
endif()


# -----------------------------------------------
# SUBDIRECTORIES
# -----------------------------------------------
# add the main executable/library
add_subdirectory(src)

# add cvmatio
add_subdirectory(cvmatio)

# add documentation
if(BUILD_DOC)
    find_package(Doxygen)
    configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in ${PROJECT_SOURCE_DIR}/doc/Doxyfile @ONLY)
    add_custom_target(${PROJECT_NAME}_doc ALL ${DOXYGEN_EXECUTABLE} ${PROJECT_SOURCE_DIR}/doc/Doxyfile)
endif()


# -----------------------------------------------
# BUILD SUMARY
# -----------------------------------------------
message("")
message("---------------------------------------------")
message(${PROJECT_NAME} " Configuration Summary")
message("")
message("Build type:                    ${CMAKE_BUILD_TYPE}")
message("Build with cvmatio bindings:   ${cvmatio_FOUND}")
message("Build with threading (OpenMP): ${WITH_OPENMP}")
message("Build as executable:           ${BUILD_EXECUTABLE}")
message("Build with documentation:      ${BUILD_DOC}")
message("---------------------------------------------")
message("")
