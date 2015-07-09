#ifndef CTRIGGEROBJECT
#define CTRIGGEROBJECT

#include <vector>
#include <string>

#include <Rtypes.h>

class Analyse;

using namespace std;

class TriggerObject
{
	private:
		UInt_t trigger_;
		const vector<string>& triggernames_;
	public:
		TriggerObject() = delete;
		TriggerObject(UInt_t trigger, const vector<string>& triggernames);
		Int_t Trigger(string name) const; //0: Trigger not available or matched. 1 not prescale fired, -1 not prescaled not fired, 2 prescaled fired, -2 prescalded not fired 
		const vector<string>& TriggerNames() const {return(triggernames_);} 
};


#endif
