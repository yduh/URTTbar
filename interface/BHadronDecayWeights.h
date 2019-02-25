#ifndef BHADRONDECAYWEIGHTS_H
#define BHADRONDECAYWEIGHTS_H
#include <unordered_map>

#include  "URStreamer.h"

#include <TFile.h>
#include <TH1D.h>
#include <TDirectory.h>

class BHadronDecayWeights
{

private:
unordered_map<int, pair<double, double> > weights;

public:

void Init(double sigma)
{
	//double Lf = 0.103;
	//double Ls = 0.022;
	//weights[5122] = make_pair((Lf+sigma*Ls)/Lf, (1-(Lf+sigma*Ls))/(1-Lf));

	//double Bnf = 0.1033;
	//double Bns = 0.0028;
	//weights[511] = make_pair((Bnf+sigma*Bns)/Bnf, (1-(Bnf+sigma*Bns))/(1-Bnf));

	//double Bsf = 0.096;
	//double Bss = 0.008;
	//weights[531] = make_pair((Bsf+sigma*Bss)/Bsf, (1-(Bsf+sigma*Bss))/(1-Bsf));

	//double Bpf = 0.1099;
	//double Bps = 0.0028;
	//weights[521] = make_pair((Bpf+sigma*Bps)/Bpf, (1-(Bpf+sigma*Bps))/(1-Bpf));



	double Lf = 0.21601;
	double Ls = 0.044;
	weights[5122] = make_pair((Lf+sigma*Ls)/Lf, (1-(Lf+sigma*Ls))/(1-Lf));

	double Bnf = 0.27677;
	double Bns = 0.01;
	weights[511] = make_pair((Bnf+sigma*Bns)/Bnf, (1-(Bnf+sigma*Bns))/(1-Bnf));

	double Bsf = 0.2555;
	double Bss = 0.03;
	weights[531] = make_pair((Bsf+sigma*Bss)/Bsf, (1-(Bsf+sigma*Bss))/(1-Bsf));

	double Bpf = 0.296096;
	double Bps = 0.01;
	weights[521] = make_pair((Bpf+sigma*Bps)/Bpf, (1-(Bpf+sigma*Bps))/(1-Bpf));
}


double Weight(const Pl& bjet)
{
	int pdgid = abs(bjet.isoR3());
	bool lepdecay = (pdgid >= 1000000);
	if(lepdecay) pdgid -= 1000000;
	auto itval = weights.find(pdgid);

	if(itval == weights.end()) {return 1.;}
	return (lepdecay ? itval->second.first : itval->second.second);
}

};

class BFragWeights
{
	private:
		TH1D* hweight = nullptr;
		double m_sigma;
	public:
		void Init(string weightfile, double sigma)
		{
			m_sigma = sigma;
			TDirectory* dir = gDirectory;
			TFile* tf = TFile::Open(weightfile.c_str());
			if(sigma < 0)
			{
				hweight = dynamic_cast<TH1D*>(tf->Get("bfrag_down"));
			}
			else
			{
				hweight = dynamic_cast<TH1D*>(tf->Get("bfrag_up"));
			}
			dir->cd();
		}
		double Weight(const Pl& bjet)
		{
			double err = hweight->Interpolate(bjet.isoR4());	
			return 1. + abs(m_sigma)*(err-1.);
		}

};

#endif
