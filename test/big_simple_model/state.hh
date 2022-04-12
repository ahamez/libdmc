// $Author: ahamez $
// $Revision: 653 $
// $Date: 2006-08-10 15:03:39 +0200 (Thu, 10 Aug 2006) $

#ifndef BIG_SIMPLE_STATE_H_
#define BIG_SIMPLE_STATE_H_

#include "dmc.hh"

using namespace dmc;

namespace big_simple_model{

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
	
  // overides the one given by dmc
	size_t hash() const;
	
	// required by dmc
// 	bool operator==(const dmc_state& as) const;
// 	void get_content(const void** data , size_t* size) const;
// 	rg::abstract_state_succ_iterator* get_succ_iterator();

};

//////////////////////////////////////////////////////////////////////

} // namespace

#endif
