#ifndef IDELECTRON_H
#define IDELECTRON_H
#include "URStreamer.h"

using namespace TMath;
using namespace std;

class IDElectron : public Electron
{
private:

public:
	static URStreamer* streamer;
	enum IDS {MEDIUM_12, LOOSE_12};
	IDElectron(const Electron el);
	double CorPFIsolation2012(double eta) const;
	bool ID(IDS idtyp);

};

#endif
