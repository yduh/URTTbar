#include "BtagEff.h"
#include <TTree.h>
#include <TRandom3.h>
#include "Permutation.h"

BtagEff::BtagEff(){}

void BtagEff::Init()
{
	btagtree = new TTree("btagtree", "btagtree", 1);
	btagtree->Branch("jwa", jwa, "jwa[5]/F");
	btagtree->Branch("jwb", jwb, "jwb[5]/F");
	btagtree->Branch("jb", jb, "jb[5]/F");
	btagtree->Branch("prob", &prob, "prob/F");
	btagtree->Branch("weight", &weight, "weight/F");
	btagtree->Branch("typ", &typ, "typ/i");

}

void BtagEff::Fill(Permutation& per, int filltyp, double theweight)
{
	bool coin = (gRandom->Uniform() < 0.5);
	Jet* bjet = 0;
	if(coin)
	{
		if(per.BLep()->csvIncl() > 0.941) bjet = per.BHad();
	}
	else
	{
		if(per.BHad()->csvIncl() > 0.941) bjet = per.BLep();
	}
	if(bjet == 0) return;
	TLorentzVector thad = per.THad();
	if(thad.Pt() < 50.) return;

	jwa[0] = per.WJa()->Px(); jwa[1] = per.WJa()->Py(); jwa[2] = per.WJa()->Pz(); jwa[3] = per.WJa()->E(); jwa[4] = per.WJa()->csvIncl();
	jwb[0] = per.WJb()->Px(); jwb[1] = per.WJb()->Py(); jwb[2] = per.WJb()->Pz(); jwb[3] = per.WJb()->E(); jwb[4] = per.WJb()->csvIncl();
	jb[0] = bjet->Px(); jb[1] = bjet->Py(); jb[2] = bjet->Pz(); jb[3] = bjet->E(); jb[4] = bjet->csvIncl();
	prob = per.Prob();
	typ = filltyp;
	weight = theweight;
	btagtree->Fill();
}

