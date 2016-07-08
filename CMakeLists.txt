#
# This section is used to set the basic configuration for CMAKE
#
IF(WIN32)
	cmake_minimum_required(VERSION 3.2)
	set_property(GLOBAL PROPERTY USE_FOLDERS On)
ELSE()
	cmake_minimum_required(VERSION 3.2)
ENDIF()
cmake_policy(SET CMP0048 NEW)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmakemodules")

set(CMAKE_BUILD_TYPE ${TS_VS_CONFIG} CACHE STRING "The configuration build type")
if (CMAKE_CONFIGURATION_TYPES)
  set (CMAKE_CONFIGURATION_TYPES ${TS_VS_CONFIG})
  set (CMAKE_CONFIGURATION_TYPES "${CMAKE_CONFIGURATION_TYPES}" CACHE STRING
	"Reset the configurations" )
endif()

#
# Set up the project name and look for the compiler configuration
#
project(OpenVEILPythonWrapper C CXX)

#
# Configure some of the Linux SO parameters
#
if(UNIX)
    SET(CMAKE_SKIP_BUILD_RPATH FALSE)
    SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
    SET(CMAKE_INSTALL_RPATH "$ORIGIN")
    SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH FALSE)
endif(UNIX)

#
# Include the TecSec macros and variables.  Look for the OpaqueVEIL, OpenVEIL and VEILCrypto
#
include(tecsec_configure_env)
include(tecsec_ConfigureOpenVEIL)
set(SDK_FOLDER ${VEIL_ROOT_DIR}/..)
set(TS_INSTALL_PREFIX ${VEIL_ROOT_DIR})
include (tecsec_installdirs)
include(tecsec_packages)
IF(MSVC)
	# include(compiler_tecsec_msvc.noWarnings)
ENDIF(MSVC)
include(precompiled.header)
include(summary)

#
# Place the output into these folders
#
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PUBLIC_BINARY_TOP_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PUBLIC_BINARY_TOP_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PUBLIC_BINARY_TOP_DIR}/bin")

#####################################################################
## Start Options
#####################################################################

add_definitions(-DHIDE_SERVICE_LOCATOR)
set(ENABLE_SMART_CARD OFF)
set(NO_PCH OFF)

#####################################################################
## End Options
#####################################################################

#
# Look for optional packages here
#
set(Python_ADDITIONAL_VERSIONS 3.5.1 3.5 3.4)
FIND_PACKAGE(PythonInterp)
if(PYTHONINTERP_FOUND)
    message(STATUS "Found Python interpreter ${PYTHON_VERSION_STRING}")
    set(Python_ADDITIONAL_VERSIONS "")
    FIND_PACKAGE(PythonLibs)
    message(STATUS PYTHONLIBS_FOUND = ${PYTHONLIBS_FOUND})
    message(STATUS PYTHON_LIBRARIES = ${PYTHON_LIBRARIES})
    message(STATUS PYTHON_INCLUDE_PATH = ${PYTHON_INCLUDE_PATH})
    message(STATUS PYTHON_INCLUDE_DIRS = ${PYTHON_INCLUDE_DIRS})
    message(STATUS PYTHON_DEBUG_LIBRARIES = ${PYTHON_DEBUG_LIBRARIES})
    message(STATUS PYTHONLIBS_VERSION_STRING = ${PYTHONLIBS_VERSION_STRING})
    message(STATUS PYTHON_LIBRARY_DEBUG = ${PYTHON_LIBRARY_DEBUG})
    message(STATUS PYTHON_LIBRARY_RELEASE = ${PYTHON_LIBRARY_RELEASE})
	# if(PYTHON_LIBRARY_DEBUG STREQUAL PYTHON_LIBRARY_RELEASE AND TS_CONFIG STREQUAL "DEBUG")
		# add_definitions(-DBOOST_DEBUG_PYTHON)
	# endif()
endif(PYTHONINTERP_FOUND)

#
# set up the solution specific files (replaces variables and copies to the output folders)
#

#
# Configure the system using settings in the imported targets
#
ImportTarget(VEILCryptoCore)
ImportTarget(VEILCore)
ImportTarget(VEILCmsHeader)
ImportTarget(VEILFileSupport)

#
# Set up the project level include paths
#
if(PYTHONLIBS_FOUND)
  include_directories(${PYTHON_INCLUDE_PATH})
  link_directories(${PYTHON_LIBRARY_DIRS})
endif()

include_directories(ThirdParty/pybind11/include)

#
# Now include the sub-folder CMakeLists.txt files
#
add_subdirectory(src)

#
# Copy the modules from the required imported targets so that the programs in this
# project can run
#
CopyImportTargetBinaries(VEILCore ${BIN_DIR})
CopyImportTargetBinaries(VEILFileSupport ${BIN_DIR})
CopyImportTargetBinaries(VEILCmsHeader ${BIN_DIR})

#
# Set up the files that are "installed" from this CMakeLists.txt
#
list(GET Boost_LIBRARIES 1 _tmp)
get_filename_component(__boostPath ${_tmp} DIRECTORY)
get_filename_component(__boostFile ${_tmp} NAME_WE)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/Samples/tokenList.py ${CMAKE_CURRENT_SOURCE_DIR}/Samples/dumpmodule.py DESTINATION ${BIN_DIR})

# DumpAllVariables()
# get_filename_component(__pythonPath ${PYTHON_EXECUTABLE} DIRECTORY)
# install(FILES ${__pythonPath}/python${PYTHON_VERSION_MAJOR}${PYTHON_VERSION_MINOR}${CMAKE_SHARED_LIBRARY_SUFFIX} DESTINATION ${BIN_DIR})

#
# This is where CPack (if used) would be set up
#

#
# Now issue the summary configuration screen
#
summary()

#
# Set up the "uninstall" project
#
add_uninstall()

