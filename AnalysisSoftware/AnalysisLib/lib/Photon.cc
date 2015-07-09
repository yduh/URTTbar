#include "Photon.h"
#include "TMath.h" 
#include "Analyse.h"
#include "SuperCluster.h"

Photon::Photon(IOPhoton ph) : IOPhoton(ph), TLorentzVector(ph.px(), ph.py(), ph.pz(), TMath::Sqrt(ph.px()*ph.px() + ph.py()*ph.py() + ph.pz()*ph.pz())), TriggerObject(TriggerMatching(), GLAN->runlist.find(GLAN->Run())->second.HLTPhotonAllNames()) {}

//SuperCluster Photon::SC() {return SuperCluster(SC());}
