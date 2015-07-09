#include "GenSelectedParticle.h"
#include "TMath.h" 
#include "Analyse.h"

GenSelectedParticle::GenSelectedParticle(SelectedGenParticle p) : SelectedGenParticle(p), TLorentzVector(p.px(), p.py(), p.pz(), TMath::Sqrt(p.px()*p.px() + p.py()*p.py() + p.pz()*p.pz())) {}

