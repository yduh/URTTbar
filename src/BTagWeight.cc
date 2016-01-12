#include "IDJet.h"
#include "ttbarxsec.h"

#include <algorithm>
#include <cmath>

using namespace std;

BTagWeight::BTagWeight()
{
}

void BTagWeight::Init(ttbar* an, string filename, double bunc, double lunc)
{
	btyp = bunc;
	ltyp = lunc;
	AN = an;
	TDirectory* dir = gDirectory;
	probfile = new TFile(filename.c_str(), "READ");
	string mcname("P8");
	hborig = dynamic_cast<TH1D*>(probfile->Get((mcname + "_B").c_str()));
	hcorig = dynamic_cast<TH1D*>(probfile->Get((mcname + "_C").c_str()));
	hlorig = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L").c_str()));
	if(an->HERWIGPP){mcname = "Hpp";}
	if(an->PYTHIA6){mcname = "P6";}
	hbeff = dynamic_cast<TH1D*>(probfile->Get((mcname + "_B").c_str()));
	hceff = dynamic_cast<TH1D*>(probfile->Get((mcname + "_C").c_str()));
	hleff = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L").c_str()));
	dir->cd();
}

double BTagWeight::scaleb(IDJet* jet)
{
	double x = jet->Pt();
	double scale = -0.0443172+(0.00496634*(log(x+1267.85)*(log(x+1267.85)*(3.-(-0.110428*log(x+1267.85))))));
	//double scale = (0.647-1.315*exp(-0.0603*x))/(0.675-0.6178*exp(-0.05437*x));
	//double scale = 0.942;
	//double scale = Max(0.8, 1.118 - 0.00163*x);
	//double scale = 0.828 + 0.3244*Exp(-0.01059*x);
	//double scale = 0.965 - 3.841 *Exp(-0.09444*x);
	//double scale = Max(0.75, 0.9884-Exp((x-305.2)*0.01563));
	return(scale);
}

double BTagWeight::scalec(IDJet* jet)
{
	double x = jet->Pt();
	double scale = -0.0443172+(0.00496634*(log(x+1267.85)*(log(x+1267.85)*(3.-(-0.110428*log(x+1267.85))))));
	return(scale);
}
double BTagWeight::scalel(IDJet* jet)
{
	//double x = jet->Pt();
	double scale = 1.14022;
	return(scale);
}

double BTagWeight::SF(vector<IDJet*>& jets)
{
	double PD=1.;
	double PM=1.;
	for(IDJet* jet : jets)
	{
		//if(jet == bhad || jet == blep) {continue;}
		double pt = jet->Pt();
		int hbin = hbeff->FindFixBin(pt);
		int bin = Min(Max(hbin, 1), hbeff->GetNbinsX()-1)-1;
		if(find_if(AN->genbpartons.begin(), AN->genbpartons.end(), [&](GenObject* bp){return jet->DeltaR(*bp) < 0.3;}) != AN->genbpartons.end())
		{
			AN->truth1d["Eff_Ball"]->Fill(pt, AN->weight);
			double eff = hbeff->GetBinContent(hbin);
			double efforig = hborig->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_Bpassing"]->Fill(pt, AN->weight);
				PM *= eff;
				PD *= efforig * (scaleb(jet) + btyp * uncb[bin]);
			}
			else
			{
				PM *= 1. - eff;
				PD *= 1. - efforig * (scaleb(jet) + btyp * uncb[bin]);
			}
		}
		else if(find_if(AN->gencpartons.begin(), AN->gencpartons.end(), [&](GenObject* bp){return jet->DeltaR(*bp) < 0.3;}) != AN->gencpartons.end())
		{
			AN->truth1d["Eff_Call"]->Fill(pt, AN->weight);
			double eff = hceff->GetBinContent(hbin);
			double efforig = hcorig->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_Cpassing"]->Fill(pt, AN->weight);
				PM *= eff;
				PD *= efforig * (scalec(jet) + btyp * uncc[bin]);
			}
			else
			{
				PM *= 1. - eff;
				PD *= 1. - efforig * (scalec(jet) + btyp * uncc[bin]);
			}
		}
		else
		{
			AN->truth1d["Eff_Lall"]->Fill(pt, AN->weight);
			double eff = hleff->GetBinContent(hbin);
			double efforig = hlorig->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_Lpassing"]->Fill(pt, AN->weight);
				PM *= eff;
				PD *= efforig * (scalel(jet) + ltyp * uncl[bin]);
			}
			else
			{
				PM *= 1. - eff;
				PD *= 1. - efforig * (scalel(jet) + ltyp * uncl[bin]);
			}
		}
	}
	
	if(PM <= 0. || PD <= 0.){cout << PM << " BTWERROR " << PD << endl; return(1.);}
	return(PD/PM);
}
