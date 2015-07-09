#include <BASEIO.h>
#include <BASEIORUN.h>
#include <BASEIOLUMI.h>
#include "TLorentzVector.h"
#include <iostream>
#include <string>

using namespace std;

void splitstring(string input, vector<string>& output);
string combinestring(const vector<string>& input);

namespace BASEIO
{
class String : public IOString
{
	private:

	public:
		String(const IOString& str);
		void Set(string cppstr);
		string Get();
};

class Muon : public IOMuon, TLorentzVector
{
private:

public:
	Muon(const IOMuon& mu);
};

class EventInfo : public IOEventInfo
{
private:

public:
	EventInfo(const IOEventInfo& evinfo);
	void SetHLT(UInt_t num, bool val);
	bool GetHLT(UInt_t num);
};
}
namespace BASEIORUN
{
class String : public IOString
{
	private:

	public:
		String(const IOString& str);
		void Set(string cppstr);
		string Get();
};
}
namespace BASEIOLUMI
{
class String : public IOString
{
	private:

	public:
		String(const IOString& str);
		void Set(string cppstr);
		string Get();
};
}
