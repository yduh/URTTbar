#include "Muon.h"
#include "TMath.h" 
#include "Analyse.h"

Muon::Muon(IOMuon mu) : IOMuon(mu), TLorentzVector(mu.px(), mu.py(), mu.pz(), TMath::Sqrt(mu.px()*mu.px() + mu.py()*mu.py() + mu.pz()*mu.pz())), TriggerObject(TriggerMatching(), GLAN->runlist.find(GLAN->Run())->second.HLTMuonAllNames()) {}
