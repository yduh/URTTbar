#ifndef URMuonsAndJetsSelector_h
#define URMuonsAndJetsSelector_h



#include "URGenericMuonsAndJetsSelector.h"



typedef URGenericMuonsAndJetsSelector<Muon, &URStreamer::muons, Jet, &URStreamer::jets> URMuonsAndJetsSelector;



#endif // URMuonsAndJetsSelector_h
