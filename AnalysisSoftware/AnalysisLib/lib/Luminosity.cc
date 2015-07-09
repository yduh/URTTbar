#include "Luminosity.h"
#include "iohelper.h"
#include <algorithm>

Luminosity::Luminosity(const BASEIOLUMI::IOLumiInfo& lumiinfo) : 
	runnumber_(lumiinfo.RunNumber()), 
	blocknumber_(lumiinfo.BlockNumber()), 
	lumival_(lumiinfo.LumiValue()), 
	lumivalunc_(lumiinfo.LumiValueUnc()), 
	avgpu_(lumiinfo.AveragePU()),
	numeventsfiltered_(lumiinfo.EventsFiltered()),
	numeventsprocessed_(lumiinfo.EventsProcessed()),
	hltcolumn_(lumiinfo.HLTPrescaleColumn()),
	counter_(0) 
{
	for(UInt_t n = 0 ; n < lumiinfo.Num_FileNames() ; ++n)
	{
		AddFile(BASEIOLUMI::String(lumiinfo.FileNames(n)).Get());
	}
}


Luminosity::Luminosity() : 
	lumival_(-1),
	numeventsfiltered_(0),
	numeventsprocessed_(0),
	counter_(0) 
{}

bool Luminosity::operator ==(const bool test) 
{
	bool val = true;
	if(lumival_ == -1)
	{
		val = false;
	} 
	return(test == val);
}

Luminosity& Luminosity::operator +=(Luminosity& other)
{
	numeventsfiltered_ += other.NumEvents();
	numeventsprocessed_ += other.NumEventsOrig();
	AddFiles(other.GetFiles());
	return(*this);
}

void Luminosity::AddFile(const string& newfile)
{
	if(find(filenames_.begin(), filenames_.end(), newfile) == filenames_.end())
	{
		filenames_.push_back(newfile);
	}
}

void Luminosity::AddFiles(const vector<string>& newfiles)
{
	for(const string& fn : newfiles)
	{
		AddFile(fn);
	}
}

string Luminosity::GetFilesString() const
{
	string files(filenames_[0].substr(filenames_[0].find_last_of("/")+1));
	for(size_t i = 1 ; i < filenames_.size() ; i++)
	{
		files += " " + filenames_[i].substr(filenames_[i].find_last_of("/")+1);
	}
	return(files);
}

Float_t Luminosity::ProcessedFraction() const 
{
	if(NumEvents() == 0)
	{
		return(1.);
	}
	return(Float_t(NumEventsProcessed())/NumEvents());
}

Float_t Luminosity::ProcessedLumi() const 
{
	return(ProcessedFraction()*LumiValue());
}

void Luminosity::FillOutPut(BASEIOLUMI::IOLumiInfo& lumiinfo)
{
	lumiinfo.RunNumber(runnumber_);
	lumiinfo.BlockNumber(blocknumber_);
	lumiinfo.LumiValue(lumival_);
	lumiinfo.LumiValueUnc(lumivalunc_);
	lumiinfo.AveragePU(avgpu_);
	lumiinfo.EventsProcessed(numeventsprocessed_);
	lumiinfo.EventsFiltered(numeventsfiltered_);
	lumiinfo.HLTPrescaleColumn(hltcolumn_);
	for(size_t i = 0 ; i < filenames_.size() ; ++i)
	{
		BASEIOLUMI::String(lumiinfo.FileNames(i)).Set(filenames_[i]);
	}
}
