#include "BASEIO.h"
#include "BASEIORUN.h"
#include "BASEIOLUMI.h"
#include "TLorentzVector.h"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

namespace BASEIO{

class String : public IOString
{
	private:

	public:
		String(const IOString& str);
		void Set(string cppstr);
		string Get();
};

class EventInfo : public IOEventInfo
{
private:
	vector<UChar_t> b;

public:
	EventInfo(const IOEventInfo& evinfo, size_t size);
	void SetHLT(UInt_t num, bool val);
	bool GetHLT(UInt_t num);
	void Write();
};

}
namespace BASEIORUN{

class String : public IOString
{
	private:

	public:
		String(const IOString& str);
		void Set(string cppstr);
		string Get();
};

}

namespace BASEIOLUMI{

class String : public IOString
{
	private:

	public:
		String(const IOString& str);
		void Set(string cppstr);
		string Get();
};

}
