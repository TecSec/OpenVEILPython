INCLUDE_DIRECTORIES(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/${TS_VS_CONFIGURATION}
    ${PUBLIC_SOURCE_TOP_DIR}/include
    )

set(Source
	OpenVEILWrapper.cpp
	ConnectorWrappers.cpp
	TokenWrapper.cpp
	SessionWrapper.cpp
	FavoriteWrapper.cpp
)

set(CMAKE_DEBUG_POSTFIX "")

add_library(OpenVEIL SHARED
	${PUBLIC_SOURCE_TOP_DIR}/include/OpenVEILWrapper.h
	${Source}
)

target_include_directories(OpenVEIL INTERFACE $<BUILD_INTERFACE:${PUBLIC_SOURCE_TOP_DIR}/include> $<INSTALL_INTERFACE:${TS_MODULE}/include>)
add_definitions("-DOPENVEILWRAPPER_EXPORTS")
TARGET_LINK_LIBRARIES(OpenVEIL ${Boost_LIBRARY_DIRS}/libboost_python3-vc140-mt-1_59.lib OpenVEILCore FileVEILSupport CmsHeader ${PYTHON_LIBRARIES})
set_target_properties(OpenVEIL PROPERTIES SUFFIX ".pyd")
# add_precompiled_header(OVCore stdafx.h FORCEINCLUDE SOURCE_CXX stdafx.cpp)


install(TARGETS OpenVEIL ARCHIVE DESTINATION ${LIB_DIR} LIBRARY DESTINATION ${SHLIB_DIR} RUNTIME DESTINATION ${BIN_DIR} )

