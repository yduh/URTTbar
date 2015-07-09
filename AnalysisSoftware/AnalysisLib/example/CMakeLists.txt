include_directories(${CMAKE_SOURCE_DIR}/lib ${CMAKE_SOURCE_DIR}/libextra ${PROJECT_BINARY_DIR}/lib)

set(ExampleDYSRC ExampleDY.cc main.cc)
set(ExampleDYHEADER ExampleDY.h)
add_executable(ExampleDY ${ExampleDYSRC} ${ExampleDYHEADER})
target_link_libraries(ExampleDY AnalysisTool AnalysisSupport ${ROOT_LIBRARIES})


install(TARGETS ExampleDY DESTINATION bin)
