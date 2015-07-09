#include "TriggerObject.h"
#include "Analyse.h"


TriggerObject::TriggerObject(UInt_t trigger, const vector<string>& triggernames) : 
trigger_(trigger),
triggernames_(triggernames)
{
}

Int_t TriggerObject::Trigger(string triggername) const
{
	Int_t index = -1;
	for(UInt_t i = 0 ; i < triggernames_.size() ; i++)
	{
		if(triggername == triggernames_[i])
		{
			index = i;
			break;
		}
	}

	if(index == -1) return(0);
	bool result = (trigger_ & 1<<index) != 0;
	size_t pos = triggername.find(":");
	if(pos != string::npos)
	{
		triggername = triggername.substr(0, pos);
	}
	Int_t glindex = GLAN->GetHLTriggerIndex(triggername);
	if(GLAN->GetHLTPrescale(glindex) == 1)
	{
		if(result) return(1);
		else return(-1);
	}
	if(result) return(2);
	else return(-2);
}

