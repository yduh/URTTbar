#include "DYTPMu.h"
#include <iostream>
#include <vector>
#include <sstream>

using namespace std;

int main(int argc, char** argv)
{
	UInt_t jobnum = atoi(argv[1])+1;
	UInt_t jobcount = atoi(argv[2]);
	string lumifile = string(argv[3]);
	string outfilename = string(argv[4]);
	stringstream ssoutfilename;
	ssoutfilename << outfilename << "_" << jobnum << ".root";

	DYTPMu ana(ssoutfilename.str());
	ana.SetPrintInfo(10000);
    ana.EnableDuplicateCheck();
	ana.AddLumiFile(lumifile);
	ana.Batch_Prepare(jobnum, jobcount);

	vector<string> isomutrigger;
    isomutrigger.push_back("HLT_IsoMu24_IterTrk02_v.*");
	ana.AddTriggerSelection("ISOSMT", isomutrigger);
	ana.GetTriggerSelection("ISOSMT")->PrintInfo();

	vector<string> isoeltrigger;
    isomutrigger.push_back("HLT_Ele27_WP85_Gsf_v.*");
	ana.AddTriggerSelection("ISOSET", isoeltrigger);
	ana.GetTriggerSelection("ISOSET")->PrintInfo();

	vector<string> dmtrigger;
    dmtrigger.push_back("HLT_Mu17_Mu8_DZ_v.*");
	ana.AddTriggerSelection("DMT", dmtrigger);
	ana.GetTriggerSelection("DMT")->PrintInfo();

//	vector<string> detrigger;
//	detrigger.push_back("HLT_Ele17_CaloIdL_CaloIsoVL_Ele8_CaloIdL_CaloIsoVL_v.*");
//	detrigger.push_back("HLT_Ele17_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_Ele8_CaloIdT_CaloIsoVL_TrkIdVL_TrkIsoVL_v.*");
//	ana.AddTriggerSelection("DET", detrigger);
//	ana.GetTriggerSelection("DET")->PrintInfo();
	
	ana.Loop();
}

