#include "Analyse.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <boost/algorithm/string.hpp>

#include <TH1D.h>
#include <TNamed.h>
#include <TApplication.h>

using namespace std;

class MyAnalysis : public Analyse
{
	private:
	public:
		MyAnalysis() {}
		virtual ~MyAnalysis() {}
		virtual Int_t AnalyseEvent() {return(1);}
};

int main(int argc, char** argv)
{
	//TApplication* app = new TApplication("LUMIxrootd", &argc, argv);
	string namebuf;
	string prefix;
	//TFile* test1 = TFile::Open("root://xrootd.unl.edu//store/user/aperiean/MC/8TEV/RootMakerV1.8.0/try_2/VBF_ZZTo4mu_SignalContinInterf_SM_M-125p6_13TeV-phantom123-pythia8/AC1B_1_1_iyZ.root");
	MyAnalysis ana;
	while(true)
	{
		cin >> namebuf;
		if(namebuf == "END")
		{
			break;
		}
		cout << namebuf << endl;
		//TFile* test2 = TFile::Open("root://xrootd.unl.edu//store/user/aperiean/MC/8TEV/RootMakerV1.8.0/try_2/VBF_ZZTo4mu_SignalContinInterf_SM_M-125p6_13TeV-phantom123-pythia8/AC1B_1_1_iyZ.root");
		ana.AddLumiFile("root://xrootd.unl.edu/"+namebuf);
		if(prefix.size() == 0)
		{
			UInt_t slashpos = namebuf.find_last_of("/");
			prefix = "root://xrootd.unl.edu/"+namebuf.substr(0,slashpos+1);
			cout << prefix << endl;
		}
	}
	ana.ResetLumiValues();
	cout << "Reading Lumi from text file" << endl;
	fstream file("lumi.cvs");
	char line[1000];
	Float_t lumirecorded;
	Float_t avgpu;
	UInt_t run;
	UInt_t block;
	TH1D* muhist = new TH1D("mudist", "mudist", 200, 0., 100.);
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
				//muhist->Fill(atof(strs[7].c_str())*766./693.);
				muhist->Fill(avgpu);
			}
		}
		file.close();
	}
	else
	{
		cout << "WARNING: lumi.cvs could not be opened! No lumi information stored. This is no problem with MC :-)." << endl;
	}
	ana.PrintLumiOfRuns();
	ana.WriteLumiFile("LUMI_INFO.root");
	TFile* lumfile = new TFile("LUMI_INFO.root", "update");
	TNamed* prefixname = new TNamed("fileprefix", prefix.c_str());
	prefixname->Write();
	muhist->Write();
 	lumfile->Write();
	lumfile->Close();
}

