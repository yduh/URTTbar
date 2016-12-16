#ifndef IDMUON_H
#define IDMUON_H
#include "URStreamer.h"
#include "MCMatchable.h"

class IDMuon : public Muon, public MCMatchable
{
private:

public:
	static URStreamer* streamer;
	static bool USEISO;
	enum IDS {TIGHT_16, LOOSE_16};
	IDMuon(const Muon mu);
	double PFIsoDb();
	double CorPFIsolation2015();
	bool ID(IDS idtyp);
};
#endif
