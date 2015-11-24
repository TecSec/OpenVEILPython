INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/${TS_VS_CONFIGURATION}
    ${PUBLIC_SOURCE_TOP_DIR}/include
    )

set(Source
	OpenVEILWrapper.cpp
)

# Set a global cache variable for this project GUID
# The TestAppNet csproj needs this GUID to properly reference this project
set_property(GLOBAL PROPERTY OpenVEILWrapper_GUID "A7FFD62E-A2B1-4AC5-A7E2-E0A9E48E6F0A") 
get_property(projectGUID GLOBAL PROPERTY OpenVEILWrapper_GUID)
MESSAGE( STATUS "Setting project GUID to: ${projectGUID}")
set(OpenVEILWrapper_GUID_CMAKE "${projectGUID}" CACHE INTERNAL "Project GUID")

add_library(OpenVEILWrapper SHARED
	${PUBLIC_SOURCE_TOP_DIR}/include/OpenVEILWrapper.h
	${Source}
)

target_include_directories(OpenVEILWrapper INTERFACE $<BUILD_INTERFACE:${PUBLIC_SOURCE_TOP_DIR}/include> $<INSTALL_INTERFACE:${TS_MODULE}/include>)
add_definitions("-DOPENVEILWRAPPER_EXPORTS")
TARGET_LINK_LIBRARIES(OpenVEILWrapper ${Boost_LIBRARY_DIRS}/libboost_python3-vc140-mt-1_59.lib OpenVEILCore ${PYTHON_LIBRARIES})
set_target_properties(OpenVEILWrapper PROPERTIES SUFFIX ".pyd")
# add_precompiled_header(OVCore stdafx.h FORCEINCLUDE SOURCE_CXX stdafx.cpp)


install(TARGETS OpenVEILWrapper ARCHIVE DESTINATION ${LIB_DIR} LIBRARY DESTINATION ${SHLIB_DIR} RUNTIME DESTINATION ${BIN_DIR} )

