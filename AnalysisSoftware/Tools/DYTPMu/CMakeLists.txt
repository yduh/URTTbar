#include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(DYTPMUSRC DYTPMu.cc main.cc)
set(DYTPMUHEADER DYTPMu.h)
add_executable(AnalysisDYTPMu ${DYTPMUSRC} ${DYTPMUHEADER})
target_link_libraries(AnalysisDYTPMu ${URAnalysis_LIBRARIES} ${ROOT_LIBRARIES} -lTMVA -lMinuit)

install(TARGETS AnalysisDYTPMu DESTINATION bin)
