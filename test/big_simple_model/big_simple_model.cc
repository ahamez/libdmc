// $Author: ahamez $
// $Revision: 736 $
// $Date: 2006-09-01 15:13:32 +0200 (Fri, 01 Sep 2006) $

#include "big_simple_model/big_simple_model.hh"
#include "big_simple_model/state_succ_iterator.hh"
#include <iostream>
#include <limits>

namespace big_simple_model {

	int big_simple_model::nb_states_ = 0;
	int big_simple_model::state_cost_ = 0;
	
//////////////////////////////////////////////////////////////////////
	
big_simple_model::big_simple_model( int nb_states,
									int state_cost)
{
	big_simple_model::nb_states_ = nb_states;
	big_simple_model::state_cost_ = state_cost;
}
	
//////////////////////////////////////////////////////////////////////
	
rg::dmc_state*
big_simple_model::get_initial_state()
{
	int i = 0;
	return new rg::dmc_state(&i,sizeof(int)); 
}

//////////////////////////////////////////////////////////////////////

char
big_simple_model::get_next_event(state* s , char e)
{	
	if( s->state_coding >= big_simple_model::nb_states_  )
	{
		return NO_MORE_EVENTS;
	}
	else if( e == DUMMY_EVENT )
	{
		return FIRST_EVENT;
	}
	else if( e == LAST_EVENT )
	{
		return NO_MORE_EVENTS;
	}
	else
	{
		return ++e;
	}
}

//////////////////////////////////////////////////////////////////////

state*
big_simple_model::fire(state* s, char e)
{

	for( int i = 0 ; i <= big_simple_model::state_cost_ ; i++ ){}
	int new_state = (s->state_coding) + (e-96);
	
	return new state(new_state);
}

//////////////////////////////////////////////////////////////////////

/// the model can compute successors of a state
dmc::rg::abstract_state_succ_iterator* 
big_simple_model::get_succ_iterator(rg::dmc_state* s) 
{
	return new state_succ_iterator(s);
}

//////////////////////////////////////////////////////////////////////

} // namespace
