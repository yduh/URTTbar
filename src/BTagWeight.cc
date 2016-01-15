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
	hborigL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_B").c_str()));
	hcorigL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_C").c_str()));
	hlorigL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_L").c_str()));
	hborigM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_B").c_str()));
	hcorigM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_C").c_str()));
	hlorigM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_L").c_str()));
	if(an->HERWIGPP){mcname = "Hpp";}
	if(an->PYTHIA6){mcname = "P6";}
	hbeffL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_B").c_str()));
	hceffL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_C").c_str()));
	hleffL = dynamic_cast<TH1D*>(probfile->Get((mcname + "_L_L").c_str()));
	hbeffM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_B").c_str()));
	hceffM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_C").c_str()));
	hleffM = dynamic_cast<TH1D*>(probfile->Get((mcname + "_M_L").c_str()));
	dir->cd();
}

double BTagWeight::scalebM(IDJet* jet)
{
	double x = jet->Pt();
	double scale = -0.0443172+(0.00496634*(log(x+1267.85)*(log(x+1267.85)*(3.-(-0.110428*log(x+1267.85))))));//MEDIUM
	return(scale);
}

double BTagWeight::scalecM(IDJet* jet)
{
	double x = jet->Pt();
	double scale = -0.0443172+(0.00496634*(log(x+1267.85)*(log(x+1267.85)*(3.-(-0.110428*log(x+1267.85))))));//MEDIUM
	return(scale);
}
double BTagWeight::scalelM(IDJet* jet)
{
	double x = jet->Pt();
	double scale = 1.14022;//MEDIUM
	return(scale);
}

double BTagWeight::scalebL(IDJet* jet)
{
	double x = jet->Pt();
	double scale = 0.908299+(2.70877e-06*(log(x+370.144)*(log(x+370.144)*(3-(-(104.614*log(x+370.144)))))));//LOOSE
	return(scale);
}

double BTagWeight::scalecL(IDJet* jet)
{
	double x = jet->Pt();
	double scale = 0.908299+(2.70877e-06*(log(x+370.144)*(log(x+370.144)*(3-(-(104.614*log(x+370.144)))))));//LOOSE
	return(scale);
}

double BTagWeight::scalelL(IDJet* jet)
{
	double x = jet->Pt();
	double scale = ((1.07278+(0.000535714*x))+(-1.14886e-06*(x*x)))+(7.0636e-10*(x*(x*x)));//LOOSE
	return(scale);
}

double BTagWeight::UnclL(IDJet* jet)
{
	double x = jet->Pt();
	//double scale = 1.14022;//MEDIUM
	double scale = ((1.12921+(0.000804962*x))+(-1.87332e-06*(x*x)))+(1.18864e-09*(x*(x*x)));
	scale -= ((1.01637+(0.000265653*x))+(-4.22531e-07*(x*x)))+(2.23396e-10*(x*(x*x)));
	scale /=2;
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
		int hbin = hbeffM->FindFixBin(pt);
		int bin = Min(Max(hbin, 1), hbeffM->GetNbinsX()-1)-1;
		if(find_if(AN->genbpartons.begin(), AN->genbpartons.end(), [&](GenObject* bp){return jet->DeltaR(*bp) < 0.3;}) != AN->genbpartons.end())
		{
			AN->truth1d["Eff_Ball"]->Fill(pt, AN->weight);
			double effM = hbeffM->GetBinContent(hbin);
			double effMorig = hborigM->GetBinContent(hbin);
			double effL = hbeffL->GetBinContent(hbin);
			double effLorig = hborigL->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_BpassingM"]->Fill(pt, AN->weight);
				PM *= effM;
				PD *= effMorig * (scalebM(jet) + btyp * uncbM[bin]);
			}
			else if(jet->csvIncl() > AN->B_LOOSE)
			{
				AN->truth1d["Eff_BpassingL"]->Fill(pt, AN->weight);
				PM *= effL;
				PD *= (effLorig+effMorig)*(scalebL(jet) + btyp * uncbL[bin]) - effMorig*(scalebM(jet) + btyp * uncbM[bin]);
			}
			else
			{
				PM *= 1. - effM - effL;
				PD *= 1. - (effLorig+effMorig)*(scalebL(jet) + btyp * uncbL[bin]);
			}
		}
		else if(find_if(AN->gencpartons.begin(), AN->gencpartons.end(), [&](GenObject* bp){return jet->DeltaR(*bp) < 0.3;}) != AN->gencpartons.end())
		{
			AN->truth1d["Eff_Call"]->Fill(pt, AN->weight);
			double effM = hceffM->GetBinContent(hbin);
			double effMorig = hcorigM->GetBinContent(hbin);
			double effL = hceffL->GetBinContent(hbin);
			double effLorig = hcorigL->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_CpassingM"]->Fill(pt, AN->weight);
				PM *= effM;
				PD *= effMorig * (scalecM(jet) + btyp * unccM[bin]);
			}
			else if(jet->csvIncl() > AN->B_LOOSE)
			{
				AN->truth1d["Eff_CpassingL"]->Fill(pt, AN->weight);
				PM *= effL;
				PD *= (effLorig+effMorig)*(scalecL(jet) + btyp * unccL[bin]) - effMorig*(scalecM(jet) + btyp * unccM[bin]);
			}
			else
			{
				PM *= 1. - effM - effL;
				PD *= 1. - (effMorig+effLorig)*(scalecL(jet) + btyp * unccL[bin]);
			}
		}
		else
		{
			AN->truth1d["Eff_Lall"]->Fill(pt, AN->weight);
			double effM = hleffM->GetBinContent(hbin);
			double effMorig = hlorigM->GetBinContent(hbin);
			double effL = hleffL->GetBinContent(hbin);
			double effLorig = hlorigL->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_LpassingM"]->Fill(pt, AN->weight);
				PM *= effM;
				PD *= effMorig * (scalelM(jet) + ltyp * unclM[bin]);
			}
			else if(jet->csvIncl() > AN->B_LOOSE)
			{
				AN->truth1d["Eff_LpassingL"]->Fill(pt, AN->weight);
				PM *= effL;
				PD *= (effLorig+effMorig)*(scalelL(jet) + ltyp * UnclL(jet)) - effMorig*(scalelM(jet) + ltyp * unclM[bin]);
			}
			else
			{
				PM *= 1. - effM - effL;
				PD *= 1. - (effMorig+effLorig)*(scalelL(jet) + ltyp * UnclL(jet));
			}
		}
	}
	
	if(PM <= 0. || PD <= 0.){cout << PM << " BTWERROR " << PD << endl; return(1.);}
	return(PD/PM);
}
