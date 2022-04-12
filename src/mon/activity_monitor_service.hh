// $Author: lbautista $
// $Revision: 1029 $
// $Date: 2008-04-03 16:15:41 +0200 (jeu, 03 avr 2008) $

#ifndef DMC_ACTIVITY_MON_SERVER_HH_
#define DMC_ACTIVITY_MON_SERVER_HH_

#include "mon/abstract_activity_monitor.hh"
#include "conf/configuration.hh"
#include "sal/tcp_server.hh"

namespace dmc { namespace mon {

//////////////////////////////////////////////////////////////////////

class activity_monitor_service
	:
	public dmc::sal::tcp_server
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;
	/// The real monitor to talk to
	mon::abstract_activity_monitor* concrete_;
	///
	pthread_mutex_t* mut_;


// methods
public:

	activity_monitor_service( conf::configuration* conf,
							  mon::abstract_activity_monitor* concrete);
	void handle_connection(int fd) override;

private:

	// cannot copy activity monitor
	activity_monitor_service(const activity_monitor_service&);
	activity_monitor_service& operator=(const activity_monitor_service&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
