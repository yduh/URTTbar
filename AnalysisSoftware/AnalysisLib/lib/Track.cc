#include "Track.h"
#include "TMath.h" 
#include "Analyse.h"

Track::Track(IOTrack trk) : IOTrack(trk), TLorentzVector(trk.px(), trk.py(), trk.pz(), Sqrt(trk.px()*trk.px() + trk.py()*trk.py() + trk.pz()*trk.pz())) {}
