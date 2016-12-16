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
	static URStreamer* streamer;
	static bool USEISO;
	enum IDS {TIGHT_16=1, LOOSE_16=2};
	double CorPFIsolation() const;
	bool ID(IDS idtyp);

};

#endif
