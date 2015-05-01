#include "JetScale.h"
#include "Permutation.h"

JetScale::JetScale(){}

JetScale::~JetScale(){}

void JetScale::Init(string treename)
{
	tree = new TTree(treename.c_str(), treename.c_str(), 1);
	tree->Branch("minp", minp, "minp[4]/F");
	tree->Branch("maxp", maxp, "maxp[4]/F");
	tree->Branch("weight", &weight_, "weight/F");
	tree->Branch("typ", &type, "typ/i");
}

void JetScale::Fill(const Permutation& per, bool wcorrect, float weight)
{
	TLorentzVector* jmin = per.WJa()->Pt() < per.WJb()->Pt() ? per.WJa() : per.WJb();
	TLorentzVector* jmax = per.WJa()->Pt() > per.WJb()->Pt() ? per.WJa() : per.WJb();
	weight_ = weight;
	minp[0] = jmin->Px();
	minp[1] = jmin->Py();
	minp[2] = jmin->Pz();
	minp[3] = jmin->E();
	maxp[0] = jmax->Px();
	maxp[1] = jmax->Py();
	maxp[2] = jmax->Pz();
	maxp[3] = jmax->E();
	type = 0;
	if(wcorrect) type = 1;
	tree->Fill();
}

