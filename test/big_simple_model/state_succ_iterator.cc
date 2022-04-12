// $Author: ahamez $
// $Revision: 961 $
// $Date: 2007-05-31 16:59:36 +0200 (Thu, 31 May 2007) $

#include <iostream>

#include "big_simple_model/state_succ_iterator.hh"
#include "big_simple_model/big_simple_model.hh"

namespace big_simple_model{

//////////////////////////////////////////////////////////////////////

state_succ_iterator::state_succ_iterator(dmc::rg::dmc_state* s)
	:
	root_state(0)
{
	if( s == NULL)
	{
		std::cerr << pthread_self()  << " ERROR:state is NULL" << std::endl;
		exit(EXIT_FAILURE);
	}

	root_state = *((int*)s->data);
	next_event = big_simple_model::get_next_event(&root_state,DUMMY_EVENT);
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
	state* tmp = big_simple_model::fire(&root_state,next_event);
	next_event = big_simple_model::get_next_event(&root_state,next_event);
	rg::dmc_state * ttmp = new rg::dmc_state(&tmp->state_coding, sizeof(int));
	delete tmp;
	return ttmp;
}

//////////////////////////////////////////////////////////////////////

} // namespace
