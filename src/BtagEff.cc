#include "BtagEff.h"
#include <TTree.h>
#include <TRandom3.h>
#include <iostream>

#include <algorithm>

#include "Permutation.h"
#include "IDJet.h"
#include "IDMet.h"

BtagEff::BtagEff(){}

void BtagEff::Init(double btagsel, double bkgcutmin, double bkgcutmax)
{
	bkgcutmin_ = bkgcutmin;
	bkgcutmax_ = bkgcutmax;
	btagselection = btagsel;
	btagtree = new TTree("btagtree", "btagtree", 1);
	//btagtree->Branch("jwa", jwa, "jwa[5]/F");
	//btagtree->Branch("jwb", jwb, "jwb[5]/F");
	btagtree->Branch("j", j, "j[7]/F");
	btagtree->Branch("prob", &prob, "prob/F");
	btagtree->Branch("problep", &problep, "problep/F");
	btagtree->Branch("probhad", &probhad, "probhad/F");
	btagtree->Branch("probnu", &probnu, "probnu/F");
	btagtree->Branch("met", &met, "met/F");
	btagtree->Branch("weight", &weight, "weight/F");
	btagtree->Branch("nvtx", &nvtx, "nvtx/F");
	btagtree->Branch("typ", &typ, "typ/I");
	btagtree->Branch("test", &test, "test/I");

}

void BtagEff::Fill(Permutation& per, float thenvtx, bool thadcorrect, bool tlepcorrect, double theweight)
{

	typ = 0;
	if(thadcorrect) typ = 1;
	if(tlepcorrect) typ = 2;
	if(tlepcorrect && thadcorrect) typ = 3;
	weight = theweight;
	nvtx = thenvtx;
	prob = per.Prob();
	probhad = per.MassDiscr();
	problep = per.MTDiscr();
	probnu = TMath::Sqrt(per.NuChisq());
	met = per.MET()->Pt();
	IDJet* blep = dynamic_cast<IDJet*>(per.BLep());
	IDJet* bhad = dynamic_cast<IDJet*>(per.BHad());
	IDJet* wja = dynamic_cast<IDJet*>(per.WJa());
	IDJet* wjb = dynamic_cast<IDJet*>(per.WJb());

	if(blep->csvIncl() > btagselection)
	{
		test = 1;
		j[0] = per.BHad()->Px(); j[1] = per.BHad()->Py(); j[2] = per.BHad()->Pz(); j[3] = per.BHad()->E(); j[4] = bhad->csvIncl(); j[5] = bhad->CombinedMVA(); j[6] = -1;
		if(bhad->CvsB() > -0.17 && bhad->CvsL() > -0.48){ j[6] = 0;}
		if(bhad->CvsB() > 0.08 && bhad->CvsL() > -0.1){ j[6] = 1;}
		if(bhad->CvsB() > -0.45 && bhad->CvsL() > 0.69){ j[6] = 2;}
		btagtree->Fill();

	}

	if(bhad->csvIncl() > btagselection)
	{
		test = 2;
		j[0] = per.BLep()->Px(); j[1] = per.BLep()->Py(); j[2] = per.BLep()->Pz(); j[3] = per.BLep()->E(); j[4] = blep->csvIncl(); j[5] = blep->CombinedMVA(); j[6] = -1;
		if(blep->CvsB() > -0.17 && blep->CvsL() > -0.48){ j[6] = 0;}
		if(blep->CvsB() > 0.08 && blep->CvsL() > -0.1){ j[6] = 1;}
		if(blep->CvsB() > -0.45 && blep->CvsL() > 0.69){ j[6] = 2;}
		btagtree->Fill();
	}

	if(bhad->csvIncl() > btagselection && blep->csvIncl() > btagselection)
	{
		test = 3;
		j[0] = per.WJa()->Px(); j[1] = per.WJa()->Py(); j[2] = per.WJa()->Pz(); j[3] = per.WJa()->E(); j[4] = wja->csvIncl(); j[5] = wja->CombinedMVA(); j[6] = -1;
		if(wja->CvsB() > -0.17 && wja->CvsL() > -0.48){ j[6] = 0;}
		if(wja->CvsB() > 0.08 && wja->CvsL() > -0.1){ j[6] = 1;}
		if(wja->CvsB() > -0.45 && wja->CvsL() > 0.69){ j[6] = 2;}
		btagtree->Fill();
		j[0] = per.WJb()->Px(); j[1] = per.WJb()->Py(); j[2] = per.WJb()->Pz(); j[3] = per.WJb()->E(); j[4] = wjb->csvIncl(); j[5] = wjb->CombinedMVA(); j[6] = -1;
		if(wjb->CvsB() > -0.17 && wjb->CvsL() > -0.48){ j[6] = 0;}
		if(wjb->CvsB() > 0.08 && wjb->CvsL() > -0.1){ j[6] = 1;}
		if(wjb->CvsB() > -0.45 && wjb->CvsL() > 0.69){ j[6] = 2;}
		btagtree->Fill();
	}

	double bmax = max({bhad->csvIncl(), blep->csvIncl(), wja->csvIncl(), wjb->csvIncl()});
	if(bmax > bkgcutmin_ && bmax < bkgcutmax_)
	{
		test = -1;
		j[0] = per.BHad()->Px(); j[1] = per.BHad()->Py(); j[2] = per.BHad()->Pz(); j[3] = per.BHad()->E(); j[4] = -1.; j[5] = -1.; j[6] = -1.;
		btagtree->Fill();
		test = -2;
		j[0] = per.BLep()->Px(); j[1] = per.BLep()->Py(); j[2] = per.BLep()->Pz(); j[3] = per.BLep()->E(); j[4] = -1.; j[5] = -1.; j[6] = -1.;
		btagtree->Fill();
		test = -3;
		j[0] = per.WJa()->Px(); j[1] = per.WJa()->Py(); j[2] = per.WJa()->Pz(); j[3] = per.WJa()->E(); j[4] = -1.; j[5] = -1.; j[6] = -1.;
		btagtree->Fill();
		j[0] = per.WJb()->Px(); j[1] = per.WJb()->Py(); j[2] = per.WJb()->Pz(); j[3] = per.WJb()->E(); j[4] = -1.; j[5] = -1.; j[6] = -1.;
		btagtree->Fill();
	}

}

