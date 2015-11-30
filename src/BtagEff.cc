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
	//btagtree->Branch("jwa", jwa, "jwa[5]/F");
	//btagtree->Branch("jwb", jwb, "jwb[5]/F");
	btagtree->Branch("j", j, "j[5]/F");
	btagtree->Branch("prob", &prob, "prob/F");
	btagtree->Branch("prob2", &prob2, "prob2/F");
	btagtree->Branch("weight", &weight, "weight/F");
	btagtree->Branch("nvtx", &nvtx, "nvtx/F");
	btagtree->Branch("typ", &typ, "typ/I");
	btagtree->Branch("test", &test, "test/I");

}

void BtagEff::Fill(Permutation& per, float thenvtx, bool filltyp, double theweight)
{
	typ = filltyp ? 1 : 2;
	weight = theweight;
	nvtx = thenvtx;
	prob = per.Prob();
	prob2 = per.MassDiscr();
	if(per.BLep()->csvIncl() > btagselection)
	{
		test = 1;
		j[0] = per.BHad()->Px(); j[1] = per.BHad()->Py(); j[2] = per.BHad()->Pz(); j[3] = per.BHad()->E(); j[4] = per.BHad()->csvIncl();
		btagtree->Fill();

	}

	if(per.BHad()->csvIncl() > btagselection)
	{
		test = 2;
		j[0] = per.BLep()->Px(); j[1] = per.BLep()->Py(); j[2] = per.BLep()->Pz(); j[3] = per.BLep()->E(); j[4] = per.BLep()->csvIncl();
		btagtree->Fill();
	}

	if(per.BHad()->csvIncl() > btagselection || per.BLep()->csvIncl() > btagselection)
	{
		test = 3;
		j[0] = per.WJa()->Px(); j[1] = per.WJa()->Py(); j[2] = per.WJa()->Pz(); j[3] = per.WJa()->E(); j[4] = per.WJa()->csvIncl();
		btagtree->Fill();
		j[0] = per.WJb()->Px(); j[1] = per.WJb()->Py(); j[2] = per.WJb()->Pz(); j[3] = per.WJb()->E(); j[4] = per.WJb()->csvIncl();
		btagtree->Fill();
	}

	if(per.BHad()->csvIncl() < 0.6 && per.BLep()->csvIncl() < 0.6 && per.WJa()->csvIncl() < 0.6 && per.WJb()->csvIncl() < 0.6)
	{
		test = 0;
		j[0] = per.BLep()->Px(); j[1] = per.BLep()->Py(); j[2] = per.BLep()->Pz(); j[3] = per.BLep()->E(); j[4] = per.BLep()->csvIncl();
		btagtree->Fill();
		j[0] = per.BHad()->Px(); j[1] = per.BHad()->Py(); j[2] = per.BHad()->Pz(); j[3] = per.BHad()->E(); j[4] = per.BHad()->csvIncl();
		btagtree->Fill();
	}

}

