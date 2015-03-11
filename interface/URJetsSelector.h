#ifndef URJetsSelector_h
#define URJetsSelector_h



#include "URGenericJetsSelector.h"
// #include "URStreamer.h"



typedef URGenericJetsSelector<Jet, &URStreamer::jets> URJetsSelector;


#endif // URJetsSelector_h
