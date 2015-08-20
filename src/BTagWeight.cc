#include "IDJet.h"
#include "ttbarxsec.h"

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

double BTagWeight::SF(vector<IDJet*>& jets, int typ)
{
	double PD=1.;
	double PM=1.;
	for(size_t j = 0 ; j < jets.size() ; ++j)
	{
		int bin = hbpass->FindFixBin(Min(jets[j]->Pt(), 650.));
		bool isbjet = false;
		for(size_t b = 0 ; b < AN->genbpartons.size() ; ++b)
		{
			if(jets[j]->DeltaR(*AN->genbpartons[b]) < 0.4)
			{
				isbjet = true;
				double eff = hbpass->GetBinContent(bin);
				if(jets[j]->csvIncl() > AN->B_MEDIUM)
				{
					PM *= eff;
					PD *= eff * (scale + typ * uncb[bin-1]);
				}
				else
				{
					PM *= 1. - eff;
					PD *= 1. - eff * (scale + typ * uncb[bin-1]);
				}
				break;
			}
		}
		if(!isbjet)
		{
			double eff = hlpass->GetBinContent(bin);
			if(jets[j]->csvIncl() > AN->B_MEDIUM)
			{
				PM *= eff;
				PD *= eff * (scale + typ * uncl[bin-1]);
			}
			else
			{
				PM *= 1. - eff;
				PD *= 1. - eff * (scale + typ * uncl[bin-1]);
			}
		}
	}
	return(PD/PM);
}
