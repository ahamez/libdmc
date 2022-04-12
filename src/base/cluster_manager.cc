// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#include <functional>

#include "base/cluster_manager.hh"
#include "base/node_manager.hh"
#include "rg/new_states_set.hh"
#include "fm/firing_manager_proxy.hh"

namespace dmc { namespace base {

//////////////////////////////////////////////////////////////////////

cluster_manager::cluster_manager( conf::configuration* conf,
								  stats::stats& stats,
								  rg::abstract_model_factory* model_fact)
	:
	conf_(conf),
	stats_(stats),
	fm_(),
	nodes_(),
	bam_(),
	bam_server_(),
    model_fact_(model_fact),
	nss_()
{
	bam_ = new mon::barrier_integer_activity_monitor(0);

	if( conf_->generation_is_distributed() )
	{
		bam_server_ = new mon::activity_monitor_service(conf,bam_);
		bam_server_->start_server();
	}

	// whatever is the type of generation, we need a local generation
	fm_ = new node_manager( conf_,
							stats_,
                            model_fact,bam_,
                            &nss_);

	if( conf_->generation_is_distributed() )
    {
		// nodes
	    for( int n = 0 ; n < conf_->get_nb_nodes() ; ++n )
        {
            if( n != conf_->get_node_id() )
            {
		    	// a distant node_manager
                nodes_.push_back( new fm::firing_manager_proxy(conf_,n) );
            }
            else
            {
				// the local node_manager
				nodes_.push_back(fm_);
            }
        }

    }
}

//////////////////////////////////////////////////////////////////////

cluster_manager::~cluster_manager()
{
	if( conf_->generation_is_distributed() )
    {
		for(auto & elem : nodes_)
		{
			if( (elem) != fm_ )
			{
				delete (elem);
			}
		}

		bam_server_->stop_server();
		delete bam_server_;

    }

	delete bam_;
	delete fm_;
}

//////////////////////////////////////////////////////////////////////

void
cluster_manager::process_initial_state()
{
	// even when the generation is distributed, only one node computes the successors
	// of the initial state, otherwise the owner node will be flooded
	fm_->process_initial_state();
}

//////////////////////////////////////////////////////////////////////

// in main thread
void
cluster_manager::start_generation()
{
	// mark the begining of the generation
	stats_.notify_begining_of_time_measurement();

	// ask everybody to start generation
	process(std::mem_fun(&abstract_firing_manager::start_generation));

	if( conf_->generation_is_distributed() )
	{
		while( true )
		{
			// pass through the following call if inactivity has taken at least
			// a fixed ammount of time
			bam_->timed_wait_until_inactive(1,0);

			// every nodes said they are inactive, let's verify this
			if( generation_has_ended() )
			{
				break;
			}
		}
	}
	else
	{
		bam_->wait_until_inactive();
	}

	// mark the ending of the generation
	stats_.notify_ending_of_time_measurement();

	// will generate a file called 'stats'
    stats_.generate_stats_file(nodes_);

	// how many time did we take?
	std::cout
		<< "Time taken: "
		<< 	stats_.get_time_taken()
		<< "s"
		<< std::endl;

	// ask everybody to stop generation
	process(std::mem_fun(&abstract_firing_manager::stop_generation));
}

//////////////////////////////////////////////////////////////////////

void
cluster_manager::stop_generation()
{
	process(std::mem_fun(&abstract_firing_manager::stop_generation));
}

//////////////////////////////////////////////////////////////////////

template <typename T>
void
cluster_manager::process(const T& x)
{
	if( conf_->generation_is_distributed() )
	{
		for( nodes::const_iterator it = nodes_.begin() ;
			 it != nodes_.end() ;
			 ++it )
		{
			x(*it);
		}
	}
	else
	{
		x(fm_);
	}
}

//////////////////////////////////////////////////////////////////////

/// Termination detection.
/// If all messages sent have been received and all new_states_set are empty,
/// then the generation is over

bool
cluster_manager::generation_has_ended()
{
	unsigned long sum_sent = 0;
	unsigned long sum_recv = 0;

	nodes::const_iterator it;
	for( it = nodes_.begin() ; it != nodes_.end() ; ++it )
	{
		base::status_t tmp = (*it)->get_status();

		if( !tmp.empty_set )
		{
			// if anyone has a non-empty new_states_set, then generation is not ended
			return false;
		}

		sum_sent += tmp.nb_sent;
		sum_recv += tmp.nb_recv;
	}

	return ( sum_sent - sum_recv ) == 0;
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
