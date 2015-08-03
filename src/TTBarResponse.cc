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

	plot1d.AddHist(name + "_truth", Tbins, label, "Events");
	plot1d.AddHist(name + "_reco", Mbins, label, "Events");
	plot2d.AddHist(name + "_matrix", Tbins, Mbins, "gen " + label, "reco " + label);
	for(int njet = 0 ; njet < 4 ; ++njet)
	{
		stringstream hname;
		hname << name << "_truth_" << njet;
		plot1d.AddHist(hname.str(), Tbins, label, "Events");
		hname.str("");
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
	int njet = Min(an_->genaddjets.size(), size_t(3));
	stringstream hname;
	hname << name << "_truth_" << njet;
	TH1D* hist = plot1d[hname.str()];
	double xmin = hist->GetXaxis()->GetXmin();
	double xmax = hist->GetXaxis()->GetXmax();
	if(val <= xmin) {val = xmin+0.00001;}
	if(val >= xmax) {val = xmax-0.00001;}
	hist->Fill(val, weight);
	plot1d[name + "_truth"]->Fill(val, weight);
}

void TTBarResponse::FillReco(string name, double val, double weight)
{
	int njet = Min(an_->reducedjets.size() - 4, size_t(3));
	stringstream hname;
	hname << name << "_reco_" << njet;
	TH1D* hist = plot1d[hname.str()];
	double xmin = hist->GetXaxis()->GetXmin();
	double xmax = hist->GetXaxis()->GetXmax();
	if(val <= xmin) {val = xmin+0.00001;}
	if(val >= xmax) {val = xmax-0.00001;}
	hist->Fill(val, weight);
	plot1d[name+"_reco"]->Fill(val, weight);
}

void TTBarResponse::FillTruthReco(string name, double tval, double rval, double weight)
{
	int njet = Min(an_->reducedjets.size() - 4, size_t(3));
	stringstream hname;
	hname << name << "_matrix_" << njet;
	TH2D* hmatrix = plot2d[hname.str()];
	double xmin = hmatrix->GetXaxis()->GetXmin();
	double xmax = hmatrix->GetXaxis()->GetXmax();
	if(tval <= xmin) {tval = xmin+0.00001;}
	else if(tval >= xmax) {tval = xmax-0.00001;}
	double ymin = hmatrix->GetYaxis()->GetXmin();
	double ymax = hmatrix->GetYaxis()->GetXmax();
	if(rval <= ymin) {rval = ymin+0.00001;}
	else if(rval >= ymax) {rval = ymax-0.00001;}
	hmatrix->Fill(tval, rval, weight);
	plot2d[name+"_matrix"]->Fill(tval, rval, weight);
}
