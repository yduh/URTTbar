#include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(TopUnfoldingSRC TopUnfolding.cc)
add_executable(TopUnfolding ${TopUnfoldingSRC})
target_link_libraries(TopUnfolding ${URAnalysis_LIBRARIES} ${RooUnfold_LIBRARIES} ${ROOT_LIBRARIES} -lRooFit -lRooFitCore)
#target_link_libraries(AnalysisTool ${ROOT_LIBRARIES})

install(TARGETS TopUnfolding DESTINATION bin)
