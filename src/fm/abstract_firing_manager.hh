// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (Fri, 28 Mar 2008) $

#ifndef DMC_ASBTRACT_FM_HH_
#define DMC_ASBTRACT_FM_HH_

#include <utility>
#include <vector>

#include "rg/dmc_state.hh"

namespace dmc { namespace fm {
		
//////////////////////////////////////////////////////////////////////

class abstract_firing_manager 
{

// methods
public:	

	virtual
	~abstract_firing_manager()
	{
	};	

	/// call to ask fm to process the model initial state.
	virtual 
	void 
	process_initial_state() = 0;

	/// Non-blocking, starts a state generation loop in one or many threads.
	/// Usually creates threads if they don't exist yet, otherwise just unblocks them.
	virtual 
	void 
	start_generation() = 0;

	/// Stop the current generation
	/// Has to be blocking while the generation is no stopped
	virtual 
	void 
	stop_generation() = 0;
	
};
       
//////////////////////////////////////////////////////////////////////
	
}} // namespaces

#endif
