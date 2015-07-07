#ifndef HJETSCALER
#define HJETSCALER
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>

#include <TH1D.h>
#include <TDirectory.h>

#include "IDJet.h"


using namespace std;

class JetScaler
{
	private:
		TH1D* Heta;
		vector<TH1D*> HptsP;
		vector<TH1D*> HptsM;


	public:
		static URStreamer* streamer;
		JetScaler(const string filename)
		{
			TDirectory* olddir = gDirectory;
			TFile* f = new TFile(filename.c_str());
			Heta = dynamic_cast<TH1D*>(f->Get("eta"));
			for(int i = 0 ; i < Heta->GetNbinsX() ; ++i)
			{
				stringstream hn;
				hn << "down_" << i;
				HptsP.push_back(dynamic_cast<TH1D*>(f->Get(hn.str().c_str())));
			}	
			for(int i = 0 ; i < Heta->GetNbinsX() ; ++i)
			{
				stringstream hn;
				hn << "up_" << i;
				HptsM.push_back(dynamic_cast<TH1D*>(f->Get(hn.str().c_str())));
			}	

			olddir->cd();
		}
		double GetUncP(const IDJet& jet)
		{
			int etabin = Heta->FindFixBin(jet.Eta()) -1;
			int ptbin = HptsP[etabin]->FindFixBin(jet.Pt());
			return(HptsP[etabin]->GetBinContent(ptbin));
		}
		double GetUncM(const IDJet& jet)
		{
			int etabin = Heta->FindFixBin(jet.Eta()) -1;
			int ptbin = HptsM[etabin]->FindFixBin(jet.Pt());
			return(HptsM[etabin]->GetBinContent(ptbin));
		}
};

#endif
