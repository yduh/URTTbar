#include "BtagEff.h"
#include <TTree.h>
#include <TRandom3.h>
#include <iostream>

#include "Permutation.h"

BtagEff::BtagEff(){}

void BtagEff::Init(double btagsel)
{
	btagselection = btagsel;
	btagtree = new TTree("btagtree", "btagtree", 1);
	btagtree->Branch("jwa", jwa, "jwa[5]/F");
	btagtree->Branch("jwb", jwb, "jwb[5]/F");
	btagtree->Branch("jb", jb, "jb[5]/F");
	btagtree->Branch("prob", &prob, "prob/F");
	btagtree->Branch("prob2", &prob2, "prob2/F");
	btagtree->Branch("weight", &weight, "weight/F");
	btagtree->Branch("nvtx", &nvtx, "nvtx/F");
	btagtree->Branch("typ", &typ, "typ/I");

}

void BtagEff::Fill(Permutation& per, float thenvtx, bool filltyp, double theweight)
{
	int ft = filltyp ? 1 : 2;

	bool coin = (gRandom->Uniform() < 0.5);
	Jet* bjet = 0;
	if(coin)
	{
		if(per.BLep()->csvIncl() > btagselection) bjet = per.BHad();
	}
	else
	{
		if(per.BHad()->csvIncl() > btagselection) bjet = per.BLep();
	}

	if(per.BHad()->csvIncl() < 0.6 && per.BLep()->csvIncl() < 0.6 && per.WJa()->csvIncl() < 0.6 && per.WJb()->csvIncl() < 0.6)
	{
		ft *= -1;
		if(coin)
		{
			bjet = per.BHad();
		}
		else
		{
			bjet = per.BLep();
		}
	}		
	if(bjet == 0) return;
	TLorentzVector thad = per.THad();
	//TLorentzVector tlep = per.TLep();
	if(thad.Pt() < 50.) return;
	//if(whad.M() < 60. || whad.M() > 90.) return;
	//if(thad.M() < 130. || thad.M() > 190.) return;

	jwa[0] = per.WJa()->Px(); jwa[1] = per.WJa()->Py(); jwa[2] = per.WJa()->Pz(); jwa[3] = per.WJa()->E(); jwa[4] = per.WJa()->csvIncl();
	jwb[0] = per.WJb()->Px(); jwb[1] = per.WJb()->Py(); jwb[2] = per.WJb()->Pz(); jwb[3] = per.WJb()->E(); jwb[4] = per.WJb()->csvIncl();
	jb[0] = bjet->Px(); jb[1] = bjet->Py(); jb[2] = bjet->Pz(); jb[3] = bjet->E(); jb[4] = bjet->csvIncl();
	prob = per.MassDiscr();
	prob2 = per.Prob();
	typ = ft;
	weight = theweight;
	nvtx = thenvtx;
	btagtree->Fill();
}

