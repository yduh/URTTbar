#ifndef LEPEFFCORRECTIO_H
#define LEPEFFCORRECTIO_H
#include "URStreamer.h"
#include <TFile.h>
#include <TDirectory.h>
#include <TH1D.h>
#include <IDElectron.h>
#include <IDMuon.h>

#include <string>
#include <vector>


using namespace std;

class LepEffCorrection
{
	private:
		TFile* efffile = nullptr;
		TH1D* heta = nullptr;
		vector<TH1D*> effhists;
		double glerrq = 0.;

	public:
		void init(string filename, double glerr)
		{
			glerrq = glerr*glerr;
			TDirectory* dir = gDirectory;
			efffile = TFile::Open(filename.c_str());
			heta = dynamic_cast<TH1D*>(efffile->Get("bin_eta"));
			for(int i = 0 ; i < heta->GetNbinsX() ; ++i)
			{
				effhists.push_back(dynamic_cast<TH1D*>(efffile->Get(("SF_"+to_string(i)).c_str())));
			}
			dir->cd();
		}

		double correctionel(IDElectron* el, double sigma = 0.)
		{
			int etabin = heta->FindFixBin(el->SCeta())-1;
			double pt = min(el->Pt(), effhists[etabin]->GetXaxis()->GetXmax());
			int ptbin = effhists[etabin]->FindFixBin(pt);
			return effhists[etabin]->Interpolate(pt) + sigma*sqrt(pow(effhists[etabin]->GetBinError(ptbin), 2) + glerrq);
			//return effhists[etabin]->GetBinContent(ptbin) + sigma*sqrt(pow(effhists[etabin]->GetBinError(ptbin), 2) + glerrq);
		}

		double correctionmu(IDMuon* mu, double sigma = 0.)
		{
			double TRK = 0.99;
			int etabin = heta->FindFixBin(mu->Eta())-1;
			double pt = min(mu->Pt(), effhists[etabin]->GetXaxis()->GetXmax());
			int ptbin = effhists[etabin]->FindFixBin(pt);
			return effhists[etabin]->Interpolate(pt)*TRK + sigma*sqrt(pow(effhists[etabin]->GetBinError(ptbin), 2) + glerrq);
			//return effhists[etabin]->GetBinContent(ptbin) + sigma*sqrt(pow(effhists[etabin]->GetBinError(ptbin), 2) + glerrq);
		}
};
#endif
