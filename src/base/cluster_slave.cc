// $Author: ahamez $
// $Revision: 752 $
// $Date: 2006-08-21 16:50:08 +0200 (Mon, 21 Aug 2006) $

#include "base/cluster_slave.hh"
#include "base/node_manager.hh"
#include "fm/firing_manager_proxy.hh"

#include <functional>

namespace dmc { namespace base {

//////////////////////////////////////////////////////////////////////

cluster_slave::cluster_slave( conf::configuration* conf,
							  stats::stats& stats,
							  rg::abstract_model_factory* model_fact)
	:
	conf_(conf),
	stats_(stats),
	local_node_(),
	fm_server_(),
	bam_proxy_(),
	nss_()
{
	bam_proxy_ = new mon::activity_monitor_proxy(conf);

	local_node_ = new node_manager( conf,
									stats_,
									model_fact,bam_proxy_,
									&nss_);

	fm_server_ = new fm::firing_manager_service( conf_,
												  stats,
												  local_node_,
												  &nss_);

}

//////////////////////////////////////////////////////////////////////

cluster_slave::~cluster_slave()
{
	delete bam_proxy_;
	fm_server_->stop_server();
	delete fm_server_;
	delete local_node_;
}

//////////////////////////////////////////////////////////////////////

void
cluster_slave::process_initial_state()
{
}

//////////////////////////////////////////////////////////////////////

// in main thread
void
cluster_slave::start_generation()
{
	// when this call is ended, the slave node has finished its works
	fm_server_->start_server();
}

//////////////////////////////////////////////////////////////////////

void
cluster_slave::stop_generation()
{
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
