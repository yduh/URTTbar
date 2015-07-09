#ifndef CTRIGGERSELECTION
#define CTRIGGERSELECTION

#include<map>
#include <string>
#include <Rtypes.h>

using namespace std;

class Analyse;

class TriggerLumi
{
	private:
		Int_t index;
		UInt_t prescale;
	public:
		TriggerLumi() : index(-1), prescale(0) {}
		TriggerLumi(Int_t Index, UInt_t Prescale) : index(Index), prescale(Prescale) {}
		Int_t Index() const {return(index);}
		UInt_t Prescale() const {return(prescale);}
};

class TriggerRun
{
	private:
		Float_t lumi;
		map< UInt_t, TriggerLumi> lumiinfo;
	public:
		TriggerRun() : lumi(-1.) {}
		void Lumi(Float_t runlumi) {lumi = runlumi;}
		void SetBlock(UInt_t block, Int_t index, UInt_t prescale);
		TriggerLumi GetBlock(UInt_t block);
		Float_t Lumi() const {return(lumi);}
		map< UInt_t, TriggerLumi>::iterator Begin() {return(lumiinfo.begin());} 
		map< UInt_t, TriggerLumi>::iterator End() {return(lumiinfo.end());} 
};

class TriggerSelection
{
	private:
		Analyse* AN;
		map<UInt_t, TriggerRun> runinfo;
	public:
		TriggerSelection(Analyse* an, vector<string> names, bool useprescaled = false);
		Int_t Result();
		Float_t LumiUsed(Int_t format = 0);
		Float_t LumiBeforeEvent();
		string GetTriggerName(UInt_t run = 0, UInt_t lumiblock = 0);
		void PrintInfo();
};

#endif
