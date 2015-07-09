#include "TTBarBins.h"
#include "TTSemi.h"
#include <sstream>

using namespace std;
using namespace TMath;

BinInfo::BinInfo(string prefix, int bin, vector<double>& bins) : lower(bins[bin]), upper(bins[bin+1])
{
	stringstream ss;
	ss << "_" << bin;
	stringstream ssv;
	ssv << "_" << lower;
	h = new TH1D((prefix+ss.str()).c_str(), (prefix+ssv.str()).c_str(), 300, 0., 600.);
}

void BinInfo::Fill(double m, double weight)
{
	h->Fill(m, weight);
}


size_t TTBarBins::GetBin(double val)
{
	return(lower_bound(sig[0].begin(), sig[0].end(), val)-sig[0].begin()-1);
}

TTBarBins::TTBarBins(string prefix) : sig(14), bkg(14), prefix_(prefix)
{
}

void TTBarBins::Init(vector<double>& bins)
{
	sort(bins.begin(), bins.end());
	min = bins.front();
	max = bins.back();
	TDirectory* dir = gDirectory;
	TDirectory* ndir = dir->mkdir(prefix_.c_str());
	ndir->cd();
	for(size_t j = 0 ; j < sig.size() ; ++j)
	{
		stringstream ss;
		ss << j+4;
		for(size_t b = 0 ; b < bins.size()-1 ; ++b)
		{
			sig[j].push_back(BinInfo("sig_"+ss.str(), b, bins));
			bkg[j].push_back(BinInfo("bkg_"+ss.str(), b, bins));
		}
	}
	dir->cd();
}

void TTBarBins::Fill(bool SIGNAL, int njets, double val, double m, double weight)
{
	if(val <= min || val >= max) return;
	if(njets-3 >= 14) return;
	size_t bin = GetBin(val);
	if(SIGNAL)
	{
		sig[njets-3][bin].Fill(m, weight);
		sig[0][bin].Fill(m, weight);
	}
	else
	{
		bkg[njets-3][bin].Fill(m, weight);
		bkg[0][bin].Fill(m, weight);
	}
}
