#ifndef MCMatchable_H
#define MCMatchable_H
#include "GenObject.h"

class MCMatchable {
private:
	const GenObject* match_=0;
public:
	MCMatchable(){};
	MCMatchable(const GenObject* m):
		match_(m)
	{};
	const GenObject* match() const {return match_;}
	void addMatch(const GenObject* m) {match_ = m;}
};

#endif
