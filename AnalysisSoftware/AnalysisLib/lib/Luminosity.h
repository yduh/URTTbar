#ifndef CLUMINOSITY
#define CLUMINOSITY

#include "BASEIOLUMI.h"
#include <string>
#include <vector>

using namespace std;

class Luminosity
{
	private:
		UInt_t runnumber_;
		UInt_t blocknumber_;
		Float_t lumival_;
		Float_t lumivalunc_;
		Float_t avgpu_;
		UInt_t numeventsfiltered_;
		UInt_t numeventsprocessed_;
		Int_t hltcolumn_;
		UInt_t counter_;
		vector<string> filenames_;
	public:
		Luminosity();
		Luminosity(const BASEIOLUMI::IOLumiInfo& lumiinfo); 
		bool operator ==(const bool test);
		void operator ++() {counter_++;}
		Luminosity& operator +=(Luminosity& other);
		void AddFile(const string& newfile);
		void AddFiles(const vector<string>& newfiles);
		void ClearFiles() {filenames_.clear();};
		UInt_t NumFiles() const {return(filenames_.size());}
		const vector<string>& GetFiles() const {return(filenames_);}
		string GetFilesString() const;
		Float_t LumiValue() const {return(lumival_);}
		Float_t LumiValueUnc() const {return(lumivalunc_);}
		Float_t AvgPU() const {return(avgpu_);}
		UInt_t NumEvents() const {return(numeventsfiltered_);}
		UInt_t NumEventsOrig() const {return(numeventsprocessed_);}
		UInt_t NumEventsProcessed() const {return(counter_);}
		Float_t ProcessedFraction() const;
		Float_t ProcessedLumi() const;
		UInt_t HLTTable() const {return(hltcolumn_);}
		void LumiValue(Float_t lumival){lumival_ = lumival;}
		void LumiValueUnc(Float_t lumivalunc){lumivalunc_ = lumivalunc;}
		void AvgPU(Float_t avgpu){avgpu_ = avgpu;}
		void FillOutPut(BASEIOLUMI::IOLumiInfo& lumiinfo);
		void EventsFiltered(UInt_t eventsfiltered){numeventsfiltered_ = eventsfiltered;}
		//void EventsOriginal(UInt_t myeventsorig){numeventsorig = myeventsorig;}
		//void EventsProcessed(UInt_t myeventsprocessed){counter = myeventsprocessed;}
};

#endif
