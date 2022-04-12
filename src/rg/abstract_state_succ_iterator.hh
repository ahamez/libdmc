// $Author: ahamez $
// $Revision: 682 $
// $Date: 2006-08-22 12:14:18 +0200 (Tue, 22 Aug 2006) $

#ifndef DMC_ABS_STATE_SUCC_ITER_H_
#define DMC_ABS_STATE_SUCC_ITER_H_

#include "rg/dmc_state.hh"

namespace dmc { namespace rg {

//////////////////////////////////////////////////////////////////////
	
class abstract_state_succ_iterator
{
	
// methods
public:
	
	virtual ~abstract_state_succ_iterator(){};
	
	virtual bool has_successors() = 0;
	virtual dmc_state* get_next_successor() = 0;
};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
