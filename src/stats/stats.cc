// $Author$
// $Revision$
// $Date$

#include <fstream>
#include <iomanip>

#include "stats/stats.hh"
#include "base/cluster_manager.hh"

namespace dmc { namespace stats {

//////////////////////////////////////////////////////////////////////

stats::stats(conf::configuration* conf)
	:
	conf_(conf),
    	nb_sent_states_(),
	nb_recv_states_(),
	states_counter_(),
	succ_counter_(),
        sent_nodes_( conf_->get_nb_nodes()) // initialize the vector with counter set to 0
{
	nb_sent_states_ = 0;
	nb_recv_states_ = 0;
	states_counter_ = 0;
	succ_counter_ = 0;
}

//////////////////////////////////////////////////////////////////////

stats::~stats()
{
}

//////////////////////////////////////////////////////////////////////

void
stats::notify_begining_of_time_measurement()
{
	start_time_ = time(nullptr);
}

//////////////////////////////////////////////////////////////////////

void
stats::notify_ending_of_time_measurement()
{
	end_time_ = time(nullptr);
}

//////////////////////////////////////////////////////////////////////

unsigned long
stats::get_time_taken() const
{
	return difftime(end_time_,start_time_);
}

//////////////////////////////////////////////////////////////////////

void
stats::generate_stats_file(const nodes& nodes)
{
	std::ofstream stats("stats");
	nodes::const_iterator it;
	const int width = 20;
	unsigned int n;
	unsigned long states_sum = 0;
	unsigned long succ_sum = 0;
	unsigned long sent_states = 0;
	unsigned long recv_states = 0;

	if( conf_->generation_is_distributed() )
	{

		stats
			<< std::endl
			<< "Generation with " << conf_->get_nb_nodes()
			<< " nodes" << std::endl << std::endl
			<< std::setfill('-') <<  std::setw(width*4+5) << "-" << std::endl << std::setfill(' ')
			<< std::endl ;

		stats
			<< std::setw(5) << "Node"
			<< std::setw(width) << "States"
			<< std::setw(width) << "Processed"
			<< std::setw(width) << "Sents"
			<< std::setw(width) << "Received"
			<< std::endl << std::endl;

		for( n = 0 , it = nodes.begin() ; it != nodes.end() ; ++n , ++it )
		{
			base::stats_t* tmp_stats = (*it)->get_stats();
			base::status_t tmp_status = (*it)->get_status();

			stats
				<< std::setw(5) << n
				<< std::setw(width) << tmp_stats->nb_states
				<< std::setw(width) << tmp_stats->nb_succ
				<< std::setw(width) << tmp_status.nb_sent
				<< std::setw(width) << tmp_status.nb_recv
				<< std::endl;

			states_sum += tmp_stats->nb_states;
			succ_sum += tmp_stats->nb_succ;
			sent_states += tmp_status.nb_sent;
			recv_states += tmp_status.nb_recv;

			delete[] tmp_stats->sent_nodes;
			delete tmp_stats;
		}

		stats
			<< std::endl
			<< std::setfill('-') <<  std::setw(width*4+5) << "-" << std::endl << std::setfill(' ')
			<< std::endl
			<< "Total number of states: " << states_sum
			<< std::endl
			<< "Total number of successors: " << succ_sum
			<< std::endl << std::endl;

		stats
			<< "Total number of sent states: " << sent_states
			<< std::endl
			<< "Total number of received states: " << recv_states
			<< std::endl;
	}
	else // local generation
	{
		stats
			<< "Total number of states: " <<  conf_->ut_->get_size() //states_counter_.get_counter()
			<< std::endl
			<< "Total number of successors: " << succ_counter_;
	}

	stats
		<< std::endl
		<< std::setfill('-') <<  std::setw(width*4+5) << "-" << std::endl << std::setfill(' ')
		<< std::endl
		<< "Time taken: " << difftime(end_time_,start_time_) << "s"
		<< std::endl;

	if( conf_->generation_is_distributed() )
	{
		std::ofstream matrix_("nodeXnode");

		matrix_ << "\t";

		for( int i = 0 ; i < conf_->get_nb_nodes() ; ++i )
		{
			matrix_
				<< i
				<< "\t";
		}

		matrix_ << std::endl;

		for( n = 0 , it = nodes.begin() ; it != nodes.end() ; ++n , ++it )
		{
			base::stats_t* tmp_stats = (*it)->get_stats();

			matrix_
				<< n
				<< "\t";

			for( int i = 0 ; i < conf_->get_nb_nodes() ; ++i )
			{
				matrix_
					<< tmp_stats->sent_nodes[i]
					<< "\t";
			}

			matrix_ << std::endl;

			delete[] tmp_stats->sent_nodes;
			delete tmp_stats;
		}
	}
}

//////////////////////////////////////////////////////////////////////

void
stats::notify_state_sent()
{
	++nb_sent_states_;
}

//////////////////////////////////////////////////////////////////////

unsigned long
stats::get_nb_sent_states() const
{
	return nb_sent_states_;
}

//////////////////////////////////////////////////////////////////////

void
stats::notify_state_received()
{
	++nb_recv_states_;
}

//////////////////////////////////////////////////////////////////////

unsigned long
stats::get_nb_received_states() const
{
	return nb_recv_states_;
}

//////////////////////////////////////////////////////////////////////

void
stats::notify_state_processed()
{
	++states_counter_;
}

//////////////////////////////////////////////////////////////////////

unsigned long
stats::get_nb_processed_states() const
{
	return states_counter_;
}

//////////////////////////////////////////////////////////////////////

void
stats::notify_successor_processed()
{
	++succ_counter_;
}

//////////////////////////////////////////////////////////////////////

unsigned long
stats::get_nb_processed_successors() const
{
	return succ_counter_;
}

//////////////////////////////////////////////////////////////////////

void
stats::notify_state_sent_to_node(int node)
{
	++sent_nodes_[node];
}

//////////////////////////////////////////////////////////////////////

int
stats::get_nb_states_sent_to_node(int node) const
{
	return sent_nodes_[node];
}

//////////////////////////////////////////////////////////////////////

}} // namespaces
