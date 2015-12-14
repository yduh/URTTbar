#include "BtagEff.h"
#include <TTree.h>
#include <TRandom3.h>
#include <iostream>

#include "Permutation.h"
#include "IDJet.h"

BtagEff::BtagEff(){}

void BtagEff::Init(double btagsel)
{
	btagselection = btagsel;
	btagtree = new TTree("btagtree", "btagtree", 1);
	//btagtree->Branch("jwa", jwa, "jwa[5]/F");
	//btagtree->Branch("jwb", jwb, "jwb[5]/F");
	btagtree->Branch("j", j, "j[5]/F");
	btagtree->Branch("prob", &prob, "prob/F");
	btagtree->Branch("problep", &problep, "problep/F");
	btagtree->Branch("probhad", &probhad, "probhad/F");
	btagtree->Branch("probnu", &probnu, "probnu/F");
	btagtree->Branch("weight", &weight, "weight/F");
	btagtree->Branch("nvtx", &nvtx, "nvtx/F");
	btagtree->Branch("typ", &typ, "typ/I");
	btagtree->Branch("test", &test, "test/I");

}

void BtagEff::Fill(Permutation& per, float thenvtx, bool thadcorrect, bool tlepcorrect, double theweight)
{

	if(per.THad().Pt() < 80. || per.TLep().Pt() < 80.) {return;}

	typ = 0;
	if(thadcorrect) typ = 1;
	if(tlepcorrect) typ = 2;
	if(tlepcorrect && thadcorrect) typ = 3;
	weight = theweight;
	nvtx = thenvtx;
	prob = per.Prob();
	probhad = per.MassDiscr();
	problep = per.MTDiscr();
	probnu = per.NuDiscr();
	IDJet* blep = dynamic_cast<IDJet*>(per.BLep());
	IDJet* bhad = dynamic_cast<IDJet*>(per.BHad());
	IDJet* wja = dynamic_cast<IDJet*>(per.WJa());
	IDJet* wjb = dynamic_cast<IDJet*>(per.WJb());

	if(blep->csvIncl() > btagselection)
	{
		test = 1;
		j[0] = per.BHad()->Px(); j[1] = per.BHad()->Py(); j[2] = per.BHad()->Pz(); j[3] = per.BHad()->E(); j[4] = bhad->csvIncl();
		btagtree->Fill();

	}

	if(bhad->csvIncl() > btagselection)
	{
		test = 2;
		j[0] = per.BLep()->Px(); j[1] = per.BLep()->Py(); j[2] = per.BLep()->Pz(); j[3] = per.BLep()->E(); j[4] = blep->csvIncl();
		btagtree->Fill();
	}

	if(bhad->csvIncl() > btagselection || blep->csvIncl() > btagselection)
	{
		test = 3;
		j[0] = per.WJa()->Px(); j[1] = per.WJa()->Py(); j[2] = per.WJa()->Pz(); j[3] = per.WJa()->E(); j[4] = wja->csvIncl();
		btagtree->Fill();
		j[0] = per.WJb()->Px(); j[1] = per.WJb()->Py(); j[2] = per.WJb()->Pz(); j[3] = per.WJb()->E(); j[4] = wjb->csvIncl();
		btagtree->Fill();
	}

	if(bhad->csvIncl() < 0.6 && blep->csvIncl() < 0.6 && wja->csvIncl() < 0.6 && wjb->csvIncl() < 0.6)
	{
		test = 0;
		j[0] = per.BLep()->Px(); j[1] = per.BLep()->Py(); j[2] = per.BLep()->Pz(); j[3] = per.BLep()->E(); j[4] = blep->csvIncl();
		btagtree->Fill();
		j[0] = per.BHad()->Px(); j[1] = per.BHad()->Py(); j[2] = per.BHad()->Pz(); j[3] = per.BHad()->E(); j[4] = bhad->csvIncl();
		btagtree->Fill();
	}

}

