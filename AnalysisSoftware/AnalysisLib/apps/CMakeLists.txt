include_directories(${CMAKE_SOURCE_DIR}/lib ${PROJECT_BINARY_DIR}/lib
/cvmfs/cms.cern.ch/slc6_amd64_gcc491/external/boost/1.57.0/include)

#set(LUMIxrootdSrcs LUMIxrootd.cc)
#add_executable(LUMIxrootd ${LUMIxrootdSrcs})
#target_link_libraries(LUMIxrootd AnalysisTool ${Boost_LIBRARIES} ${ROOT_LIBRARIES})

set(LUMIlocaldirSrcs LUMIlocaldir.cc)
add_executable(LUMIlocaldir ${LUMIlocaldirSrcs})
target_link_libraries(LUMIlocaldir AnalysisTool ${Boost_LIBRARIES} ${ROOT_LIBRARIES})

install(TARGETS LUMIlocaldir DESTINATION bin)
