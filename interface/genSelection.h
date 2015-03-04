#ifndef genSelection_h
#define genSelection_h

#include "Hypotheses.h"
#include "URStreamer.h"
#include <vector>

int Collapse(int root, std::vector<const Genparticle*> &particles);
TTbarHypothesis SelectGenParticles(URStreamer& event);

#endif
