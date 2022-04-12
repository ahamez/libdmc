// $Author: lbautista $
// $Revision: 1029 $
// $Date: 2008-04-03 16:15:41 +0200 (jeu, 03 avr 2008) $

#ifndef DMC_FM_SERVICE_H_
#define DMC_FM_SERVICE_H_

#include "fm/abstract_firing_manager.hh"
#include "rg/new_states_set.hh"
#include "conf/configuration.hh"
#include "sal/tcp_server.hh"
#include "stats/stats.hh"

namespace dmc { namespace fm {

//////////////////////////////////////////////////////////////////////

class firing_manager_service
	:
	public sal::tcp_server
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	/// Statistics
	stats::stats& stats_;

	/// The object which is really in charge of tasks asked to this class
	fm::abstract_firing_manager* concrete_;

	/// The set of new states to process
	rg::new_states_set* nss_;


// methods
public:

	firing_manager_service( conf::configuration* conf,
							stats::stats& stats,
							fm::abstract_firing_manager* concrete,
							rg::new_states_set* nss);

	void
	handle_connection(int fd) override;

private:

	// cannot copy firing_manager_service
	firing_manager_service(const firing_manager_service&);
	firing_manager_service& operator=(const firing_manager_service&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
