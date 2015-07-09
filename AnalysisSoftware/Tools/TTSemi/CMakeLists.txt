#include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(TTSemiSRC TTBarBins.cc TTSemi.cc main.cc TTBarPlots.cc NeutrinoSolver.cc TTBarSolver.cc)
set(TTSemiHEADER TTBarBins.h TTSemi.h TTBarPlots.h NeutrinoSolver.h TTBarSolver.h)
add_executable(AnalysisTTsemiLep ${TTSemiSRC} ${TTSemiHEADER})
target_link_libraries(AnalysisTTsemiLep ${URAnalysis_LIBRARIES} ${ROOT_LIBRARIES} -lTMVA -lMinuit)

install(TARGETS AnalysisTTsemiLep DESTINATION bin)
