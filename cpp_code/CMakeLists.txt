
#生成搜索覆盖的EXE

#添加定义
#add_definitions(-D__EXPORT_LIB)

cmake_minimum_required(VERSION 2.9)

project(MultiCover)

#添加文件
set(file_list 
	tutorial.cpp
	stadfx.h
	Obmap.h
	Obmap.cpp
	bgeometry.h
	bgeometry.cpp
	bgraph.h
	bgraph.cpp
	ReadConfig.h
	ReadConfig.cpp
	ssconfig.hpp
	#
	simple_svg.hpp
	Gwrite.hpp
	Gwrite.cpp
	#
	STCPlan.h
	STCPlan.cpp
	GroupVert.h
	GroupVert.cpp
	MultiAuction.h
	MultiAuction.cpp
	#
	MultiAuctionSTC.h
	MultiAuctionSTC.cpp
	#EST
	MultiAuctionSTCEst.h
	MultiAuctionSTCEst.cpp
	AstarPlan.h
	AstarPlan.cpp
)

set(DARP_file_list tutorial_DARP.cpp
	stadfx.h
	Obmap.h
	Obmap.cpp
	bgeometry.h
	bgeometry.cpp
	bgraph.h
	bgraph.cpp
	ReadConfig.h
	ReadConfig.cpp
	ssconfig.hpp
	#
	simple_svg.hpp
	Gwrite.hpp
	Gwrite.cpp
	#
	STCPlan.h
	STCPlan.cpp
	GroupVert.h
	GroupVert.cpp
	DARP.hpp
	DARP.cpp
	#
	)


IF(WIN32)
#set(BOOST_ROOT "D:/boost/boost_1_65_0_64")
set(BOOST_ROOT "D:/boost_1_64_0/boost_1_64_0_64")
ENDIF()
#include_directories(../include)
#find_package(Boost 1.65.0)

find_package(Boost 1.64.0)

if(Boost_FOUND)

include_directories(${Boost_INCLUDE_DIRS})

message(${Boost_INCLUDE_DIRS})

endif()
#lib文件所存在的位置
#set(PROJECT_LIB_DIR ${PROJECT_SOURCE_DIR}/src)

#输出包含文件信息
message(${file_list})
message(${PROJECT_BINARY_DIR})
#message(${PROJECT_LIB_DIR})


#生成DLL
add_executable(MultiCoverPathPlan ${file_list})
add_executable(DARP ${DARP_file_list})
add_executable(primTest prim.cpp)
add_executable(svgTest tutorial_svg.cpp simple_svg.hpp)
add_executable(componentTest component.cpp)
#add_executable(test_pathplan  ${PROJECT_LIB_DIR}/${file_list})

#add_library(coveragePathPlan  SHARED ${file_list})

#链接外部DLL
#target_link_libraries(libPathPlanning libGPS)

#设置执行文件的输出路径
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exc)
#设置lib的输出路径
#set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/exc)

#设置输出名称
set_target_properties(MultiCoverPathPlan PROPERTIES OUTPUT_NAME "MultiCover")


# SHARE or STATIC
#add_library(pathplan SHARED  ${file_list})

#target_compile_definitions(pathplan PRIVATE __EXPORT_LIB)
#target_link_libraries(pathplan libgps2local)

#add_executable(test_pathplan TestPathPlan.cpp)

#target_link_libraries(test_pathplan pathplan libgps2local)

if (UNIX)
    target_compile_options(pathplan PRIVATE -std=c++11)
    target_compile_options(test_pathplan PRIVATE -std=c++11)
    message(STATUS "Using -std=c++11")
endif()
