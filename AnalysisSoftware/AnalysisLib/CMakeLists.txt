cmake_minimum_required(VERSION 2.6)
project(AnalysisPackage)
set(CMAKE_C_FLAGS "-Wall -g")
set(CMAKE_CXX_FLAGS "-std=c++11 -g -Wall")
#set(CMAKE_CXX_FLAGS "-O2 -g -std=c++11 -Wall -pedantic")


set(AP_VERSION 3.0.0)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
set(CMAKE_BUILD_TYPE Debug)

find_package(ROOT REQUIRED)
#find_package(LHAPDF REQUIRED)
include_directories(${ROOT_INCLUDE_DIR})

add_subdirectory(lib)
add_subdirectory(libextra)
add_subdirectory(apps)
#add_subdirectory(AnalysisZZ)
#add_subdirectory(AnalysisVBS4j2l)
#add_subdirectory(AnalysisVBS4j2l_TMVA)
#add_subdirectory(AnalysisZZ_TMVA)
add_subdirectory(example)
#add_subdirectory(VBS3Jet2l)
#add_subdirectory(external)
#set_source_files_properties(AnalysisToolUseThis PROPERTIES GENERATED true)
add_custom_target(AnalysisToolUseThis ALL DEPENDS AnalysisToolUseThis.in COMMAND sed -e 's,[@]prefix[@],${CMAKE_INSTALL_PREFIX},g' -e 's,[@]VERSION[@],${AP_VERSION},g' < AnalysisToolUseThis.in > ${CMAKE_BINARY_DIR}/AnalysisToolUseThis WORKING_DIRECTORY ${CMAKE_SOURCE_DIR})
install(PROGRAMS ${CMAKE_BINARY_DIR}/AnalysisToolUseThis DESTINATION bin)

