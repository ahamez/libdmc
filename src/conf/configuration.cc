// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (ven, 28 mar 2008) $

#include <cstdlib>
#include <unistd.h>
#include <fcntl.h>

#include "configuration.hh"

// not so ugly, because these files are generated
// by the deployment script
#define DMC_NODE_CONF_FILE "node.conf"
#define DMC_HOSTS_FILE "global.conf"
#define DMC_MAX_LINE_SIZE 4096

namespace dmc { namespace conf {

//////////////////////////////////////////////////////////////////////

configuration::configuration(conf::options* opt)
	:
	opt_(opt),
	distributed_generation_(false),
	nb_nodes_(1),
	node_id_(DMC_MASTER_NODE),
	nb_threads_(),
	tab_host_id_()
{
	if( opt == nullptr )
	{
		exit(EXIT_SUCCESS);
	}

	if( access( DMC_NODE_CONF_FILE, F_OK) == 0
		&& !opt->force_local_)
	{
		distributed_generation_ = true;

		////////////////////////////
		// distributed generation //
		////////////////////////////

		// read the configuration of the local node
		FILE* conf_file = fopen( DMC_NODE_CONF_FILE, "r");

		fscanf( conf_file,
				"%d %d %d",
				&nb_nodes_,
				&node_id_,
				&nb_threads_);

		fclose(conf_file);

		// std::cerr << "Number of nodes: " << nb_nodes_  << std::endl
		// 	<< "Local id: " << node_id_ << std::endl;

		// construct the list of pair <ip/port>
		if ( access( DMC_HOSTS_FILE, F_OK) != 0 )
		{
			exit(EXIT_FAILURE);
		}
		else
		{
			if( nb_nodes_ != 1 )
			{

				tab_host_id_.reserve(nb_nodes_);

				conf_file = fopen(DMC_HOSTS_FILE, "r");
				char line[DMC_MAX_LINE_SIZE];
				char host[DMC_MAX_LINE_SIZE];
				int port;
				int nodes_cpt = 0;
				fgets( line, DMC_MAX_LINE_SIZE, conf_file );
				do
				{
					sscanf( line,
							"%s %d %d",
							host,
							&port,
                            &nb_threads_);

					add_node( host, port);
					fgets( line, DMC_MAX_LINE_SIZE, conf_file );
					++nodes_cpt;
				}
				while( !feof(conf_file) && nodes_cpt < nb_nodes_ );
				fclose(conf_file);
			}
			else
			{
				distributed_generation_ = false;
			}
		}
	}
	else
	{
		////////////////////////////
		//    local generation    //
		////////////////////////////

		distributed_generation_ = false;
	}

	if( opt_->nb_threads_ != 0 )
	{
		nb_threads_ = opt->nb_threads_;
	}

}

//////////////////////////////////////////////////////////////////////

configuration::~configuration()
{

	tab_host_id::iterator it;
	for( it = tab_host_id_.begin();
		 it != tab_host_id_.end();
		 ++it)
	{
		if( *it != nullptr )
		{
			delete *it;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void
configuration::add_node( const std::string& host,
						 int port)
{
	tab_host_id_.push_back(new host_id(host,port));
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
