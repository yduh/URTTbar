#ifndef IDELECTRON_H
#define IDELECTRON_H
#include "URStreamer.h"
#include "MCMatchable.h"

class IDElectron : public Electron, public MCMatchable
{
private:

public:
	IDElectron(const Electron el) : 
		Electron(el),
		MCMatchable()
	{
	}
	//FIXME
	//why this here? it is likely to break 
  //if we ever move to threaded running!
	static URStreamer* streamer;
	static bool USEISO;
	enum IDS {MEDIUM_12=1, LOOSE_12=2, MEDIUM_12Db=3, LOOSE_12Db=4, MEDIUM_15=5, LOOSE_15=6, MEDIUM_15_25ns=7, LOOSE_15_25ns=8};
	double CorPFIsolation2012(double eta) const;
	double CorPFIsolation2015() const;
	double CorPFIsolation2015_25ns() const;
	double PFIsoDb() const;
	bool ID(IDS idtyp);

};

#endif
