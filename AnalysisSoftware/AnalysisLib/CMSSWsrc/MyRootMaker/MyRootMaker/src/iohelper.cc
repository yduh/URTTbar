#include "../interface/iohelper.h"
#include "TFile.h"
#include "TMath.h"
#include <iostream>

using namespace std;
using namespace BASEIO;

namespace BASEIO{
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

EventInfo::EventInfo(const IOEventInfo& evinfo, size_t size) : IOEventInfo(evinfo), b(size/8 + 1, 0)
{

}
void EventInfo::SetHLT(UInt_t num, bool val)
{
	if(val)
	{
		b[num/8] |=  1 << num % 8;
	}
	else
	{
		b[num/8] &= ~(1 << num % 8);
	}
}

void EventInfo::Write()
{
	for(size_t i = 0 ; i < b.size() ; ++i)
	{
		TriggerHLT(b[i], i);
	}
}

}


namespace BASEIORUN{
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

namespace BASEIOLUMI{
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
