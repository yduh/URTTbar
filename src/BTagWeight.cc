#include "IDJet.h"
#include "ttbarxsec.h"

#include <algorithm>

using namespace std;

BTagWeight::BTagWeight()
{
}

void BTagWeight::Init(ttbar* an, string filename)
{
	AN = an;
	TDirectory* dir = gDirectory;
	probfile = new TFile(filename.c_str(), "READ");
	hbpass = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_Eff_Bpassing"));
	hball = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_Eff_Ball"));
	hbpass->Divide(hball);
	hlpass = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_Eff_Lpassing"));
	hlall = dynamic_cast<TH1D*>(probfile->Get("TRUTH/truth_Eff_Lall"));
	hlpass->Divide(hlall);
	dir->cd();
}

double BTagWeight::SF(vector<IDJet*>& jets, int btyp, int ltyp)
{
	double PD=1.;
	double PM=1.;
	for(IDJet* jet : jets)
	{
		double pt = jet->Pt();
		int hbin = hbpass->FindFixBin(pt);
		int bin = Min(Max(hbin, 1), hbpass->GetNbinsX()-1)-1;
		//cout << pt << " " << hbin << " " << bin << " " << hbpass->GetBinContent(hbin) << " " << hlpass->GetBinContent(hbin) << endl;

		if(find_if(AN->genbpartons.begin(), AN->genbpartons.end(), [&](GenObject* bp){return jet->DeltaR(*bp) < 0.4;}) != AN->genbpartons.end())
		{
			AN->truth1d["Eff_Ball"]->Fill(pt, AN->weight);
			double eff = hbpass->GetBinContent(hbin);
			if(jet->csvIncl() > AN->B_MEDIUM)
			{
				AN->truth1d["Eff_Bpassing"]->Fill(pt, AN->weight);
				PM *= eff;
				PD *= eff * (scale + btyp * uncb[bin]);
			}
			else
			{
				PM *= 1. - eff;
				PD *= 1. - eff * (scale + btyp * uncb[bin]);
			}
			break;
		}

		double eff = hlpass->GetBinContent(hbin);
		AN->truth1d["Eff_Lall"]->Fill(pt, AN->weight);
		if(jet->csvIncl() > AN->B_MEDIUM)
		{
			AN->truth1d["Eff_Lpassing"]->Fill(pt, AN->weight);
			PM *= eff;
			PD *= eff * (scale + ltyp * uncl[bin]);
		}
		else
		{
			PM *= 1. - eff;
			PD *= 1. - eff * (scale + ltyp * uncl[bin]);
		}
	}
	return(PD/PM);
}
