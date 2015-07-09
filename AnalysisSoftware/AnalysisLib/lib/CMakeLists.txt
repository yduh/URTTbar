add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/BASEIO.h ${CMAKE_CURRENT_BINARY_DIR}/BASEIO.cc COMMAND python2 ${CMAKE_SOURCE_DIR}/createIOCode.py ${CMAKE_SOURCE_DIR}/lib/content.def BASEIO DEPENDS ${CMAKE_SOURCE_DIR}/createIOCode.py ${CMAKE_SOURCE_DIR}/lib/content.def)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/BASEIORUN.h ${CMAKE_CURRENT_BINARY_DIR}/BASEIORUN.cc COMMAND python2 ${CMAKE_SOURCE_DIR}/createIOCode.py ${CMAKE_SOURCE_DIR}/lib/contentRUN.def BASEIORUN DEPENDS ${CMAKE_SOURCE_DIR}/createIOCode.py ${CMAKE_SOURCE_DIR}/lib/contentRUN.def)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/BASEIOLUMI.h ${CMAKE_CURRENT_BINARY_DIR}/BASEIOLUMI.cc COMMAND python2 ${CMAKE_SOURCE_DIR}/createIOCode.py ${CMAKE_SOURCE_DIR}/lib/contentLUMI.def BASEIOLUMI DEPENDS ${CMAKE_SOURCE_DIR}/createIOCode.py ${CMAKE_SOURCE_DIR}/lib/contentLUMI.def)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(ATheads Track.h GenSimpleParticle.h GenJet.h GenSelectedParticle.h Photon.h SuperCluster.h GenBasicParticle.h Muon.h Electron.h Jet.h Analyse.h Luminosity.h Run.h TriggerSelection.h TriggerObject.h iohelper.h ${PROJECT_BINARY_DIR}/lib/BASEIO.h ${PROJECT_BINARY_DIR}/lib/BASEIORUN.h ${PROJECT_BINARY_DIR}/lib/BASEIOLUMI.h)
set(ATSrcs Track.cc GenSimpleParticle.cc GenJet.cc GenSelectedParticle.cc Photon.cc SuperCluster.cc GenBasicParticle.cc Muon.cc Electron.cc Jet.cc Analyse.cc Luminosity.cc Run.cc TriggerSelection.cc TriggerObject.cc iohelper.cc ${CMAKE_CURRENT_BINARY_DIR}/BASEIO.cc ${CMAKE_CURRENT_BINARY_DIR}/BASEIORUN.cc ${CMAKE_CURRENT_BINARY_DIR}/BASEIOLUMI.cc)

add_library(AnalysisTool SHARED ${ATSrcs} ${ATheads})
target_link_libraries(AnalysisTool ${ROOT_LIBRARIES})

install(TARGETS AnalysisTool DESTINATION lib)
install(FILES ${ATheads} DESTINATION include/AnalysisTool/)
