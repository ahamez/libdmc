// $Author: ahamez $
// $Revision: 972 $
// $Date: 2007-06-14 23:26:43 +0200 (jeu, 14 jun 2007) $

#ifndef DMC_ACTIVITY_MON_PROXY_HH_
#define DMC_ACTIVITY_MON_PROXY_HH_

#include "mon/abstract_activity_monitor.hh"
#include "conf/configuration.hh"
#include "sal/tcp_client.hh"

namespace dmc { namespace mon {

//////////////////////////////////////////////////////////////////////

class activity_monitor_proxy
	:
	public mon::abstract_activity_monitor,
	public sal::tcp_client
{

// methods
public:

	activity_monitor_proxy(conf::configuration* conf);
    ~activity_monitor_proxy();

	void
	notify_active() override;

	void
	notify_inactive() override;

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
