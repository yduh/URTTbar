#ifndef IDMUON_H
#define IDMUON_H
#include "URStreamer.h"
#include "MCMatchable.h"

class IDMuon : public Muon, public MCMatchable
{
private:

public:
	static URStreamer* streamer;
	enum IDS {TIGHT_12, LOOSE_12, TIGHT_12Db, LOOSE_12Db};
	IDMuon(const Muon mu);
	double PFIsoDb();
	double CorPFIsolation2015();
	bool ID(IDS idtyp);
};
#endif
