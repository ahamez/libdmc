// $Author: ahamez $
// $Revision: 653 $
// $Date: 2006-08-10 15:03:39 +0200 (Thu, 10 Aug 2006) $

#ifndef SIMPLE_STATE_SUCC_ITER_H_
#define SIMPLE_STATE_SUCC_ITER_H_

#include "dmc.hh"
#include "simple_model/state.hh"

using namespace dmc;

namespace simple_model{
	
//////////////////////////////////////////////////////////////////////
	
class state_succ_iterator : public rg::abstract_state_succ_iterator
{
	
// attributes
private:
	
	state root_state;
	char next_event;
	
	
// methods
public:

  state_succ_iterator(dmc::rg::dmc_state* s);
		
	bool has_successors();
	rg::dmc_state* get_next_successor();
	
};


//////////////////////////////////////////////////////////////////////

} // namespace


#endif
