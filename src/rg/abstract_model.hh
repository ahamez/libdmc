// $Author: ahamez $
// $Revision: 682 $
// $Date: 2006-08-22 12:14:18 +0200 (Tue, 22 Aug 2006) $

#ifndef DMC_abstract_model_H_
#define DMC_abstract_model_H_

#include "rg/dmc_state.hh"
#include "rg/abstract_state_succ_iterator.hh"

namespace dmc { namespace rg {

//////////////////////////////////////////////////////////////////////
	
class abstract_model
{
	
// methods
public:
	
	abstract_model(){};
	virtual ~abstract_model(){};
	
	// required from user
	// the model gives the intial state
	virtual dmc_state* get_initial_state() = 0;
	// the model can compute successors of a state
	virtual abstract_state_succ_iterator* get_succ_iterator(dmc_state* s) = 0 ;
	
private:
		
	// cannot copy a model
	abstract_model(const abstract_model&);
	const abstract_model& operator=(const abstract_model&);
	
};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
