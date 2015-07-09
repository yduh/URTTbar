#include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(TopFitterSRC TopFitter.cc)
add_executable(TopFitter ${TopFitterSRC})
target_link_libraries(TopFitter ${URAnalysis_LIBRARIES} ${ROOT_LIBRARIES} -lRooFit -lRooFitCore)

set(TopFitter2fitSRC TopFitter2fit.cc)
add_executable(TopFitter2fit ${TopFitter2fitSRC})
target_link_libraries(TopFitter2fit ${URAnalysis_LIBRARIES} ${ROOT_LIBRARIES} -lRooFit -lRooFitCore)

set(TopFitterBTAGSRC TopFitterBTAG.cc)
add_executable(TopFitterBTAG ${TopFitterBTAGSRC})
target_link_libraries(TopFitterBTAG ${URAnalysis_LIBRARIES} ${ROOT_LIBRARIES} -lRooFit -lRooFitCore)

set(JetScaleFitterSRC JetScaleFitter.cc)
add_executable(JetScaleFitter ${JetScaleFitterSRC})
target_link_libraries(JetScaleFitter ${ROOT_LIBRARIES} -lRooFit -lRooFitCore)


set(JetScaleCalculatorSRC JetScaleCalculator.cc)
add_executable(JetScaleCalculator ${JetScaleCalculatorSRC})
target_link_libraries(JetScaleCalculator ${ROOT_LIBRARIES} -lRooFit -lRooFitCore)

#target_link_libraries(AnalysisTool ${ROOT_LIBRARIES})

install(TARGETS TopFitter TopFitter2fit TopFitterBTAG JetScaleFitter JetScaleCalculator DESTINATION bin)
