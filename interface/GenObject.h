#ifndef GENOBJECT_H
#define GENOBJECT_H
#include "URStreamer.h"

class GenObject : public TLorentzVector
{
private:
	int pdgId_;
	int status_;

public:
	GenObject(const Genparticle& gp) : TLorentzVector(gp), pdgId_(gp.pdgId()), status_(gp.status())
	{
	}
	GenObject(const Pst& gp) : TLorentzVector(gp), pdgId_(gp.pdgId()), status_(gp.status())
	{
	}
	GenObject(const Genjet& jet) : TLorentzVector(jet), pdgId_(0), status_(0)
	{
	}
	
	int pdgId() const {return pdgId_;}
	int status() const {return status_;}

};
#endif
