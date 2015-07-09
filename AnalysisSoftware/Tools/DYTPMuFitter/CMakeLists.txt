#include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(DYTPMUSRC main.cc)
set(DYTPMUHEADER )
add_executable(AnalysisDYTPMuFitter ${DYTPMUSRC} ${DYTPMUHEADER})
target_link_libraries(AnalysisDYTPMuFitter ${ROOT_LIBRARIES} -lRooFit -lRooFitCore)

install(TARGETS AnalysisDYTPMuFitter DESTINATION bin)
