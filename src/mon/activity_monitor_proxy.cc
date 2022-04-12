// $Author: ahamez $
// $Revision: 972 $
// $Date: 2007-06-14 23:26:43 +0200 (jeu, 14 jun 2007) $

#include "activity_monitor_proxy.hh"

#include <netinet/in.h>

namespace dmc { namespace mon {

//////////////////////////////////////////////////////////////////////

activity_monitor_proxy::activity_monitor_proxy(conf::configuration* conf)
    :
	sal::tcp_client( conf->get_node_hostname(DMC_MASTER_NODE),
	conf->get_monitor_port())
{
}

//////////////////////////////////////////////////////////////////////

activity_monitor_proxy::~activity_monitor_proxy()
{
	this->disconnect();
}

//////////////////////////////////////////////////////////////////////

void
activity_monitor_proxy::notify_active()
{
	int msg = 0;
	send_data(&msg,sizeof(int));
}

//////////////////////////////////////////////////////////////////////

void
activity_monitor_proxy::notify_inactive()
{
	int msg = 1;
	send_data(&msg,sizeof(int));
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
