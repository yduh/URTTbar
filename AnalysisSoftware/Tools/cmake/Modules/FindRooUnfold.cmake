set(RooUnfold_INCLUDE_DIRS $ENV{ATINCLUDE}/RooUnfold)
find_library(RooUnfold_LIBRARY_A NAMES RooUnfold HINTS $ENV{ATLIB})
set(RooUnfold_LIBRARIES ${RooUnfold_LIBRARY_A})
MESSAGE(STATUS "Looking for RooUnfold ... ${RooUnfold_LIBRARIES}")
