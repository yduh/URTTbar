#include "TTBarSolver.h"
#include "NeutrinoSolver.h"


TTBarSolver* TTBarSolver::TTBS = 0; 

void myfuncln(Int_t& npar, Double_t* deriv, Double_t& val, Double_t* par, Int_t flag)
{
	val = TTBarSolver::TTBS->Test(par);
}

TTBarSolver::TTBarSolver() : minuit(9), probfile(0)
{
}

TTBarSolver::~TTBarSolver()
{
	if(probfile != 0) {probfile->Close();}
}

void TTBarSolver::Init(string filename, bool WTMASS)
{
	FULLMASS = WTMASS;
	TDirectory* dir = gDirectory;
	probfile = new TFile(filename.c_str(), "READ");
	WTmass_right = dynamic_cast<TH2D*>(probfile->Get("TRUTH/truth_Wmasshad_tmasshad_right"));
	WTmass_wrong = dynamic_cast<TH2D*>(probfile->Get("TRUTH/truth_Wmasshad_tmasshad_wrong"));
	Tmass_right = WTmass_right->ProjectionY();
	Tmass_wrong = WTmass_wrong->ProjectionY();
	BTag_right = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_btag_true"));
	BTag_wrong = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_btag_wrong"));
	N_right = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_nschi_right"));
	N_wrong = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_nschi_wrong"));
	dir->cd();
}

void TTBarSolver::Solve(OJet* bhad, OJet* j1had, OJet* j2had, OJet* blep, TLorentzVector* llep, IOMET* met)
{
	bhad_ = bhad;
	j1had_ = j1had;
	j2had_ = j2had;
	blep_ = blep;
	llep_ = llep;
	met_ = met;
	bool SMEAR = false;
	bool LEPMASS = false;
	ubhad_ = 0.05;
	uj1had_ = 0.05;
	uj2had_ = 0.05;
	ublep_ = 0.05;
	ullep_ = 0.01;
	umetx_ = Sqrt(met_->pxUnc());
	umety_ = Sqrt(met_->pyUnc());
	rhomet_ = met_->pxpyUnc()/(umetx_*umety_);

	nschi = -1;
	res = 1.E10;
	nstest = 1.E10;
	masstest = 1.E10;
	btagtest = -1.*Log(BTag_right->Interpolate(bhad->BTagCSVv2())/BTag_wrong->Interpolate(bhad->BTagCSVv2()));
	btagtest -= Log(BTag_right->Interpolate(blep->BTagCSVv2())/BTag_wrong->Interpolate(blep->BTagCSVv2()));
	//btagtest -= Log(BTag_wrong->Interpolate(j1had->BTagCSVv2())/BTag_right->Interpolate(j1had->BTagCSVv2()));
	//btagtest -= Log(BTag_wrong->Interpolate(j2had->BTagCSVv2())/BTag_right->Interpolate(j2had->BTagCSVv2()));

//	double Mw = 80.;
//	double A = Mw*Mw/2. + llep->Px()*met->Px() + llep->Py()*met->Py();
//	double h1 = (llep->Pz()*llep->Pz() - llep->E()*llep->E());
//	double p = 2.*A*llep->Pz()/h1;
//	double q = (A*A-llep->E()*llep->E()*(met->Px()*met->Px() + met->Py()*met->Py()))/h1;
//	double npz1 = -0.5*p;
//	double npz2 = -0.5*p;
//	if(0.25*p*p - q > 0)
//	{
//		npz1 += Sqrt(0.25*p*p - q);
//		npz2 -= Sqrt(0.25*p*p - q);
//	}
	
	TTBS = this;
	minuit.SetFCN(myfuncln);
	minuit.SetPrintLevel(-1);
	Int_t flags = 0;
	minuit.mnparm(0, "top mass", 173., 1., 10, 1000, flags);
	minuit.mnparm(1, "w mass", 80., 1., 10, 1000, flags);
	minuit.mnparm(2, "ubhad", 1, 0.01, 0.5, 1.5, flags);
	minuit.mnparm(3, "uj1had", 1, 0.01, 0.5, 1.5, flags);
	minuit.mnparm(4, "uj2had", 1, 0.01, 0.5, 1.5, flags);
	minuit.mnparm(5, "ublep", 1, 0.01, 0.5, 1.5, flags);
	minuit.mnparm(6, "ullep", 1, 0.01, 0.5, 1.5, flags);
	minuit.mnparm(7, "umetx", 1, 0.01, 0.5, 1.5, flags);
	minuit.mnparm(8, "umety", 1, 0.01, 0.5, 1.5, flags);
	//minuit.mnparm(9, "metz", neutrino.Pz(), 1., 0., 10000., flags);
	//minuit.mnparm(10, "wn mass", 80., 1., 10, 1000, flags);
	if(!LEPMASS)
	{
		minuit.FixParameter(0);
		minuit.FixParameter(1);
	}
	if(!SMEAR)
	{
		minuit.FixParameter(2);
		minuit.FixParameter(3);
		minuit.FixParameter(4);
		minuit.FixParameter(5);
		minuit.FixParameter(6);
		minuit.FixParameter(7);
		minuit.FixParameter(8);
	}
	if(!LEPMASS && !SMEAR)
	{
		double par[9];
		par[0] = 173.;
		par[1] = 80.;
		par[2] = 1.;
		par[3] = 1.;
		par[4] = 1.;
		par[5] = 1.;
		par[6] = 1.;
		par[7] = 1.;
		par[8] = 1.;
		Test(par);
	}
	else
	{
		minuit.SetMaxIterations(500);
		minuit.Migrad();
	}
}

//double TTBarSolver::Test(double* par)
//{
//	bhadT_ = TLorentzVector(bhad_->Px()*par[2], bhad_->Py()*par[2], bhad_->Pz()*par[2], bhad_->P()*par[2]);
//	j1hadT_ = TLorentzVector(j1had_->Px()*par[3], j1had_->Py()*par[3], j1had_->Pz()*par[3], j1had_->P()*par[3]);
//	j2hadT_ = TLorentzVector(j2had_->Px()*par[4], j2had_->Py()*par[4], j2had_->Pz()*par[4], j2had_->P()*par[4]);
//	blepT_ = TLorentzVector(blep_->Px()*par[5], blep_->Py()*par[5], blep_->Pz()*par[5], blep_->P()*par[5]);
//	llepT_ = TLorentzVector(llep_->Px()*par[6], llep_->Py()*par[6], llep_->Pz()*par[6], llep_->P()*par[6]);
//	
//	res = 0.;
//	double sqrt2 = Sqrt(1);
//	res += Power((j1hadT_ + j2hadT_).M() - par[1], 2)/Power(12.1*2., 2);
//	res += Power((j1hadT_ + j2hadT_ + bhadT_).M() - par[0], 2)/Power(17.3*2., 2);
//	res += Power((par[2]-1.)/ubhad_/sqrt2 , 2);
//	res += Power((par[3]-1.)/uj1had_/sqrt2 , 2);
//	res += Power((par[4]-1.)/uj2had_/sqrt2 , 2);
//	res += Power((par[5]-1.)/ublep_/sqrt2 , 2);
//	res += Power((par[6]-1.)/ullep_/sqrt2 , 2);
//	res += Power((par[7]-1.)/umetx_/sqrt2 , 2);
//	res += Power((par[8]-1.)/umety_/sqrt2 , 2);
//	res += Power((par[10]-80)/(12.1*2.), 2);
//
//	double metx = met_->Px() * par[7];
//	double mety = met_->Py() * par[8];
//
//	double Mw = par[10];
//	double A = Mw*Mw/2. + llepT_.Px()*metx + llepT_.Py()*mety;
//	double h1 = (llepT_.Pz()*llepT_.Pz() - llepT_.E()*llepT_.E());
//	double p = 2.*A*llepT_.Pz()/h1;
//	double q = (A*A-llepT_.E()*llepT_.E()*(metx*metx + mety*mety))/h1;
//	double npz1 = -0.5*p;
//	double npz2 = -0.5*p;
//	if(0.25*p*p - q > 0)
//	{
//		npz1 += Sqrt(0.25*p*p - q);
//		npz2 -= Sqrt(0.25*p*p - q);
//	}
//
//	TLorentzVector met1(metx, mety, npz1, Sqrt(metx*metx + mety*mety + npz1*npz1));
//	double res1 = Power((llepT_ + met1 + blepT_).M() - par[0], 2)/Power(17.3*2., 2);
//
//	TLorentzVector met2(metx, mety, npz1, Sqrt(metx*metx + mety*mety + npz1*npz1));
//	double res2 = Power((llepT_ + met2 + blepT_).M() - par[0], 2)/Power(17.3*2., 2);
//
//	if(res1 < res2)
//	{
//		metT_ = met1;
//		res += res1;
//	}
//	else
//	{
//		metT_ = met2;
//		res += res2;
//	}
//
//	return(res);
//}

double TTBarSolver::Test(double* par)
{
	bhadT_ = TLorentzVector(bhad_->Px()*par[2], bhad_->Py()*par[2], bhad_->Pz()*par[2], bhad_->E()*par[2]);
	j1hadT_ = TLorentzVector(j1had_->Px()*par[3], j1had_->Py()*par[3], j1had_->Pz()*par[3], j1had_->E()*par[3]);
	j2hadT_ = TLorentzVector(j2had_->Px()*par[4], j2had_->Py()*par[4], j2had_->Pz()*par[4], j2had_->E()*par[4]);
	blepT_ = TLorentzVector(blep_->Px()*par[5], blep_->Py()*par[5], blep_->Pz()*par[5], blep_->E()*par[5]);
	llepT_ = TLorentzVector(llep_->Px()*par[6], llep_->Py()*par[6], llep_->Pz()*par[6], llep_->E()*par[6]);
	NeutrinoSolver NS(&llepT_, &blepT_, par[1], par[0]);
	metT_ = TLorentzVector(NS.GetBest(met_->px()*par[7], met_->py()*par[8], umetx_, umety_, rhomet_, nschi));
	if(nschi > -0.1 && nschi < 50.)
	{
		nstest = -1.*Log(N_right->Interpolate(nschi)/N_wrong->Interpolate(nschi));
	}
	//if(metT_.Pt() < 1) {cout << metT_.Px() << " " << metT_.Py() << " " << met_->px()*par[7] << " " << met_->py()*par[8] << " " << umetx_ << " " << umety_ << " " << rhomet_ << endl;}

	if(FULLMASS)
	{
		double mwhad = (j1hadT_ + j2hadT_).M();
		double mthad = (j1hadT_ + j2hadT_ + bhadT_).M();
		//if(mthad > 490. || mwhad > 490. || mthad != mthad || mwhad != mwhad || WTmass_right->Interpolate(mwhad, mthad) < 1.)
		if(mthad > 490. || mwhad > 490. || WTmass_right->Interpolate(mwhad, mthad) < 1.)
		{
		}
		else
		{
			masstest = -1.*Log(WTmass_right->Interpolate(mwhad, mthad)/Max(1., WTmass_wrong->Interpolate(mwhad, mthad)));
		}
	}
	else
	{
		double mthad = (j1hadT_ + j2hadT_ + bhadT_).M();
		//if(mthad > 490. || mwhad > 490. || mthad != mthad || mwhad != mwhad || WTmass_right->Interpolate(mwhad, mthad) < 1.)
		if(mthad > 490. || Tmass_right->Interpolate(mthad) < 1.)
		{
		}
		else
		{
			masstest = -1.*Log(Tmass_right->Interpolate(mthad)/Max(1., Tmass_wrong->Interpolate(mthad)));
		}
	}


	//masstest = Power((mthad-173)/(30), 2) + Power((mwhad-80)/(20), 2);
	//double p0 = -0.006529;
	//double p1 = 75.52;
	//double p2 = 0.002706;
	//double p3 = 160.3;
	//double p4 = -0.002637;
	//masstest = -1.*(p0*(mwhad - p1)*(mwhad - p1) + 2.*p2*(mwhad - p1)*(mthad - p3) + p4*(mthad - p3)*(mthad - p3));

	res = 0.;
	double sqrt2 = Sqrt(2);
	res += Power((par[0]-173)/(20.), 2);
	res += Power((par[1]-80)/(20.), 2);
	res += Power((par[2]-1.)/ubhad_/sqrt2 , 2);
	res += Power((par[3]-1.)/uj1had_/sqrt2 , 2);
	res += Power((par[4]-1.)/uj2had_/sqrt2 , 2);
	res += Power((par[5]-1.)/ublep_/sqrt2 , 2);
	res += Power((par[6]-1.)/ullep_/sqrt2 , 2);
	res += Power((par[7]-1.)/umetx_/sqrt2 , 2);
	res += Power((par[8]-1.)/umety_/sqrt2 , 2);
	res += nstest;
	res += btagtest;
	res += masstest;
	//res += Power((par[10]-80)/(3.), 2);

//	double metx = met_->Px() * par[7];
//	double mety = met_->Py() * par[8];
//
//	double Mw = par[10];
//	double A = Mw*Mw/2. + llepT_.Px()*metx + llepT_.Py()*mety;
//	double h1 = (llepT_.Pz()*llepT_.Pz() - llepT_.E()*llepT_.E());
//	double p = 2.*A*llepT_.Pz()/h1;
//	double q = (A*A-llepT_.E()*llepT_.E()*(metx*metx + mety*mety))/h1;
//	double npz1 = -0.5*p;
//	double npz2 = -0.5*p;
//	if(0.25*p*p - q > 0)
//	{
//		npz1 += Sqrt(0.25*p*p - q);
//		npz2 -= Sqrt(0.25*p*p - q);
//	}
//
//	TLorentzVector met1(metx, mety, npz1, Sqrt(metx*metx + mety*mety + npz1*npz1));
//	//double res1 = Power((llepT_ + met1 + blepT_).M() - 173., 2)/Power(17.3*2., 2);
//	double res1 = Power((llepT_ + met1 + blepT_).M() - 173., 2)/Power(30., 2);
//
//	TLorentzVector met2(metx, mety, npz1, Sqrt(metx*metx + mety*mety + npz1*npz1));
//	//double res2 = Power((llepT_ + met2 + blepT_).M() - 173., 2)/Power(17.3*2., 2);
//	double res2 = Power((llepT_ + met2 + blepT_).M() - 173., 2)/Power(30., 2);
//
//	if(res1 < res2)
//	{
//		metT_ = met1;
//		res += res1;
//	}
//	else
//	{
//		metT_ = met2;
//		res += res2;
//	}

	return(res);
}

