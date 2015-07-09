#include "Analyse.h"

#include <regex>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <numeric>

#include <TFile.h>
#include <TMath.h>
#include <TH1D.h>
#include <TStopwatch.h>
#include <TROOT.h>

#include <sys/types.h>
#include <dirent.h>

//Analyse
Analyse* GLAN = 0;
Analyse::Analyse(int argc, char** argv, bool batchmode) : 
	BASEIO::BaseIO("UREVENT", false),
	currentmin(-1),
	currentmax(-1),
	currentfile(0),
	printinfo(1),
	processed(0),
	analyzed(0),
	duplicatecheck(false),
	skimtree(0),
	skimfilename("None"),
	skimfile(0),
	lumicalculation(false),
	minRun(100000000),
	minLumi(100000000),
	maxRun(0),
	maxLumi(0),
	jsonfilter(false),
	batch_myid(-1),
	batch_numjobs(-1),
	ismc(false),
	pu_reweight(0),
	totalnumberofevents(0)
{
	GLAN = this;
}

Analyse::~Analyse()
{
	if(skimtree != 0)
	{
		TFile* file = skimtree->GetCurrentFile();
		file->Write();
		file->Close();
		Long64_t maxsize = TTree::GetMaxTreeSize();
		TTree::SetMaxTreeSize(maxsize + 1000000000ll);
		WriteLumiFile(skimfilename, "update");		
		TTree::SetMaxTreeSize(maxsize);
	}
	if(currentfile != 0)
	{
		delete currentfile;
	}
}

Int_t Analyse::AddFile(string filename)
{
	if(find(filenames.begin(), filenames.end(), filename) != filenames.end())
	{
		return(0);
	}
	TFile* file = 0;
	file = TFile::Open((fileprefix + filename).c_str());
	if(file == 0 || file->IsZombie())
	{
		cerr << "ERROR AddFile: " << filename << " is not a valid file." << endl;
		return(-1);
	}
	SetFile(file);
	filenames.push_back(filename);
	if(filelimits.size() == 0)
	{
		filelimits.push_back(GetEntries());
	}
	else
	{
		filelimits.push_back(GetEntries() + filelimits.back());
	}
	delete file;
	return(1);
}

void Analyse::GetEvent(Long64_t num)
{
	if(num < currentmin || num >= currentmax)
	{
		if(currentfile != 0)
		{ 
			delete currentfile;
		}
		if(num < filelimits[0])
		{
			currentfile = TFile::Open((fileprefix + filenames[0]).c_str());
			SetFile(currentfile);
			FileChanged();
			currentmin = 0;
			currentmax = filelimits[0];
		}
		for(UInt_t i = 1 ; i < filelimits.size() ; i++)
		{
			if(num < filelimits[i] && num >= filelimits[i-1])
			{
				currentfile = TFile::Open((fileprefix + filenames[i]).c_str());
				SetFile(currentfile);
				FileChanged();
				currentmax = filelimits[i];
				currentmin = filelimits[i-1];
				break;
			}
		}
	}
	GetEntry(num - currentmin);
}

bool Analyse::GetHLTriggerResult(UInt_t index)
{
	if((GetIOEventInfo(0).TriggerHLT(index/8) & 1<<(index % 8)) > 0)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

bool Analyse::IsBatchSelected(UInt_t run, UInt_t lumiblock)
{

	if(Batch_MyId() <= 0) return(true);
	if(batchselection.find(run) != batchselection.end() && batchselection.find(run)->second.find(lumiblock) != batchselection.find(run)->second.end())
	{
		return(true);
	}
	return(false);
}

void Analyse::Batch_Prepare(UInt_t jobnum, UInt_t totaljobnum)
{
	batch_myid = jobnum;
	batch_numjobs = totaljobnum;
	map<string, vector<pair<UInt_t, UInt_t> > > filelumimap;
	UInt_t numlumisections = 0;
	for(map< UInt_t, map< UInt_t, Luminosity > >::iterator a = lumilist.begin() ; a != lumilist.end() ; ++a)
	{
		for(map< UInt_t, Luminosity >::iterator b = a->second.begin() ; b != a->second.end() ; ++b)
		{
			if((jsonfilter && jsonlist[a->first][b->first]) || !jsonfilter)
			{
				numlumisections++;
				const vector<string>& filesofls = b->second.GetFiles(); 
				filelumimap[filesofls[0]].push_back(pair<UInt_t, UInt_t>(a->first, b->first));
			}
		}
	}
	cerr << "JOB " << Batch_MyId() << ": " << "Total number of lumisections: " << numlumisections << endl;

	UInt_t stepval = numlumisections/(Batch_NumJobs());
	UInt_t i = 0;
	for(map<string, vector<pair<UInt_t, UInt_t> > >::iterator a = filelumimap.begin() ; a != filelumimap.end() ; ++a)
	{
		for(vector<pair<UInt_t, UInt_t> >::iterator b = a->second.begin() ; b != a->second.end() ; ++b)
		{
			if((jsonfilter && jsonlist[b->first][b->second]) || !jsonfilter)
			{
				i++;
				if(i > stepval*(Batch_MyId()-1) && (i <= stepval*(Batch_MyId()) || Batch_MyId() == Batch_NumJobs()))
				{
					batchselection[b->first][b->second] = true;
					//cout << i << " Batch_Prepare: " << a->first << " " << b->first << " " << b->second << endl;
					const vector<string>& files = lumilist[b->first][b->second].GetFiles();
					for(const string& file : files)
					{
						AddFile(file);
					}
				}
			}
		}
	}
	//GetEvent(0);
}

UInt_t Analyse::CheckDuplicate() {return(eventlist[Run()][LumiBlock()][Number()]);}

Long64_t Analyse::Loop(Long64_t start, Long64_t end)
{
	if(end == -1 || end > GetNumAddedEvents())
	{
		end = GetNumAddedEvents();
	}

	processed = 0;
	analyzed = 0;
	BeginLoop();
	if(Batch_MyId() != -1) cerr << "JOB " << Batch_MyId() << ": ";
	cerr << GetNumAddedEvents() << " Events are added." << endl;
	//Looping
	if(Batch_MyId() != -1) cerr << "JOB " << Batch_MyId() << ": ";
	cerr << "Events " << start << " - " << end << " will be processed (" << end - start << " Events)." << endl;
	TStopwatch watch;
	double timeremain;
	watch.Start();
	for(Long64_t i = start ; i < end ; i++)
	{
		Int_t evres = 0; 
		GetEvent(i);

		if(IsBatchSelected(Run(), LumiBlock()))
		{
			if(NumErrors() != 0)
			{
				cerr << "ERROR " << NumErrors() << "in Event: Nr: " << UInt_t(Number()) << ", Run: " << Run() << ", LumiBlock: " << LumiBlock() << ". It's rejected!" << endl;
				continue;
			}
			if(!jsonfilter || jsonlist[Run()][LumiBlock()])
			{
				evres = AnalyseEvent();
				analyzed++;
			}

			if(duplicatecheck)
			{
				eventlist[Run()][LumiBlock()][Number()]++;
			}

			if(lumicalculation && evres > 0)
			{
				++lumilist[Run()][LumiBlock()];
				//Setting the Range
				if(Run() < minRun){minRun = Run(); minLumi = LumiBlock();}
				if(Run() == minRun && LumiBlock() < minLumi) {minLumi = LumiBlock();} 
				if(Run() > maxRun){maxRun = Run(); maxLumi = LumiBlock();}
				if(Run() == maxRun && LumiBlock() > maxLumi) {maxLumi = LumiBlock();} 
			}
		}
		processed++;

		if(printinfo > 0 && processed%printinfo == 0)
		{
			timeremain = watch.RealTime()/processed*(end-start-processed);
			watch.Continue();
			if(Batch_MyId() != -1) cerr << "JOB " << Batch_MyId() << ": ";
			//cerr << "Processed: " << processed << "/" << end - start << ", Event: " << UInt_t(Number()) << ", Run: " << Run() << ", LumiBlock: " << LumiBlock() << ", Time: " << int(timeremain)/3600 << ":" << int(timeremain)%3600/60 << ":" << int(timeremain)%60 << ", Mem: " << mem_usage()/1024./1024. << "Mb" << endl;
			cerr << "Processed: " << processed << "/" << end - start << ", Analyzed: " << analyzed << ", Time: " << int(timeremain)/3600 << ":" << int(timeremain)%3600/60 << ":" << int(timeremain)%60 << ", Mem: " << mem_usage()/1024./1024. << "Mb" << endl;
		}

		if(evres == -1){break;}
	}
	
	EndLoop();
	return(processed);
}

//Int_t Analyse::PrepareSkimming(string filename)
//{
//	skimfilename = filename;
//	if(skimfile != 0) {cerr << "YOU CAN USE PREPARESKIMMING() ONLY ONCE." << endl; return(-1);}
//	
//	skimfile = TFile::Open(skimfilename.c_str(), "recreate");
//	skimfile->cd();
//	skimtree = new TTree("AC1B", "AC1B", 1);
//
//	return(1);
//}
//
//Int_t Analyse::SkimEvent()
//{
//	if(skimtree == 0)
//	{
//		cerr << "use PrepareSkimming() before SkimEvent()" << endl;
//		return(-1);
//	}
//	string currentskimfilename(skimtree->GetCurrentFile()->GetName());
//	if(currentskimfilename != skimfilename)
//	{
//		cout << "Change " << skimfilename << " " << currentskimfilename << endl;
//		Long64_t maxsize = TTree::GetMaxTreeSize();
//		TTree::SetMaxTreeSize(maxsize + 1000000000ll);
//		WriteLumiFile(skimfilename, "update");		
//		TTree::SetMaxTreeSize(maxsize);
//		skimfilename = currentskimfilename;
//		selected = map< UInt_t, map< UInt_t, UInt_t > >();
//	}
//	selected[Run()][LumiBlock()]++;
//	skimtree->Fill();
//	return(1);
//}

Int_t Analyse::SetLumi(UInt_t run, UInt_t block, Float_t lumival, Float_t avgpu)
{
	if(lumilist.find(run) != lumilist.end() && lumilist.find(run)->second.find(block) != lumilist.find(run)->second.end())
	{
		lumilist[run][block].LumiValue(lumival);
		lumilist[run][block].AvgPU(avgpu);
		return(1);
	}
	return(0);
}

void Analyse::WriteLumiFile(string filename, string mode)
{
	TFile* file = TFile::Open(filename.c_str(), mode.c_str());
	BASEIOLUMI::BaseIO baseiolumi("URLUMI", true);
	BASEIORUN::BaseIO baseiorun("URRUN", true);

	baseiorun.SetFile(file);
	baseiolumi.SetFile(file);


	for(map<UInt_t, RunInfo>::iterator b = runlist.begin() ; b != runlist.end() ; ++b)
	{
		baseiorun.StartFilling();
		BASEIORUN::IORunInfo runinfo(baseiorun.GetIORunInfo(0));
		b->second.FillOutPut(runinfo);
		baseiorun.Fill();
	}

	UInt_t orignumevents = 0;
	UInt_t numevents = 0;
	for(map<UInt_t, map<UInt_t, Luminosity> >::iterator a = lumilist.begin() ; a != lumilist.end() ; ++a)
	{
		for(map<UInt_t, Luminosity>::iterator b = a->second.begin() ; b != a->second.end() ; ++b)
		{
			orignumevents += b->second.NumEventsOrig();
			numevents += b->second.NumEvents();
			baseiolumi.StartFilling();
			BASEIOLUMI::IOLumiInfo lumiinfo(baseiolumi.GetIOLumiInfo(0));
			b->second.FillOutPut(lumiinfo);
			if(skimtree != 0)
			{
				if(selected.find(a->first) != selected.end() && selected.find(a->first)->second.find(b->first) != selected.find(a->first)->second.end())
				{
					lumiinfo.EventsFiltered(selected[a->first][b->first]);
				}
				else
				{
					lumiinfo.EventsFiltered(0);
				}
			}
			if(!jsonfilter || (jsonfilter && jsonlist[lumiinfo.RunNumber()][lumiinfo.BlockNumber()])) baseiolumi.Fill();
		}
	}
	cout << numevents << " out of " << orignumevents << " in dataset." << endl;
	file->Write();
	file->Close();
}

int Analyse::AddLumiFile(string filename, bool updatefiles)
{
	TFile* lumifile = 0;
	lumifile = TFile::Open(filename.c_str());
	if(lumifile == 0 || lumifile->IsZombie())
	{
		cerr << "ERROR AddLumiFile: " << filename << " is not a valid file." << endl;
		return(-1);
	}
	BASEIOLUMI::BaseIO baseiolumi("URLUMI", false);
	BASEIORUN::BaseIO baseiorun("URRUN", false);

	baseiorun.SetFile(lumifile);
	baseiolumi.SetFile(lumifile);

	TNamed* pf = 0;
	lumifile->GetObject("fileprefix", pf);
	if(pf != 0)
	{
		fileprefix = string(pf->GetTitle());
	}
	else
	{
		fileprefix = filename.substr(0, filename.find_last_of("/")+1);
	}

	for(UInt_t i = 0 ; i < baseiolumi.GetEntries() ; i++)
	{
		baseiolumi.GetEntry(i);
		BASEIOLUMI::IOLumiInfo lumiinfo(baseiolumi.GetIOLumiInfo(0));

		if(!IsBatchSelected(lumiinfo.RunNumber(), lumiinfo.BlockNumber())) continue;
		
		if(lumilist.find(lumiinfo.RunNumber()) == lumilist.end() || lumilist.find(lumiinfo.RunNumber())->second.find(lumiinfo.BlockNumber()) == lumilist.find(lumiinfo.RunNumber())->second.end())
		{
			Luminosity& lumi = lumilist[lumiinfo.RunNumber()][lumiinfo.BlockNumber()];
			lumi = Luminosity(lumiinfo);
			if(updatefiles)
			{
				lumi.ClearFiles();
				lumi.AddFile(filename.substr(filename.find_last_of("/")+1));
			}
		}
		else
		{
			//Luminosity newlumi(lumi_value, lumi_valueerr, lumi_livefrac, lumi_deadfrac, lumi_avgpu, lumi_quality, lumi_eventsfiltered, lumi_eventsprocessed, lumi_l1algoprescaletable, lumi_l1algoprescaletable, lumi_l1techprescaletable, myfilenames);
			//lumilist[Run()][LumiBlock()] += newlumi;
			cout << "Warning: Information of run: " << lumiinfo.RunNumber() << ", block: " << lumiinfo.BlockNumber() << " has already been filled. For data this means there are duplicated files for MC it might be ok. Check for duplicated events anyway!" << endl;
		}
	}

	for(UInt_t i = 0 ; i < baseiorun.GetEntries() ; i++)
	{
		baseiorun.GetEntry(i);
		BASEIORUN::IORunInfo runinfo = baseiorun.GetIORunInfo(0);
		if(runlist.find(runinfo.RunNumber()) == runlist.end())
		{
			runlist[runinfo.RunNumber()] = RunInfo(runinfo);
		}
	}
	lumicalculation = true;

	TH1D* mu = 0;
	lumifile->GetObject("mc_mu", mu);
	TH1D* pu = 0;
	lumifile->GetObject("mc_pu", pu);
	if(mu != 0 && pu != 0)
	{
	gROOT->cd();
	mc_mu_in = new TH1D(*mu);
	mc_pu_in = new TH1D(*pu);
	if(mc_mu_in->GetEntries() == 0)
	{
		ismc = false;
		TH1D* da_mu = static_cast<TH1D*>(lumifile->Get("da_mu"));
		totalnumberofevents += da_mu->GetEntries();
	}
	else
	{
		ismc = true;
		totalnumberofevents += mc_mu_in->GetEntries();
	}
	}
	lumifile->Close();
	return(1);
}

Int_t Analyse::IsLumiAvailable()
{
	if(lumilist.find(Run()) != lumilist.end() && lumilist.find(Run())->second.find(LumiBlock()) != lumilist.find(Run())->second.end())
	{
		if(lumilist.at(Run()).at(LumiBlock()).LumiValue() != -1) //We have lumi value or it is MC.
		{
			return(2);
		}
		return(1);
	}
	return(0);
}

void Analyse::ResetLumiValues()
{
	for(map< UInt_t, map< UInt_t, Luminosity > >::iterator a = lumilist.begin() ; a != lumilist.end() ; a++)
	{
		for(map<UInt_t, Luminosity>::iterator b = (a->second).begin() ; b != (a->second).end() ; b++)
		{
			b->second.LumiValue(-1.);
		}
	}
}

Int_t Analyse::GetNumHLTriggers()
{
	if(runlist.find(Run()) != runlist.end())
	{
		return(runlist.at(Run()).NumHLT());
	}
	return(-1);
}

string Analyse::GetHLTNames(UInt_t index)
{
	if(runlist.find(Run()) != runlist.end())
	{
		return(runlist.at(Run()).HLTName(index));
	}
	return("Index out of range.");
}


Int_t Analyse::GetHLTriggerIndex(string triggername)
{
	if(runlist.find(Run()) != runlist.end())
	{
		return(runlist.at(Run()).HLTIndex(triggername));
	}
	else
	{
		cerr << "ERROR GetHLTriggerIndex: Trigger " << triggername << " not found." << endl; 
		return(-1);
	}
}

TriggerSelection* Analyse::AddTriggerSelection(string id, vector<string> triggernames, bool useprescaled)
{
	TriggerSelection* triggerselection = new TriggerSelection(this, triggernames, useprescaled);
	triggerselections[id] = triggerselection;
	return(triggerselection);
}

TriggerSelection* Analyse::GetTriggerSelection(string id)
{
	return(triggerselections[id]);
}

//string Analyse::GetHLTriggerName(UInt_t index) const
//{
//	if(runlist.find(Run()) != runlist.end())
//	{
//		return(runlist.at(Run()).HLTName(index));
//	}
//	else
//	{
//		cerr << "ERROR GetHLTriggerName: Could not find name of trigger with index " << index << endl;
//		return(string("NotFound"));
//	}
//}

Int_t Analyse::GetHLTPrescale(UInt_t triggerindex)
{
	if(IsLumiAvailable())
	{
		Int_t triggertable = lumilist.at(Run()).at(LumiBlock()).HLTTable();
		return(runlist.find(Run())->second.HLTPrescale(triggerindex, triggertable));
	}
	else
	{
		return(-1);
	}
}

//void Analyse::PrintPrescaleInfo(string triggername)
//{
//	for(map< UInt_t , RunInfo>::iterator a = runlist.begin() ; a != runlist.end() ; ++a)
//	{
//		Int_t index = a->second.HLTIndex(triggername);
//		if(index < 0) {cout << a->first << ": " << triggername << " not available." << endl;}
//		cout << a->first << ":" << index;
//		for(UInt_t i = 0 ; i < a->second.NumHLTTables() ; i++)
//		{
//			cout << " " << a->second.HLTPrescale(index, i);
//		}
//		cout << lumilist.at(a->first).size() <<  endl;
//		for(map<UInt_t, Luminosity>::iterator b = lumilist.at(a->first).begin() ; b != lumilist.at(a->first).end() ; b++)
//		{
//			Int_t triggertable = b->second.HLTTable();
//			Int_t prescale = a->second.HLTPrescale(index, triggertable);
//			if(prescale != 1)
//			{
//				cout << "(" << a->first << ":" << prescale << "),";
//			}
//		}
//		cout << endl;
//	}
//
//}
//
//void Analyse::PrintPrescaleInfoB(string triggername)
//{
//	Int_t precur = -2;
//	Int_t triggertablecur = -1;
//	UInt_t runcur = 0;
//	UInt_t lumicur = 0;
//	UInt_t runlast = 0;
//	UInt_t lumilast = 0;
//	Double_t lumi = 0.;
//	string triggernamecur;
//	Int_t triggerindexcur = -1;
//	for(map<UInt_t, map<UInt_t, Luminosity> >::iterator a = lumilist.begin() ; a != lumilist.end() ; ++a)
//	{
//		string triggernamenow;
//		RunInfo& runinfo = runlist.find(a->first)->second;
//		for(UInt_t i = 0 ; i < runinfo.NumHLT() ; i++)
//		{
//			if(regex_match(runinfo.HLTName(i).c_str(), regex(triggername + "_v.*")))
//			{
//				triggernamenow = runinfo.HLTName(i);
//				break;
//			}
//		}
//		Int_t triggerindex = runinfo.HLTIndex(triggernamenow);
//		for(map<UInt_t, Luminosity>::iterator b = a->second.begin() ; b != a->second.end() ; b++)
//		{
//			if(b->second.LumiValue() == -1) continue;
//			lumi+=b->second.LumiValue();
//			Int_t triggertablenow = b->second.HLTTable();
//			Int_t prenow = -1;
//			if(triggerindex != -1)
//			{
//				prenow = runinfo.HLTPrescale(triggerindex, triggertablenow);
//			}
//	
//			
//			if(precur == -2)
//			{
//				triggernamecur = triggernamenow;
//				triggertablecur = triggertablenow;
//				triggerindexcur = triggerindex;
//				precur = prenow;
//				runcur = a->first;
//				lumicur = b->first;
//			}			
//
//			if(precur != prenow || triggertablecur != triggertablenow || triggernamecur != triggernamenow || triggerindexcur != triggerindex)
//			{
//				cout << triggerindexcur << " " << triggernamecur << ", " << runcur << ":" << lumicur << " - " << runlast << ":" << lumilast << ", prescale: " << precur << ", column: "<< triggertablecur  << ", lumi: " << lumi << "/pb"<< endl; 
//				lumi = 0.;
//				triggerindexcur = triggerindex;
//				triggernamecur = triggernamenow;
//				triggertablecur = triggertablenow;
//				precur = prenow;
//				runcur = a->first;
//				lumicur = b->first;
//			}
//			runlast = a->first;
//			lumilast = b->first; 
//		}
//	}
//	cout << triggerindexcur << " " << triggernamecur << ", " << runcur << ":" << lumicur << " - " << runlast << ":" << lumilast << ", prescale: " << precur << ", column: " << triggertablecur  << ", lumi: " << lumi << "/pb"<< endl; 
//}

Double_t Analyse::GetInstLumi()
{
	map<UInt_t, map<UInt_t, Luminosity> >::const_iterator a = lumilist.find(Run());
	if(a == lumilist.end()) return(-1.);
	map<UInt_t, Luminosity>::const_iterator b = a->second.find(LumiBlock());
	if(b == a->second.end()) return(-1.);
	return(b->second.LumiValue());
}

Double_t Analyse::GetAvgPU()
{
	map<UInt_t, map<UInt_t, Luminosity> >::const_iterator a = lumilist.find(Run());
	if(a == lumilist.end()) return(-1.);
	map<UInt_t, Luminosity>::const_iterator b = a->second.find(LumiBlock());
	if(b == a->second.end()) return(-1.);
	return(b->second.AvgPU());
}

//Double_t Analyse::GetLumi(Int_t format)
//{
//	Double_t lumi = 0.;
//	Double_t alllumi = 0.;
//	Double_t zerolumi = 0.;
//	UInt_t nolumiinfo = 0;
//	Double_t events = 0.;
//	Double_t eventsprocessed = 0.;
//	for(map<UInt_t, map<UInt_t, Luminosity> >::iterator a = lumilist.begin() ; a != lumilist.end() ; ++a)
//	{
//		Double_t runlumi = 0.;
//		Double_t runalllumi = 0.;
//		Double_t runzerolumi = 0.;
//		Double_t runevents = 0.;
//		Double_t runeventsprocessed = 0.;
//		UInt_t runnolumiinfo = 0;
//		UInt_t numblocks = 0;	
//		for(map<UInt_t, Luminosity>::iterator b = a->second.begin() ; b != a->second.end() ; b++)
//		{
//			if(b->second.LumiValue() == -1){continue;}
//			if(IsInRange(a->first, b->first))
//			{
//				numblocks++;
//			//	cout << a->first << " " << b->first << " " << minLumi << " " << maxLumi << endl;
//				if(b->second == true)
//				{
//					if(b->second.NumEvents() > 0)
//					{
//						runlumi += b->second.ProcessedLumi();
//						runalllumi += b->second.LumiValue();
//						runevents += b->second.NumEvents();
//						runeventsprocessed += b->second.NumEventsProcessed();
//						if(b->second.ProcessedFraction() > 1)
//						{
//							cerr << "WARNING GetLumi: You ran on " << b->second.ProcessedFraction()*100 <<"% of available events in Run " << a->first << ", Lumiblock " << b->first <<". :-O" << endl;  
//						}
//						if(format >= 3) cout << "    Block: " << b->first << ", fraction: " << b->second.ProcessedFraction() << ", lumi: " << b->second.ProcessedLumi() << " pb^-1" << endl;
//					}
//					else
//					{
//						runzerolumi += b->second.LumiValue(); 
//						runalllumi += b->second.LumiValue();
//					}
//				}
//				else if(b->second == false)
//				{
//					runnolumiinfo += b->second.NumEventsProcessed();
//				}
//			}
//		}
//		if(a->first <= maxRun && a->first >= minRun)
//		{
//			if(runevents != 0) zerolumi *= runeventsprocessed/runevents;
//			if(format >= 2)  cout << "Run: " << a->first << ", blocks: " << numblocks << ", fraction: " << runeventsprocessed/runevents << ", total lumi: " << runalllumi << " pb^-1, processed lumi: " << runlumi+zerolumi << " zero event lumi: " << zerolumi << " pb^-1, no lumi info for: " << runnolumiinfo << " event(s)."<< endl;
//			lumi += runlumi;
//			zerolumi += runzerolumi;
//			alllumi += runalllumi;
//			events += runevents;
//			eventsprocessed += runeventsprocessed;
//			nolumiinfo += runnolumiinfo; 
//		}
//	}
//	if(format >= 1)
//	{
//		cout << "From run " << minRun << ", block " << minLumi << " to run " << maxRun << ", block " << maxLumi << "." << endl;
//		cout << "Total lumi: " << lumi+zerolumi << " pb^-1, zero event lumi: " << zerolumi << " pb^-1, (lumi in Range: " << alllumi << " pb^-1), no lumi-info for " << nolumiinfo << " event(s), events: " << eventsprocessed << "(" << events << ")"  << endl;
//	}
//return(lumi+zerolumi);
//}

bool Analyse::IsInRange(UInt_t theRun, UInt_t theLumiBlock)
{
	if(theRun < maxRun && theRun > minRun) 
	{
		return(true);
	}
	else if(maxRun != minRun && theRun == maxRun && theLumiBlock <= maxLumi)
	{
		return(true);
	}
	else if(maxRun != minRun && theRun == minRun && theLumiBlock >= minLumi)
	{
		return(true);
	}
	else if(maxRun == minRun && theRun == minRun && theLumiBlock <= maxLumi && theLumiBlock >= minLumi)
	{
		return(true);
	}
	else 
	{
		return(false);
	}
}

//void Analyse::PrintLumiOfRuns()
//{
//	Double_t totlumi = 0.;
//	Double_t filteredevents = 0;
//	Double_t allevents = 0;
//	for(map<UInt_t, map<UInt_t, Luminosity> >::iterator a = lumilist.begin() ; a != lumilist.end() ; ++a)
//	{
//		Double_t runlumi = 0;
//		Double_t runfilteredevents = 0;
//		Double_t runallevents = 0;
//		for(map<UInt_t, Luminosity>::iterator b = a->second.begin() ; b != a->second.end() ; b++)
//		{
//			if(b->second.LumiValue() != -1.)
//			{
//				runlumi+=b->second.LumiValue();
//				totlumi+=b->second.LumiValue();
//			}
//			else
//			{
//				cout << "Missing luminosity value for section: " << a->first << " " << b->first << endl;
//			}
//			filteredevents+=b->second.NumEvents();
//			runfilteredevents+=b->second.NumEvents();
//			allevents+=b->second.NumEventsOrig();
//			runallevents+=b->second.NumEventsOrig();
//		}
//		cout << "Run: " << a->first << " -> " << runlumi << ", Events: " << (UInt_t)runfilteredevents << "/" << (UInt_t)runallevents <<"="<< runfilteredevents/runallevents << "  sum: " << totlumi << ", Events: " << (UInt_t)filteredevents << "/" << (UInt_t)allevents <<"="<< filteredevents/allevents<< endl;
//	}
//}
//
//void Analyse::PrintLumiOfLumiSectionsInRun(UInt_t runnumber)
//{
//	Double_t runlumi = 0.;
//	Double_t runfilteredevents = 0.;
//	Double_t runallevents = 0.;
//	map<UInt_t, map<UInt_t, Luminosity> >::iterator blocklist = lumilist.find(runnumber);
//	for(map<UInt_t, Luminosity>::iterator b = blocklist->second.begin() ; b != blocklist->second.end() ; ++b)
//	{
//		if(b->second.LumiValue() != -1.)
//		{
//			runlumi+=b->second.LumiValue();
//			runfilteredevents+=b->second.NumEvents();
//			runallevents+=b->second.NumEventsOrig();
//			cout << "Section: " << b->first << ", Lumi: " << b->second.LumiValue() << "("<<runlumi<<"), Events: "<< b->second.NumEvents() << "(" << runfilteredevents << ")" << endl; 
//		}
//		else
//		{
//			cout << "Missing lumi info for section: " << b->first << endl;
//		}
//	}
//
//}

bool Analyse::LoadJSON(string filename)
{
	jsonfilter = true;
	char buf[100];
	string strjson;
	fstream json(filename.c_str());
	while(true)
	{
		json.read(buf, 100);
		int read = json.gcount();
		strjson += string(buf, read);
		if(read < 100) break;
	}

	UInt_t run;
	UInt_t lumibeg;
	UInt_t lumiend;
	size_t posquota = 0;
	size_t posquotb = 0;
	size_t runbraon = 0;
	size_t runbraoff = 0;
	size_t lumibraon = 0;
	size_t lumibraoff = 0;
	size_t sep;
	bool result = true;
	while(true)
	{
		posquota = strjson.find("\"", runbraoff+1);
		posquotb = strjson.find("\"", posquota+1);
		if(posquota == string::npos) break;
		run = atoi(strjson.substr(posquota+1, posquotb-1).c_str());
		runbraon = strjson.find("[", posquotb+1);
		lumibraoff = runbraon;
		while(true)
		{
			lumibraon = strjson.find("[", lumibraoff+1);
			lumibraoff = strjson.find("]", lumibraoff+1);
			if(lumibraoff < lumibraon) break;
			sep = strjson.find(",", lumibraon+1);
			lumibeg = atoi(strjson.substr(lumibraon+1, sep-1).c_str());
			lumiend = atoi(strjson.substr(sep+1, lumibraoff-1).c_str());

			for(UInt_t u = lumibeg ; u <= lumiend ; u++)
			{
				if(lumilist.find(run) != lumilist.end() && lumilist.find(run)->second.find(u) != lumilist.find(u)->second.end())
				{
					jsonlist[run][u] = true;
				}
				else
				{
					cout << "JSONFilter: Run: " << run << ", LS: " << u << " is not part of your input files." << endl;
					result = false;
				}
			}  

		}
		runbraoff = lumibraoff;
	}
	return(result);
}

TH1D* Analyse::SetupPileupWeighting(string filename, string histname, bool mu)
{
	if(IsData()) {return(0);}
	TFile* pufile = TFile::Open(filename.c_str(), "read");
	TH1D* outhist = 0;
	pufile->GetObject(histname.c_str(), outhist);
	TH1D* inhist = mc_pu_in;
	if(mu){inhist = mc_mu_in;}
	if(outhist->GetNbinsX() != inhist->GetNbinsX() || outhist->GetXaxis()->GetXmin() != inhist->GetXaxis()->GetXmin() || outhist->GetXaxis()->GetXmax() != inhist->GetXaxis()->GetXmax()) {cerr << "ERROR SetupPileupWeighting: binning of PU histograms not compatible (exit)." << endl; exit(-1);}
	TDirectory* curdir = gDirectory;
	gROOT->cd();
	pu_reweight = new TH1D(*outhist);
	double iout = outhist->Integral();
	double iin = inhist->Integral();
	for(int b = 1 ; b <= pu_reweight->GetNbinsX() ; ++b)
	{
		double weight = 0.;
		if(inhist->GetBinContent(b) == 0 && outhist->GetBinContent(b) != 0)
		{cerr << "WARNING SetupPileupWeighting: you request weights for PU that is not available in the input distribution." << endl;}
		else if(inhist->GetBinContent(b) != 0 && outhist->GetBinContent(b) != 0)
		{
			weight = outhist->GetBinContent(b)/inhist->GetBinContent(b)*iin/iout;
		}
		pu_reweight->SetBinContent(b, weight);
	}
	if(mu) {pu_reweight->SetBinContent(0, 1);} else {pu_reweight->SetBinContent(0, 0);}
	curdir->cd();
	return(pu_reweight);
}

double Analyse::GetPileupWeight()
{
	if(IsData() || pu_reweight == 0) {return(1.);}
	double val = -1;
	if(pu_reweight->GetBinContent(0) == 1)
	{
		val = GetGenInfo(0).NumTrueInteractions();
	}
	else if(pu_reweight->GetBinContent(0) == 0)
	{
		val = GetGenInfo(0).NumPUInteractions();
	}
	return(pu_reweight->GetBinContent(pu_reweight->FindFixBin(val)));
}

//Double_t Analyse::GetPileUpMaxWeight(vector<Double_t>& datadist) const
//{
//	double maxweight = -1.;
//	for(size_t nv = 1 ; nv < datadist.size() ; nv++)
//	{
//		cout << nv <<  " " << datadist[nv] << " " << pileUpDist[nv] << endl;
//		if(pileUpDist[nv] > 0. && datadist[nv]/pileUpDist[nv] > maxweight)
//		{
//			maxweight = datadist[nv]/pileUpDist[nv];
//		}
//	}
//	return(maxweight);
//}
//
//Double_t Analyse::GetPileUpWeight(vector<Double_t>& datadist) const
//{
//	if(!usepileupinfo) return(1.);
//	unsigned numtrueinteractions = unsigned(2*NumTruePileUpInteractions());
//	Double_t dataval = 0.;
//	if(datadist.size() > numtrueinteractions)
//	{
//		dataval = datadist[numtrueinteractions];
//	}
//	return(dataval/pileUpDist[numtrueinteractions]);
//}
//
//
//Double_t Analyse::GetPrimVertexMaxWeight(vector<Double_t>& datadist) const
//{
//	double maxweight = -1.;
//	for(size_t nv = 1 ; nv < datadist.size() ; nv++)
//	{
//		//cout << nv <<  " " << datadist[nv] << " " << primVertexDist[nv] << endl;
//		if(primVertexDist[nv] > 0. && datadist[nv]/primVertexDist[nv] > maxweight)
//		{
//			maxweight = datadist[nv]/primVertexDist[nv];
//		}
//	}
//	return(maxweight);
//}
//
//Double_t Analyse::GetPrimVertexWeight(vector<Double_t>& datadist) const
//{
//	if(!useprimvertexinfo) return(1.);
//	UInt_t numgoodprimvertices = NumGoodPrimVertices();
//	Double_t dataval = 0.;
//	if(datadist.size() > numgoodprimvertices)
//	{
//		dataval = datadist[numgoodprimvertices];
//	}
//	return(dataval/primVertexDist[numgoodprimvertices]);
//}
//
//Int_t Analyse::NumGoodPrimVertices() const
//{
//	Int_t numgoodprimvertices = 0;
//	for(UInt_t i = 0 ; i < NumPrimVertices() ; i++)
//	{
//		if(PrimVertices(i).IsGood()) numgoodprimvertices++;
//	}
//	return(numgoodprimvertices);
//}

Long64_t mem_usage()
{
	ifstream stat_stream("/proc/self/stat",ios_base::in);

	// dummy vars for leading entries in stat that we don't care about
	//
	string pid, comm, state, ppid, pgrp, session, tty_nr;
	string tpgid, flags, minflt, cminflt, majflt, cmajflt;
	string utime, stime, cutime, cstime, priority, nice;
	string O, itrealvalue, starttime;

	// the two fields we want
	//
	unsigned long vsize;
	Long64_t rss;

	stat_stream >> pid >> comm >> state >> ppid >> pgrp >> session >> tty_nr
		>> tpgid >> flags >> minflt >> cminflt >> majflt >> cmajflt
		>> utime >> stime >> cutime >> cstime >> priority >> nice
		>> O >> itrealvalue >> starttime >> vsize >> rss; // don't care about the rest

	stat_stream.close();
	return(rss*sysconf(_SC_PAGE_SIZE));
}

