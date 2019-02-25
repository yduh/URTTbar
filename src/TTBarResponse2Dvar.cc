#include <TTBarResponse2Dvar.h>
#include <sstream>
#include <iostream>

#include <TDirectory.h>

TTBarResponse2Dvar::TTBarResponse2Dvar(string prefix) : prefix_(prefix), dir(nullptr), plot1d(""), plot2d("")
{
}

TTBarResponse2Dvar::~TTBarResponse2Dvar() {}

void TTBarResponse2Dvar::AddMatrix(string name, const vector<double>& xbins, const vector<vector<double> >& ybins, bool ofx, bool ofy)
{
	TDirectory* olddir = gDirectory;
	bool indir = olddir->cd(prefix_.c_str());
	if(!indir)
	{
		dir = olddir->mkdir(prefix_.c_str());
		dir->cd();
	}
	else
	{
		dir = gDirectory;
	}

	ofx_[name] = ofx;
	ofy_[name] = ofy;

	int nb = 0;
	plot1d.AddHist(name + "_X_0", xbins, "x binning", "empty");
	for(size_t x = 0 ; x < xbins.size()-1 ; ++x)
	{
		Bin xb(xbins[x], xbins[x+1]);  
		const vector<double>& yvec = ybins[x];
		stringstream hn;
		hn << name << "_Y_" << x;
		plot1d.AddHist(hn.str(), yvec, "y binning", "empty");
		for(size_t y = 0 ; y < yvec.size()-1 ; ++y)
		{
			Bin yb(yvec[y], yvec[y+1]);
			nb++;
			binning[name][xb][yb] = nb; 
		}	
	}	

	plot1d.AddHist(name + "_truth", nb, 0., nb, "gen", "Events");
	plot1d.AddHist(name + "_bkg", nb, 0, nb, "reco", "Events");
	plot1d.AddHist(name + "_all", nb, 0, nb, "reco", "Events");
	plot2d.AddHist(name + "_matrix", nb, 0., nb, nb, 0, nb, "gen", "reco");
	plot2d.AddHist(name + "_res", nb, 0., nb, 600, -3.*abs(ybins[0][0]-ybins[0][1]), 3.*abs(ybins[0][0]-ybins[0][1]), "y binning", "res");

	olddir->cd();
}


int TTBarResponse2Dvar::GetBin(string name, double valx, double valy)
{
	map< Bin, map< Bin, double> >& bins = binning[name];

	if(bins.begin()->first.min() > valx)
	{
		if(ofx_[name] == false)
		{
			return 0;
		}
		else
		{
			valx = 0.5*(bins.begin()->first.min() + bins.begin()->first.max());
		}
	}
	else if((--bins.end())->first.max() < valx)
	{
		if(ofx_[name] == false)
		{
			return 0;
		}
		else
		{
			valx = 0.5*((--bins.end())->first.min() + (--bins.end())->first.max());
		}
	}

	map< Bin, double>& binsy = bins[valx];
	
	if(binsy.begin()->first.min() > valy)
	{
		if(ofy_[name] == false)
		{
			return 0;
		}
		else
		{
			valy = 0.5*(binsy.begin()->first.min() + binsy.begin()->first.max());
		}
	}
	else if((--binsy.end())->first.max() < valy)
	{
		if(ofy_[name] == false)
		{
			return 0;
		}
		else
		{
			valy = 0.5*((--binsy.end())->first.min() + (--binsy.end())->first.max());
		}
	}

	return binsy[valy];
}

void TTBarResponse2Dvar::FillTruth(string name, double valx, double valy, double weight)
{
	int bin = GetBin(name, valx, valy);
	plot1d[name + "_truth"]->Fill(bin-0.5, weight);
}

void TTBarResponse2Dvar::FillBKG(string name, double valx, double valy, double weight)
{
	int bin = GetBin(name, valx, valy);
	plot1d[name + "_bkg"]->Fill(bin-0.5, weight);
}

void TTBarResponse2Dvar::FillAll(string name, double valx, double valy, double weight)
{
	int bin = GetBin(name, valx, valy);
	plot1d[name + "_all"]->Fill(bin-0.5, weight);
}

void TTBarResponse2Dvar::FillTruthReco(string name, double tvalx, double tvaly, double rvalx, double rvaly, double weight)
{
	int tbin = GetBin(name, tvalx, tvaly);
	int rbin = GetBin(name, rvalx, rvaly);
	plot2d[name+"_matrix"]->Fill(tbin-0.5, rbin-0.5, weight);
	plot2d[name+"_res"]->Fill(rbin-0.5, rvaly-tvaly, weight);
//	plot2d[name+"_res"]->Fill(rbin-0.5, rvalx-tvalx, weight);
}


