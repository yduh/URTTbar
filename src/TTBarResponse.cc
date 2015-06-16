#include <TTBarResponse.h>
#include <sstream>

#include <ttbarxsec.h>
#include <TDirectory.h>


TTBarResponse::TTBarResponse(string prefix, ttbar* an) : prefix_(prefix), an_(an), dir(0), plot1d(""), plot2d("")
{
}

TTBarResponse::~TTBarResponse() {}

void TTBarResponse::AddMatrix(string name, const vector<double>& Mbins, const vector<double>& Tbins, string label)
{
	TDirectory* olddir = gDirectory;
	if(dir == 0)
	{
		dir = olddir->mkdir(prefix_.c_str());
	}
	dir->cd();

	plot1d.AddHist(name + "_truth", Tbins, "Truth " + label, "Events");
	plot1d.AddHist(name + "_reco", Mbins, label, "Events");
	plot2d.AddHist(name + "_matrix", Tbins, Mbins, "gen " + label, "reco " + label);
	for(int njet = 0 ; njet < 4 ; ++njet)
	{
		stringstream hname;
		hname << name << "_reco_" << njet;
		plot1d.AddHist(hname.str(), Mbins, label, "Events");
		hname.str("");
		hname << name << "_matrix_" << njet;
		plot2d.AddHist(hname.str(), Tbins, Mbins, "gen " + label, "reco " + label);
	}

	olddir->cd();
}

void TTBarResponse::FillTruth(string name, double val, double weight)
{
	plot1d[name + "_truth"]->Fill(val, weight);
}

void TTBarResponse::FillReco(string name, double val, double weight)
{
	int njet = Min(an_->reducedjets.size() - 4, size_t(3));
	stringstream hname;
	hname << name << "_reco_" << njet;
	plot1d[hname.str()]->Fill(val, weight);
	plot1d[name+"_reco"]->Fill(val, weight);
}

void TTBarResponse::FillTruthReco(string name, double tval, double rval, double weight)
{
	int njet = Min(an_->reducedjets.size() - 4, size_t(3));
	stringstream hname;
	hname << name << "_matrix_" << njet;
	plot2d[hname.str()]->Fill(tval, rval, weight);
	plot2d[name+"_matrix"]->Fill(tval, rval, weight);
}
