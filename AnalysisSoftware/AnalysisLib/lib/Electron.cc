#include "Electron.h"
#include "TMath.h" 
#include "Analyse.h"

Electron::Electron(IOElectron el) : IOElectron(el), TLorentzVector(el.px(), el.py(), el.pz(), TMath::Sqrt(el.px()*el.px() + el.py()*el.py() + el.pz()*el.pz())), TriggerObject(TriggerMatching(), GLAN->runlist.find(GLAN->Run())->second.HLTElectronAllNames()) {}
