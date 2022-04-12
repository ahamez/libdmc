// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (ven, 28 mar 2008) $

#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include "firing_manager_service.hh"
#include "firing_manager_protocol.hh"

namespace dmc { namespace fm {

//////////////////////////////////////////////////////////////////////

firing_manager_service::firing_manager_service( conf::configuration* conf,
												stats::stats& stats,
												fm::abstract_firing_manager* concrete,
												rg::new_states_set* nss)
	:
	sal::tcp_server( conf->get_my_fm_server_port(),
					  true,true),  // no threads created
	conf_(conf),
	stats_(stats),
	concrete_(concrete),
	nss_(nss)
{
}

//////////////////////////////////////////////////////////////////////

void
firing_manager_service::handle_connection(int fd)
{
	bool asked_to_stop = false;

    while (true)
	{
		int msg;
		int nb;

		if( (nb = recv(fd, (char*)&msg, sizeof(int),0)) == -1 )
		{
			perror("[fms read]");
			exit(1);
		}

		// test if the socket was closed
		if( nb == 0 )
		{
			break;
		}

		switch (msg)
		{
			case DMC_PROCESS_INITIAL:
			{
				// could read more info (i.e. msg sent recvd)
				concrete_->process_initial_state();
				break;
			}

			case  DMC_GENERATE:
			{
				concrete_->start_generation();
				break;
			}

			case  DMC_STOP:
			{
				asked_to_stop = true;
				concrete_->stop_generation();
				break;
			}

			case DMC_STATUS:
			{
				unsigned long sent = stats_.get_nb_sent_states();
				unsigned long recv = stats_.get_nb_received_states();
				nss_->lock();
				bool tmp = nss_->is_empty();
				nss_->unlock();

				::send( fd, &tmp, sizeof(bool),0);
				::send( fd, &sent, sizeof(unsigned long),0);
				::send( fd, &recv, sizeof(unsigned long),0);

				break;
			}

			case DMC_STATS:
			{
				unsigned int size = nss_->get_size();
				unsigned long nb_states = conf_->ut_->get_size();
				unsigned long nb_succ = stats_.get_nb_processed_successors();
				auto   sent_nodes = new unsigned long[conf_->get_nb_nodes()];

				for( int i = 0 ; i < conf_->get_nb_nodes() ; ++ i )
				{
					sent_nodes[i] = stats_.get_nb_states_sent_to_node(i);
				}

				::send( fd, &size, sizeof(unsigned int),0);
				::send( fd, &nb_states, sizeof(unsigned long),0);
				::send( fd, &nb_succ, sizeof(unsigned long),0);

				send_data( sent_nodes,
						   sizeof(unsigned long) * conf_->get_nb_nodes(),
						   fd );

				break;
			}

			default :
			{
				exit(EXIT_FAILURE);
				break;
			}
		}
	}

	if( !asked_to_stop )
	{
		concrete_->stop_generation();
	}
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
