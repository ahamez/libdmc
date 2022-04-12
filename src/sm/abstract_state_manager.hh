// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (Fri, 28 Mar 2008) $

#ifndef DMC_SM_ITF_H_
#define DMC_SM_ITF_H_

#include "rg/dmc_state.hh"


namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

/// \brief Interface of the components which are in charge of processing states
class abstract_state_manager
{
	
// methods
public :
		
	virtual 
	~abstract_state_manager()
	{
	};	
	
	virtual 
	void 
	process_state(rg::dmc_state* as) = 0;
	
};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
