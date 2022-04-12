// $Author: lbautista $
// $Revision: 1029 $
// $Date: 2008-04-03 16:15:41 +0200 (jeu, 03 avr 2008) $

#ifndef DMC_SM_SERVER_H_
#define DMC_SM_SERVER_H_

#include "rg/dmc_state.hh"
#include "sm/abstract_state_manager.hh"
#include "conf/configuration.hh"
#include "sal/tcp_server.hh"
#include "stats/stats.hh"

namespace dmc { namespace sm {

//////////////////////////////////////////////////////////////////////

class state_manager_service
	:
	public sal::tcp_server
{

// attributes
private:

	/// Statistics
	stats::stats& stats_;

	/// The object in charge of processing incoming states
	sm::abstract_state_manager* concrete_;


// methods
public:

	state_manager_service( conf::configuration* conf,
						   stats::stats& stats,
						   sm::abstract_state_manager* concrete);

	///
	void
	handle_connection(int fd) override;

private:

	// cannot copy state_manager_service
	state_manager_service( const state_manager_service&);
	state_manager_service& operator=(const state_manager_service&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
