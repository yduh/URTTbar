#ifndef IDJet_H
#define IDJet_H
#include "URStreamer.h"
#include "MCMatchable.h"

class IDJet : public Jet, public MCMatchable
{
private:

public:
	IDJet(const Jet el):
		Jet(el),
    MCMatchable()
		{
		}
	int flavor() const {return (match()) ? match()->pdgId() : partonFlavour();}
};

#endif
