#include "iohelper.h"
#include "TFile.h"
#include "TMath.h"
#include <iostream>

using namespace std;

void splitstring(string input, vector<string>& output)
{
	UInt_t posstart = 0;
	for(UInt_t i = 0 ; i < input.size() ; i++)
	{
		if(input[i] == ' ')
		{
			output.push_back(input.substr(posstart, i-posstart));
			posstart=i+1;
		}
	}
}

string combinestring(const vector<string>& input)
{
	string output;
	for(UInt_t i = 0 ; i < input.size() ; i++)
	{
		output += input[i] + string(" ");
	}
	return(output);
}

namespace BASEIO
{
String::String(const IOString& str) : IOString(str)
{
}	
void String::Set(string cppstr)
{
	for(size_t b = 0 ; b < cppstr.size() ; ++b)
	{
		str(cppstr[b], b);
	}
}
string String::Get()
{
	string cppstr(Num_str(), ' ');	
	for(size_t b = 0 ; b < cppstr.size() ; ++b)
	{
		cppstr[b] = str(b);
	}
	return cppstr;
}


Muon::Muon(const IOMuon& mu) : IOMuon(mu), TLorentzVector(px(), py(), pz(), TMath::Sqrt(px()*px() + py()*py() + pz()*pz()))
{

}

EventInfo::EventInfo(const IOEventInfo& evinfo) : IOEventInfo(evinfo)
{

}
void EventInfo::SetHLT(UInt_t num, bool val)
{
	UChar_t b = TriggerHLT(num/8);
	if(val)
	{
		b |=  1 << num % 8;
	}
	else
	{
		b &= ~(1 << num % 8);
	}
	TriggerHLT(b, num/8);
}

bool EventInfo::GetHLT(UInt_t num)
{
	if(TriggerHLT(num/8) & 1 << num % 8){return true;}
	return false;
}
}
namespace BASEIORUN
{
String::String(const IOString& str) : IOString(str)
{
}	
void String::Set(string cppstr)
{
	for(size_t b = 0 ; b < cppstr.size() ; ++b)
	{
		str(cppstr[b], b);
	}
}
string String::Get()
{
	string cppstr(Num_str(), ' ');	
	for(size_t b = 0 ; b < cppstr.size() ; ++b)
	{
		cppstr[b] = str(b);
	}
	return cppstr;
}
}
namespace BASEIOLUMI
{
String::String(const IOString& str) : IOString(str)
{
}	
void String::Set(string cppstr)
{
	for(size_t b = 0 ; b < cppstr.size() ; ++b)
	{
		str(cppstr[b], b);
	}
}
string String::Get()
{
	string cppstr(Num_str(), ' ');	
	for(size_t b = 0 ; b < cppstr.size() ; ++b)
	{
		cppstr[b] = str(b);
	}
	return cppstr;
}
}
