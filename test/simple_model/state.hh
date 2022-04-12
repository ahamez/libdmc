// $Author: ahamez $
// $Revision: 653 $
// $Date: 2006-08-10 15:03:39 +0200 (Thu, 10 Aug 2006) $

#ifndef SIMPLE_STATE_H_
#define SIMPLE_STATE_H_

#include "dmc.hh"

using namespace dmc;

namespace simple_model{

//////////////////////////////////////////////////////////////////////
	
class state // : public rg::dmc_state
{
	
// attributes
public:
	
	int state_coding;
	
	
// methods
public:
	
	state(int i);
	state(void* data,size_t)
		: 
		state_coding( *((int*)data) )
	{};
	~state();
	
	// overides the ones given by dmc
	size_t hash() const;
//	int localization() const;
	
	// required by dmc
//	void get_content(const void** data , size_t* size) const;
//	bool operator==(const dmc_state& as) const;

  friend class state_succ_iterator;
	
};

//////////////////////////////////////////////////////////////////////

} // namespace

#endif
