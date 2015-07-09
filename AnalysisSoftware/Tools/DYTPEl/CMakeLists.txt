#include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(DYTPELSRC DYTPEl.cc main.cc)
set(DYTPELHEADER DYTPEl.h)
add_executable(AnalysisDYTPEl ${DYTPELSRC} ${DYTPELHEADER})
target_link_libraries(AnalysisDYTPEl ${URAnalysis_LIBRARIES} ${ROOT_LIBRARIES})

install(TARGETS AnalysisDYTPEl DESTINATION bin)
