#include <TTBarResponse.h>
#include <sstream>

#include <ttbarxsec.h>
#include <TDirectory.h>


TTBarResponse::TTBarResponse(string prefix, ttbar* an) : prefix_(prefix), an_(an), dir(nullptr), plot1d(""), plot2d("")
{
}

TTBarResponse::~TTBarResponse() {}

void TTBarResponse::AddMatrix(string name, const vector<double>& Mbins, const vector<double>& Tbins, string label)
{
	TDirectory* olddir = gDirectory;
	if(dir == nullptr)
	{
		dir = olddir->mkdir(prefix_.c_str());
	}
	dir->cd();
	minm = Mbins.front();
	minmeps = minm + (*(++Mbins.begin()) - *Mbins.begin())*0.5;
	maxm = Mbins.back();
	maxmeps = maxm + (*(++Mbins.rbegin()) - *Mbins.rbegin())*0.5;
	mint = Mbins.front();
	minteps = mint + (*(++Mbins.begin()) - *Mbins.begin())*0.5;
	maxt = Mbins.back();
	maxteps = maxt + (*(++Mbins.rbegin()) - *Mbins.rbegin())*-0.5;

	plot1d.AddHist(name + "_truth", Tbins, label, "Events");
	plot1d.AddHist(name + "_all", Mbins, label, "Events");
	plot1d.AddHist(name + "_bkg", Mbins, label, "Events");
	plot2d.AddHist(name + "_matrix", Tbins, Mbins, "gen " + label, "reco " + label);
	//for(int njet = 0 ; njet < 4 ; ++njet)
	//{
	//	stringstream hname;
	//	hname << name << "_truth_" << njet;
	//	plot1d.AddHist(hname.str(), Tbins, label, "Events");
	//	hname.str("");
	//	hname << name << "_reco_" << njet;
	//	plot1d.AddHist(hname.str(), Mbins, label, "Events");
	//	hname.str("");
	//	hname << name << "_matrix_" << njet;
	//	plot2d.AddHist(hname.str(), Tbins, Mbins, "gen " + label, "reco " + label);
	//}

	olddir->cd();
}

void TTBarResponse::FillTruth(string name, double val, double weight)
{
	if(val <= mint) {val = minteps;}
	else if(val >= maxt) {val = maxteps;}
	plot1d[name + "_truth"]->Fill(val, weight);
}

void TTBarResponse::FillAll(string name, double val, double weight)
{
	if(val <= minm) {val = minmeps;}
	else if(val >= maxm) {val = maxmeps;}
	plot1d[name+"_all"]->Fill(val, weight);
}

void TTBarResponse::FillBKG(string name, double val, double weight)
{
	if(val <= minm) {val = minmeps;}
	else if(val >= maxm) {val = maxmeps;}
	plot1d[name+"_bkg"]->Fill(val, weight);
}

void TTBarResponse::FillTruthReco(string name, double tval, double rval, double weight)
{
	if(tval <= mint) {tval = minteps;}
	else if(tval >= maxt) {tval = maxteps;}
	if(rval <= minm) {rval = minmeps;}
	else if(rval >= maxm) {rval = maxmeps;}
	plot2d[name+"_matrix"]->Fill(tval, rval, weight);
}
