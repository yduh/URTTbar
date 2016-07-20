#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <cmath>
#include <algorithm>

#include <TH1D.h>
#include <TFile.h>
#include <TGraph.h>
#include <TDirectory.h>
#include <TRandom3.h>

#include "IDJet.h"
#include "ttbarxsec.h"
#include "JetScaler.h"


void JetScaler::InitMCrescale(ttbar* an, const string rescalefilename)
{
	AN = an;
	string mcname = "";
	if(an->HERWIGPP){mcname = "Hpp";}
	if(an->SCALEUP){mcname = "psup";}
	if(an->SCALEDOWN){mcname = "psdown";}
	if(mcname != "")
	{
		TDirectory* dir = gDirectory;
		tf = TFile::Open(rescalefilename.c_str());
		hlE = dynamic_cast<TGraph*>(tf->Get((mcname+"_l_E").c_str()));
		hlB = dynamic_cast<TGraph*>(tf->Get((mcname+"_l_B").c_str()));
		hbE = dynamic_cast<TGraph*>(tf->Get((mcname+"_b_E").c_str()));
		hbB = dynamic_cast<TGraph*>(tf->Get((mcname+"_b_B").c_str()));
		dir->cd();
	}

}

void JetScaler::Init(const string& filename, const string& type)
{
	TDirectory* dir = gDirectory;
	//gROOT->cd();
	fstream fs(filename.c_str(), fstream::in);
	string line;
	int selected = -1;
	vector<double> etabins;
	while(!fs.eof())
	{
		getline(fs, line);
		if(line.size() == 0) continue;
		if(selected != -1) selected++;
		if(line[0] == '[')
		{
			vector<string> test = string_split(line, {"[", "]"});
			if(test.size() == 1 && test[0] == type)
			{
				selected = 0;
			}
			else
			{
				selected = -1;
			}
		}
		if(selected > 1)
		{
			vector<string> vals = string_split(line);
			if(selected == 2)
			{
				etabins.push_back(stringtotype<double>(vals[0]));
			}		
			etabins.push_back(stringtotype<double>(vals[1]));
			vector<double> pts;
			vector<double> errm;
			vector<double> errp;
			for(size_t p = 3; p < vals.size() ; p+=3)
			{
				if(vals[p].size() == 0) break;
				pts.push_back(stringtotype<double>(vals[p]));
				errm.push_back(stringtotype<double>(vals[p+1]));
				errp.push_back(stringtotype<double>(vals[p+2]));
				//cout << p << " " << pts.back() << " " << errm.back() << " " << errp.back() << endl;
			}
			stringstream name;
			name << "jeterror_" << type << "_" << selected-2; 
			HptsM.push_back(new TH1D((name.str()+"M").c_str(), (name.str()+"M").c_str(), pts.size()-1, pts.data()));
			HptsP.push_back(new TH1D((name.str()+"P").c_str(), (name.str()+"P").c_str(), pts.size()-1, pts.data()));
			for(size_t b = 0 ; b < pts.size()-1 ; ++b)
			{
				HptsM.back()->SetBinContent(b+1, errm[b]);
				HptsP.back()->SetBinContent(b+1, errp[b]);
			}
		}
	}
	Heta = new TH1D(("etabins_"+type).c_str(), ("etabins_"+type).c_str(), etabins.size()-1, etabins.data());
	if(etabins.size() == 0)
	{
		cerr << "ERROR - Jetscaler.Init: Could not find " << type << " in file " << filename << endl;
	}
	fs.close();
	dir->cd();
}

void JetScaler::InitResolution(const string& resolutionfile, const string& sffile)
{
	fstream fs(resolutionfile.c_str(), fstream::in);
	string line;
	while(!fs.eof())
	{
		getline(fs, line);
		if(line.size() == 0 || line[0] == '{') continue;
		vector<string> vals = string_split(line, {" ", "\t"});
		//for(string s:vals) cout << "|" << s << "|";
		//cout << endl;
		//cout << vals.size() << " " << vals[0] << " " << vals[1] << endl;
		if(vals.size() != 11) continue;
		
		Bin eta(stringtotype<double>(vals[0]), stringtotype<double>(vals[1]));
		Bin rho(stringtotype<double>(vals[2]), stringtotype<double>(vals[3]));
		//cout << stringtotype<double>(vals[0]) << " " << stringtotype<double>(vals[1]) << " " <<stringtotype<double>(vals[2]) << " " <<stringtotype<double>(vals[3]) << endl;
		resinfo[eta][rho] = {stringtotype<double>(vals[7]), stringtotype<double>(vals[8]), stringtotype<double>(vals[9]), stringtotype<double>(vals[10])};
	}
	fs.close();
	//cout << "finished "<< resinfo.size() << endl;
	fstream fsc(sffile.c_str(), fstream::in);
	while(!fsc.eof())
	{
		getline(fsc, line);
		if(line.size() == 0 || line[0] == '{') continue;
		vector<string> vals = string_split(line, {" ", "\t"});
		if(vals.size() != 6) continue;
		
		Bin eta(stringtotype<double>(vals[0]), stringtotype<double>(vals[1]));
		ressf[eta] = {stringtotype<double>(vals[3]), stringtotype<double>(vals[4]), stringtotype<double>(vals[5])};
	}
	fsc.close();
}

double JetScaler::GetScale(const IDJet& jet, double rho, double sigmascale, double sigmares)
{
	double sf = 1.;
	if(AN->isDA == 0)
	{
		//cout << jet.Eta() << " " << rho << endl;
		//cout << (resinfo.find(jet.Eta()) - resinfo.begin()) << endl;
		const vector<double>& par = resinfo[jet.Eta()][rho];
		double x = jet.Pt();
		double resolution = sqrt(par[0]*abs(par[0])/(x*x)+par[1]*par[1]*pow(x,par[3])+par[2]*par[2]); 	
		//cout << jet.Eta() << " " << rho << " - " << resolution << ": " << par[0] << " " << par[1]<< " " << par[2]<< " " << par[3] << endl;
		const vector<double>& sfs = ressf[jet.Eta()];
		double s = sfs[0];
		//cout << sfs[0] << " " << sfs[1] << " " << sfs[2] << endl;
		if(sigmares <= 0) {s = sfs[0] + sigmares*(sfs[0]-sfs[1]);}
		if(sigmares > 0) {s = sfs[0] + sigmares*(sfs[2]-sfs[0]);}
		sf = gRandom->Gaus(1., resolution*sqrt(s*s-1.));
	}

	//MC specific correction 
	//if(hlB != nullptr)
	if(false)
	{
		double mccorr = 1.;
		if(find_if(AN->genbpartons.begin(), AN->genbpartons.end(), [&](GenObject* bp){return jet.DeltaR(*bp) < 0.3;}) != AN->genbpartons.end())
		{
			if(Abs(jet.Eta()) < 1.5)
			{
				mccorr = hbB->Eval(jet.Pt());	
			}
			else
			{
				mccorr = hbE->Eval(jet.Pt());	
			}
		}
		else
		{
			if(Abs(jet.Eta()) < 1.5)
			{
				mccorr = hlB->Eval(jet.Pt());	
			}
			else
			{
				mccorr = hlE->Eval(jet.Pt());	
			}
		}
		sf += mccorr;
	}


	int etabin = Heta->FindFixBin(jet.Eta()) -1;
	int ptbin = HptsP[etabin]->FindFixBin(jet.Pt());
	if(sigmascale >= 0)
	{
		return(sf + sigmascale*HptsP[etabin]->GetBinContent(ptbin));
	}
	else
	{
		return(sf + sigmascale*HptsM[etabin]->GetBinContent(ptbin));
	}
}



