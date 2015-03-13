#ifndef URElectronsSelector_h
#define URElectronsSelector_h



#include "URGenericElectronsSelector.h"
// #include "URStreamer.h"



typedef URGenericElectronsSelector<Electron, &URStreamer::electrons> URElectronsSelector;


#endif // URElectronsSelector_h
