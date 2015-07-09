#include "Analyse.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <boost/algorithm/string.hpp>

#include <TH1D.h>
#include <TROOT.h>

#include <sys/types.h>
#include <dirent.h>

using namespace std;

class MyAnalysis : public Analyse
{
	private:
	public:
		MyAnalysis() 
		{
		}
		virtual ~MyAnalysis() {}
		virtual Int_t AnalyseEvent() 
		{
			return(1);
		}
};

int main(int argc, char** argv)
{
	if(argc == 1)
	{
		cerr << "You have to pass a local directory as argument." << endl;
		exit(-1);
	}
	string localdir(argv[1]);

	DIR* dirhandle = opendir(localdir.c_str());

	if(!dirhandle)
	{
		cerr << "Directory " << localdir << " cannot be opened." << endl;
		exit(-1);
	}

	list<string> filenames;
	struct dirent* dirinfo;
	while((dirinfo = readdir(dirhandle)) != 0)
	{
		string filename(dirinfo->d_name);
		if(filename.find(".root") != string::npos && filename.find("LUMI_INFO") == string::npos)
		{
			filenames.push_back(filename);
			cout << filename << endl;
		}
	}

	MyAnalysis ana;
	TH1D* mc_mu = 0;
	TH1D* mc_pu = 0;
	TH1D* mc_pup = 0;
	TH1D* mc_pum = 0;
	for(list<string>::iterator it = filenames.begin() ; it != filenames.end() ; ++it)
	{
		string fullfilename(localdir + "/" + *it);
		ana.AddLumiFile(fullfilename, true);
		TFile* eventsfile = TFile::Open(fullfilename.c_str(), "read");
		TH1D* mu = static_cast<TH1D*>(eventsfile->Get("makeroottree/mu"));
		TH1D* pu = static_cast<TH1D*>(eventsfile->Get("makeroottree/pu"));
		TH1D* pum = static_cast<TH1D*>(eventsfile->Get("makeroottree/pum"));
		TH1D* pup = static_cast<TH1D*>(eventsfile->Get("makeroottree/pup"));
		if(mc_mu == 0)
		{
			gROOT->cd();
			mc_mu = new TH1D(*mu);
			mc_pu = new TH1D(*pu);
			mc_pup = new TH1D(*pup);
			mc_pum = new TH1D(*pum);
		}
		else
		{
			mc_mu->Add(mu);
			mc_pu->Add(pu);
			mc_pup->Add(pup);
			mc_pum->Add(pum);
		}
		eventsfile->Close();
	}

	ana.ResetLumiValues();

	TH1D* da_mu = new TH1D(*mc_mu);
	da_mu->Reset();
	if(mc_mu->GetEntries() == 0)
	{
		fstream file("lumi.cvs");
		char line[1000];
		Float_t lumirecorded;
		Float_t avgpu;
		UInt_t run;
		UInt_t block;
		if(file.is_open())
		{
			file.getline(line, 1000);
			while(!file.eof())
			{
				file.getline(line,1000);
				vector<std::string> strs;
				boost::split(strs, line, boost::is_any_of(","));
				if(strs.size() != 8) continue;
				run = atoi(strs[0].c_str());
				vector<std::string> blocks;
				boost::split(blocks, strs[1], boost::is_any_of(":"));
				block = atoi(blocks[0].c_str());
				lumirecorded = atof(strs[6].c_str());
				avgpu = atof(strs[7].c_str());
				if(lumirecorded == 0.)
				{
					cout << "WARNING Lumi value is 0. Run: " << run << ", Block: " << block << endl;
					continue;
				}
				if(ana.SetLumi(run, block, lumirecorded/1000000., avgpu) == 1)
				{
					da_mu->Fill(avgpu);
				}
			}
			file.close();
		}
		else
		{
			cout << "WARNING: lumi.cvs could not be opened! No lumi information stored." << endl;
		}
	}
	//ana.PrintLumiOfRuns();
	ana.WriteLumiFile(localdir + "/LUMI_INFO.root", "recreate");
	TFile* lumfile = new TFile((localdir + "/LUMI_INFO.root").c_str(), "update");
	mc_mu->Write("mc_mu");
	mc_pu->Write("mc_pu");
	mc_pup->Write("mc_pup");
	mc_pum->Write("mc_pum");
	da_mu->Write("da_mu");
 	lumfile->Write();
	lumfile->Close();
}

