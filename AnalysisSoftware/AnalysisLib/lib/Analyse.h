#ifndef ACANALYSE
#define ACANALYSE
#include <BASEIO.h>
#include "Luminosity.h"
#include "Run.h"
#include "TriggerSelection.h"

#include <iostream>
#include <map>
#include <vector>
#include <string>

class TFile;
class TH1D;

using namespace std;
using namespace TMath;

class Analyse : public BASEIO::BaseIO
{
	friend class TriggerSelection;
	friend class TriggerObject;
	friend class Muon;
	friend class Electron;
	friend class Jet;
	friend class Photon;
	private:
	string fileprefix;
	vector<string> filenames;
	vector<Long64_t> filelimits;
	Long64_t currentmin, currentmax;
	TFile* currentfile;
	Int_t printinfo;
	Long64_t processed;
	Long64_t analyzed;
	void GetEvent(Long64_t num);
	Int_t AddFile(string filename);

	//dupcheck
	bool duplicatecheck;
	map< UInt_t, map< UInt_t, map< UInt_t, UInt_t> > > eventlist;

	//Skimming
	TTree* skimtree;
	map< UInt_t, map< UInt_t, UInt_t > > selected;
	string skimfilename;
	TFile* skimfile;

	//Lumi calculation
	map< UInt_t, map< UInt_t, Luminosity > > lumilist;
	map< UInt_t, RunInfo> runlist;
	map<string, TriggerSelection*> triggerselections;
	bool lumicalculation;
	bool IsInRange(UInt_t Run, UInt_t LumiBlock);
	UInt_t minRun;
	UInt_t minLumi;
	UInt_t maxRun;
	UInt_t maxLumi;

	//JSON filter
	bool jsonfilter;
	map< UInt_t, map< UInt_t, bool > > jsonlist;

	//Process
	Int_t batch_myid; 
	Int_t batch_numjobs; 
	map< UInt_t, map< UInt_t, bool > > batchselection;
	bool IsBatchSelected(UInt_t run, UInt_t lumiblock);

	bool ismc;
	TH1D* mc_mu_in;
	TH1D* mc_pu_in;
	TH1D* pu_reweight;
	Long64_t totalnumberofevents;
	public:
	Analyse(int argc = 0, char** argv = 0, bool batchmode = false);
	virtual ~Analyse();
	void Batch_Prepare(UInt_t jobnum, UInt_t totaljobnum); //batch jobnum 1 2 3 ....
	Int_t Batch_MyId() const {return(batch_myid);} 
	Int_t Batch_NumJobs() const {return(batch_numjobs);} 
	Long64_t GetNumAddedEvents() const {return(filelimits[filelimits.size()-1]);};
	string GetCurrentFileName() const {if(currentfile != 0) {return(currentfile->GetName());} else {return("NO FILE OPENED");}}

	TH1D* SetupPileupWeighting(string filename, string histname, bool mu = true);
	double GetPileupWeight();

	//virtual dummies:
	//executed for each event in the loop
	virtual Int_t AnalyseEvent(){return(1);};
	//executed once at the beginning of a the loop
	virtual void BeginLoop(){};
	virtual void FileChanged(){};
	//executed once at the end of a loop
	virtual void EndLoop(){};

	//use this to start your analysis
	Long64_t Loop(Long64_t start = 0, Long64_t end = -1);
	//Number of Processed Events
	Long64_t Processed() const {return(processed);}
	Long64_t Analyzed() const {return(analyzed);}

	//setting output: -1 no output, the p-th events are printed
	void SetPrintInfo(Int_t p = -1) {printinfo = p;}

	//tool to check for duplicated events
	void EnableDuplicateCheck(bool switchval = true) {duplicatecheck = switchval;}
	UInt_t CheckDuplicate();

	//Skiming
	//Int_t PrepareSkimming(string filename);
	//Int_t SkimEvent();

	//trigger information
	TriggerSelection* AddTriggerSelection(string id, vector<string> triggernames, bool useprescaled = false);
	TriggerSelection* GetTriggerSelection(string id);
	//simple trigger information use Add/GetTriggerSelection instead.
	bool GetHLTriggerResult(UInt_t index);
	Int_t GetHLTriggerIndex(string triggername);
	Int_t GetHLTPrescale(UInt_t triggerindex);
	Int_t GetNumHLTriggers();
	string GetHLTNames(UInt_t index);


	//Lumi calculation
	int AddLumiFile(string filename, bool updatefiles = false);
	Int_t IsLumiAvailable();
	Double_t GetInstLumi();
	Double_t GetAvgPU(); 
	//	Double_t GetLumi(Int_t format = 0);
	//	void PrintPrescaleInfo(string triggername);
	//	void PrintPrescaleInfoB(string triggername);
	//	void PrintLumiOfRuns();
	//	void PrintLumiOfLumiSectionsInRun(UInt_t runnumber);
	//	//very special don't use!
	Int_t SetLumi(UInt_t run, UInt_t block, Float_t lumival, Float_t avgpu = -1);
	void ResetLumiValues();
	void WriteLumiFile(string filename, string mode = "recreate");
	//	//Use JSON filter
	bool LoadJSON(string filename);

	//Event data
	UInt_t Run() {return(GetIOEventInfo(0).RunNumber());}
	UInt_t LumiBlock() {return(GetIOEventInfo(0).LumiSectionNumber());}
	UInt_t Number() {return(GetIOEventInfo(0).EventNumber());}
	Float_t AK5PFRho() {return(GetIOEventInfo(0).AK5PFRho());}
	bool IsMC() const {return(ismc);}
	bool IsData() const {return(!ismc);}
	Long64_t TotalNumberOfEvents() const {return(totalnumberofevents);}

};

extern Analyse* GLAN;

Long64_t mem_usage();
#endif
