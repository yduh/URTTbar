#include "Jet.h"
#include "TMath.h" 
#include "Analyse.h"

Jet::Jet(IOPFJet jet) : IOPFJet(jet), TLorentzVector(jet.px(), jet.py(), jet.pz(), jet.e()), TriggerObject(TriggerMatching(), GLAN->runlist.find(GLAN->Run())->second.HLTJetAllNames()) {}
