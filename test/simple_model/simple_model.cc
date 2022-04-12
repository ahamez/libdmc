// $Author: ahamez $
// $Revision: 996 $
// $Date: 2007-06-26 20:55:06 +0200 (Tue, 26 Jun 2007) $

#include "simple_model/simple_model.hh"
#include "simple_model/state_succ_iterator.hh"
#include <iostream>

namespace simple_model {

	int simple_model::cost_ = 1;
	bool simple_model::verbose_ = false;
	
//////////////////////////////////////////////////////////////////////
	
simple_model::simple_model( int cost, bool verbose)
{
	cost_ = cost;
	verbose_ = verbose;
}

	
//////////////////////////////////////////////////////////////////////
	
rg::dmc_state*
simple_model::get_initial_state()
{
  int i = 0;
  return new rg::dmc_state(&i,sizeof(int)); 
}

//////////////////////////////////////////////////////////////////////

char
simple_model::get_next_event(state* s,char e)
{
	switch ( s->state_coding ) 
	{
		
		case 0:
			switch(e)
			{
				case FIRST_EVENT: 
					return 'a';
					break;
				
				case 'a': 
					return 'b' ;
					break;

				case 'b' : 
					return 'c' ; 
					break;
				
				default : 
					return NO_MORE_EVENTS ;
			}
			break;
	
		case 4:
			switch(e)
			{
				case FIRST_EVENT: 
					return 'd'; 
					break;
				
				case 'd': 
					return 'e'; 
					break;
				
				default: 
					return NO_MORE_EVENTS; 
					break;
			}
			break;
		
		case 5:
			return NO_MORE_EVENTS; 
			break;
		
		case 6:
			switch(e)
			{
				case FIRST_EVENT:
					return 'f';
					break;
					
				case 'f':
					return 'g';
					break;
					
				default:
					return NO_MORE_EVENTS;
					break;
			}
			break;
		
		case 7:
			return NO_MORE_EVENTS;
			break;
		
		default:
			switch (e) 
			{
				case FIRST_EVENT: 
					return SINGLE_EVENT;	
					break;
				
				default : 
					return NO_MORE_EVENTS ; 
					break ;
			}
			break;
	}
}


//////////////////////////////////////////////////////////////////////

state*
simple_model::fire(state* s, char e)
{
	int new_state = 0;
	
	// to simulate an heavy transition firing
	for( int i=0 ; i<= cost_ ; i++ ){}
	
	switch( s->state_coding )
	{
		
		case 0:
			switch(e)
			{
				case 'a':
					new_state = 1;
					break;
				case 'b':
					new_state = 2;
					break;
				case 'c':
					new_state = 3;
					break;
				default:
					break;
			}
			break;
			
		case 1:
			new_state = 0;
			break;
			
		case 2:
			new_state = 4;
			break;
			
		case 3:
			new_state = 4;
			break;
			
		case 4:
			switch(e) 
			{
				case 'd':
					new_state = 5;
					break;
				case 'e':
					new_state = 6;
					break;
				default:
					break;
			}
			break;
			
		case 6:
			switch(e)
			{
				case 'f':
					new_state = 3;
					break;
				case 'g':
					new_state = 7;
					break;
			}
			break;
			
		default:
			break;
	}
	
	if( simple_model::verbose_ == true )
	{
		std::cout 
			<< "State " << s->state_coding 
			<< " with event " << e 
			<< " to state " << new_state 
			<< std::endl;
	}
	return new state(new_state);	
}

//////////////////////////////////////////////////////////////////////

// the model can compute successors of a state
dmc::rg::abstract_state_succ_iterator* 
simple_model::get_succ_iterator(rg::dmc_state* s) 
{
  return new state_succ_iterator(s);
}

//////////////////////////////////////////////////////////////////////

} // namespace
