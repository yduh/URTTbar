#include "TriggerSelection.h"
#include "Analyse.h"


void TriggerRun::SetBlock(UInt_t block, Int_t index, UInt_t prescale)
{
	if(lumiinfo.find(block) == lumiinfo.end())
	{
		lumiinfo[block] = TriggerLumi(index, prescale);
	}
}

TriggerLumi TriggerRun::GetBlock(UInt_t block)
{
	map<UInt_t, TriggerLumi>::const_iterator blockinfo = lumiinfo.find(block);
	if(blockinfo == lumiinfo.end())
	{
		return(TriggerLumi(-1, 0));
	}
	return(blockinfo->second);
}

TriggerSelection::TriggerSelection(Analyse* an, vector<string> names, bool useprescaled) : AN(an)
{
	for(map<UInt_t, RunInfo>::iterator a = AN->runlist.begin() ; a != AN->runlist.end(); ++a)
	{
		UInt_t runnumber = a->first;
		Float_t runlumi = 0.;
		vector<UInt_t> indices;
		for(UInt_t i = 0 ; i < names.size() ; i++)
		{
			vector<string> goodnames = a->second.MatchTriggerNames(names[i]);
			if(goodnames.size() == 1)
			{
				indices.push_back(a->second.HLTIndex(goodnames[0]));
			}
			else if(goodnames.size() > 1)
			{
				cout << "WARNING: TriggerSelection::TriggerSelection: " << names[i] << " is not a unique selection!" << endl;
			}
		}

		map<UInt_t, map<UInt_t, Luminosity> >::iterator blocklist = AN->lumilist.find(runnumber);
		for(map<UInt_t, Luminosity>::iterator b = blocklist->second.begin() ; b != blocklist->second.end() ; ++b)
		{
			Int_t minindex = -1;
			UInt_t minprescale = 10000000;

			for(UInt_t i = 0 ; i < indices.size() ; i++)
			{
				UInt_t prescale = a->second.HLTPrescale(indices[i], b->second.HLTTable());
				if(prescale <= 0)
				{
					continue;
				}
				if(prescale < minprescale)
				{
					minindex = indices[i];
					minprescale = prescale;
				}
				if(prescale == 1)
				{
					break;
				}
			}
			if(useprescaled == false && minprescale != 1)
			{
				runinfo[runnumber].SetBlock(b->first, -1, minprescale);
				continue;
			}
			if(b->second.LumiValue() != -1)
			{
				runlumi += b->second.LumiValue() * minprescale;
			}
			runinfo[runnumber].SetBlock(b->first, minindex, minprescale);
		}
		runinfo[runnumber].Lumi(runlumi);
	}
}

Int_t TriggerSelection::Result()
{
	const TriggerLumi& triggerlumi = runinfo[AN->Run()].GetBlock(AN->LumiBlock());
	if(triggerlumi.Index() < 0){return(0);}
	if(AN->GetHLTriggerResult(triggerlumi.Index()))
	{
		return(triggerlumi.Prescale());
	}
	else
	{
		return(triggerlumi.Prescale() * -1);
	}
}

Float_t TriggerSelection::LumiUsed(Int_t format)
{
	Double_t lumi = 0.;
	Double_t alllumi = 0.;
	Double_t zerolumi = 0.;
	UInt_t nolumiinfo = 0;
	Double_t events = 0.;
	Double_t eventsprocessed = 0.;
	for(map<UInt_t, map<UInt_t, Luminosity> >::iterator a = AN->lumilist.begin() ; a != AN->lumilist.end() ; ++a)
	{
		Double_t runlumi = 0.;
		Double_t runalllumi = 0.;
		Double_t runzerolumi = 0.;
		Double_t runevents = 0.;
		Double_t runeventsprocessed = 0.;
		UInt_t runnolumiinfo = 0;
		UInt_t numblocks = 0;	
		for(map<UInt_t, Luminosity>::iterator b = a->second.begin() ; b != a->second.end() ; b++)
		{
			const TriggerLumi& triggerlumi = runinfo[a->first].GetBlock(b->first);
			if(b->second.LumiValue() == -1 || triggerlumi.Index() < 0){continue;}
			if(AN->IsInRange(a->first, b->first))
			{
				numblocks++;
			//	cout << a->first << " " << b->first << " " << minLumi << " " << maxLumi << endl;
				if(b->second == true)
				{
					if(b->second.NumEvents() > 0)
					{
						runlumi += b->second.ProcessedLumi()*triggerlumi.Prescale();
						runalllumi += b->second.LumiValue()*triggerlumi.Prescale();
						runevents += b->second.NumEvents();
						runeventsprocessed += b->second.NumEventsProcessed();
						if(b->second.ProcessedFraction() > 1)
						{
							cerr << "WARNING GetLumi: You ran on " << b->second.ProcessedFraction()*100 <<"% of available events in Run " << a->first << ", Lumiblock " << b->first <<". :-O" << endl;  
						}
						if(format >= 3) cout << "    Block: " << b->first << ", fraction: " << b->second.ProcessedFraction() << ", lumi: " << b->second.ProcessedLumi() << " pb^-1" << endl;
					}
					else
					{
						runzerolumi += b->second.LumiValue()*triggerlumi.Prescale(); 
						runalllumi += b->second.LumiValue()*triggerlumi.Prescale();
					}
				}
				else if(b->second == false)
				{
					runnolumiinfo += b->second.NumEventsProcessed();
				}
			}
		}
		if(a->first <= AN->maxRun && a->first >= AN->minRun)
		{
			if(runevents != 0) zerolumi *= runeventsprocessed/runevents;
			if(format >= 2)  cout << "Run: " << a->first << ", blocks: " << numblocks << ", fraction: " << runeventsprocessed/runevents << ", total lumi: " << runalllumi << " pb^-1, processed lumi: " << runlumi+zerolumi << " zero event lumi: " << zerolumi << " pb^-1, no lumi info for: " << runnolumiinfo << " event(s)."<< endl;
			lumi += runlumi;
			zerolumi += runzerolumi;
			alllumi += runalllumi;
			events += runevents;
			eventsprocessed += runeventsprocessed;
			nolumiinfo += runnolumiinfo; 
		}
	}
	if(format >= 1)
	{
		cout << "From run " << AN->minRun << ", block " << AN->minLumi << " to run " << AN->maxRun << ", block " << AN->maxLumi << "." << endl;
		cout << "Total lumi: " << lumi+zerolumi << " pb^-1, zero event lumi: " << zerolumi << " pb^-1, (lumi in Range: " << alllumi << " pb^-1), no lumi-info for " << nolumiinfo << " event(s), events: " << eventsprocessed << "(" << events << ")"  << endl;
	}
return(lumi+zerolumi);

}

Float_t TriggerSelection::LumiBeforeEvent()
{
	Float_t lumi = 0.;
	for(map<UInt_t, TriggerRun>::iterator a = runinfo.begin() ; a != runinfo.end(); ++a)
	{
		if(a->first < AN->Run()) 
		{
			lumi+=a->second.Lumi();
		}
		else
		{
			break;
		}
	}

	map<UInt_t, map<UInt_t, Luminosity> >::iterator blocklist = AN->lumilist.find(AN->Run());
	for(map<UInt_t, Luminosity>::iterator b = blocklist->second.begin() ; b != blocklist->second.end() ; ++b)
	{
		if(b->first <= AN->LumiBlock())
		{
			const TriggerLumi& triggerlumi = runinfo[AN->Run()].GetBlock(b->first);
			if(triggerlumi.Index() >= 0 && b->second.LumiValue() != -1)
			{
				lumi+=b->second.LumiValue()*triggerlumi.Prescale();
			}
		}
		else
		{
			break;
		}
	}
	return(lumi);
}


string TriggerSelection::GetTriggerName(UInt_t run, UInt_t lumiblock)
{
	if(run == 0 && lumiblock == 0)
	{
		run = AN->Run();
		lumiblock = AN->LumiBlock();
	}

	Int_t index = runinfo[run].GetBlock(lumiblock).Index();
	if(index != -1)
	{
		return(AN->runlist[run].HLTName(index));
	}
	cerr << "TriggerSelection::GetTriggerName: Invalid run and/or lumiblock." << endl;
	return("ERROR: TriggerSelection::GetTriggerName: Invalid run and/or lumiblock.");
}

void TriggerSelection::PrintInfo()
{
	Float_t lumi = 0.;
	Float_t lumiuse = 0.;
	Float_t curprescale;
	string curname;
	bool beg = true;
	UInt_t begrun, begblock;
	Float_t beglumi = 0.;
	UInt_t run, block; 

	for(map<UInt_t, TriggerRun>::iterator runit = runinfo.begin() ; runit != runinfo.end() ; ++runit)
	{
		for(map< UInt_t, TriggerLumi >::iterator lumiit = runit->second.Begin() ; lumiit != runit->second.End() ; ++lumiit)
		{
			run = runit->first;
			block = lumiit->first;
			Int_t index = lumiit->second.Index();
			Float_t prescale = 0;
			string name("NA");
			if(index != -1)
			{
				name = AN->runlist[run].HLTName(index);
				prescale = lumiit->second.Prescale();
				if(AN->lumilist[run][block].LumiValue() != -1) {lumiuse += AN->lumilist[run][block].LumiValue();}
			}
			if(beg)
			{
				begrun = run;
				begblock = block;
				beglumi = lumi;
				curname = name;
				curprescale = prescale;
				beg = false;
			}
			if(AN->lumilist[run][block].LumiValue() != -1){lumi += AN->lumilist[run][block].LumiValue();}
			if(curname != name || curprescale != prescale)
			{
				cout << begrun << ":" << begblock << " - " << run << ":" << block << ", " << curname << "(" << curprescale << "), Lumi: " << lumi << "(" << lumi - beglumi << ")/pb" << endl; 	
				begrun = run;
				begblock = block;
				beglumi = lumi;
				curname = name;
				curprescale = prescale;
			}
		}
	}
				cout << begrun << ":" << begblock << " - " << run << ":" << block << ", " << curname << "(" << curprescale << "), Lumi: " << lumi << "(" << lumi - beglumi << ")/pb" << endl; 	
				cout << "Triggered Lumi: " << lumiuse << "/pb" << endl;

}


