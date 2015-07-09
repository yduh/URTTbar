#include "GenJet.h"
#include "TMath.h" 
#include "Analyse.h"

GenJet::GenJet(IOGenAK4Jet p) : IOGenAK4Jet(p), TLorentzVector(p.px(), p.py(), p.pz(), p.e()) {}

