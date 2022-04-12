// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_SM_H_
#define DMC_SM_H_

#include "rg/new_states_set.hh"
#include "rg/dmc_state.hh"
#include "sm/abstract_state_manager.hh"
#include "rg/unicity_table.hh"
#include "conf/configuration.hh"

namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

/// A concrete local implementation of abstract_state_manager, using a unicity table.
/// Initialize with the node's firing_state_set.

class state_manager
	:
	public abstract_state_manager
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	///
	rg::new_states_set* nss_;

	///
	rg::unicity_table ut_;


// methods
public:

	state_manager( conf::configuration* conf,
                   rg::new_states_set* nss);

	void
	process_state(rg::dmc_state* as) override;

	// cannot copy state_manager
	state_manager(const state_manager&);
	state_manager& operator=(const state_manager&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
