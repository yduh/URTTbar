#ifndef IDELECTRON_H
#define IDELECTRON_H
#include "URStreamer.h"
#include "MCMatchable.h"
#include <TMath.h>

using namespace TMath;
using namespace std;

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
	enum IDS {MEDIUM_12, LOOSE_12};
	IDElectron(const Electron el);
	double CorPFIsolation2012(double eta) const;
	bool ID(IDS idtyp);

};

#endif
