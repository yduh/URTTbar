#ifndef MCMatchable_H
#define MCMatchable_H
#include "URStreamer.h"

class MCMatchable {
private:
	const Genparticle* match_=0;
public:
	MCMatchable(){};
	MCMatchable(const Genparticle* m):
		match_(m)
	{};
	const Genparticle* match() const {return match_;}
	void addMatch(const Genparticle* m) {match_ = m;}
};

#endif
