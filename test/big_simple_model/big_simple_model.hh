// $Author: ahamez $
// $Revision: 690 $
// $Date: 2006-08-27 16:10:12 +0200 (Sun, 27 Aug 2006) $

#ifndef BIG_SIMPLE_MODEL_H_
#define BIG_SIMPLE_MODEL_H_

#include "dmc.hh"
#include "big_simple_model/state.hh"

#define NO_MORE_EVENTS '$'
#define DUMMY_EVENT '#'
#define FIRST_EVENT 'a'
#define LAST_EVENT 'e'

namespace big_simple_model {
	
//////////////////////////////////////////////////////////////////////
	
class big_simple_model : public rg::abstract_model
{
	
// attributes
private:
	
	static int nb_states_;
	static int state_cost_;
	
	
// methods
public:
	
	big_simple_model(int nb_states,
					 int state_cost);
	
	rg::dmc_state* get_initial_state();
        /// the model can compute successors of a state
	dmc::rg::abstract_state_succ_iterator* get_succ_iterator(rg::dmc_state* s) ;

	static state* fire(state* s , char e);
	static char get_next_event(state* s , char e );
};

//////////////////////////////////////////////////////////////////////

} // namespace model


#endif
