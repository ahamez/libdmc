// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (ven, 28 mar 2008) $

#ifndef DMC_FM_PROXY_H_
#define DMC_FM_PROXY_H_

#include "base/abstract_node.hh"
#include "conf/configuration.hh"
#include "sal/tcp_client.hh"

namespace dmc { namespace fm {

//////////////////////////////////////////////////////////////////////

class firing_manager_proxy
	:
	public base::abstract_node,
	public sal::tcp_client
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;


// methods
public:

	firing_manager_proxy( conf::configuration* conf,
						  int node);

	void
	process_initial_state() override;

	void
	start_generation() override;

	void
	stop_generation() override;

	base::status_t
	get_status() override;

	base::stats_t*
	get_stats() override;

private:

	void
	notify_active();

	void
	notify_inactive();

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
