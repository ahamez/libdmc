// $Author: ahamez $
// $Revision: 629 $
// $Date: 2006-08-07 15:23:09 +0200 (Mon, 07 Aug 2006) $

#include "simple_model/state_succ_iterator.hh"
#include "simple_model/simple_model.hh"

#include <iostream>

namespace simple_model{

	
//////////////////////////////////////////////////////////////////////
	
  state_succ_iterator::state_succ_iterator(dmc::rg::dmc_state* s)
	:
    root_state(*((int*) s->data))
{
	next_event = simple_model::get_next_event(&root_state,FIRST_EVENT);
}

//////////////////////////////////////////////////////////////////////

bool
state_succ_iterator::has_successors()
{
	return next_event != NO_MORE_EVENTS;
}

//////////////////////////////////////////////////////////////////////

rg::dmc_state* 
state_succ_iterator::get_next_successor()
{
	state* tmp = simple_model::fire(&root_state,next_event);
	next_event = simple_model::get_next_event(&root_state,next_event);
	rg::dmc_state * ttmp = new rg::dmc_state(&tmp->state_coding, sizeof(int));
	delete tmp;
	return ttmp;
}

//////////////////////////////////////////////////////////////////////

} // namespace
