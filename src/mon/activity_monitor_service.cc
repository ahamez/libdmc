// $Author: ahamez $
// $Revision: 960 $
// $Date: 2007-05-31 16:03:48 +0200 (jeu, 31 mai 2007) $

#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>

#include "activity_monitor_service.hh"
#include "mon/barrier_integer_activity_monitor.hh"
#include "base/node_manager.hh"

namespace dmc { namespace mon {

//////////////////////////////////////////////////////////////////////

activity_monitor_service::activity_monitor_service( conf::configuration* conf,
													mon::abstract_activity_monitor* concrete)
	:
	sal::tcp_server(conf->get_monitor_port()),
	conf_(conf),
	concrete_(concrete)
{
}

//////////////////////////////////////////////////////////////////////

void
activity_monitor_service::handle_connection(int fd)
{
    while(true)
	{
		int msg;
		int ret;

		if( ( ret = ::recv( fd, &msg , sizeof(int), 0 )) == -1 )
		{
			perror("[ams read]");
			exit(1);
		}

		if( ret == 0 ) // ie. socket closed
		{
			break;
		}

		switch(msg)
		{
			case 0: // activity notification
			{
				concrete_->notify_active();
				break;
			}

			case 1: // inactivity notification
			{
				concrete_->notify_inactive();
				break;
			}

			default: // error
			{
				exit(EXIT_FAILURE);
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
