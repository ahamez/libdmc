// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include "state_manager.hh"

namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

state_manager::state_manager(	conf::configuration* conf,
								rg::new_states_set* nss)
	:
	conf_(conf),
	nss_(nss),
	ut_()
{
	// so ugly...
	conf_->ut_ = &ut_;
}

//////////////////////////////////////////////////////////////////////

void
state_manager::process_state(rg::dmc_state* as)
{
        rg::state_state ss = ut_.insert(as);

	if( ss == rg::NOT_NEW_S )
	{
		delete as;
	}
	else
	{
		nss_->push_state(as);
	}
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
