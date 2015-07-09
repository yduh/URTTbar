include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(ATHeads helper.h OPhoton.h OMuon.h OElectron.h OJet.h ConfigParser.h)
set(ATSrcs helper.cc OPhoton.cc OMuon.cc OElectron.cc OJet.cc ConfigParser.cc)
add_library(AnalysisSupport SHARED ${ATSrcs} ${ATHeads})
target_link_libraries(AnalysisSupport AnalysisTool ${ROOT_LIBRARIES} -lTMVA)

install(TARGETS AnalysisSupport DESTINATION lib)
#install(FILES helper.h OPhoton.h OMuon.h OElectron.h OJet.h GluonJetFinder.h ME.h RAMBO.h DESTINATION include/AnalysisTool/)
install(FILES helper.h OMuon.h OElectron.h OJet.h OPhoton.h ConfigParser.h DESTINATION include/AnalysisTool/)
