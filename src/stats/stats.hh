// $Author$
// $Revision$
// $Date$

#ifndef DMC_STATS_HH_
#define DMC_STATS_HH_

#include <vector>

#include "conf/configuration.hh"
#include "base/abstract_node.hh"

namespace dmc {

	typedef std::vector<base::abstract_node*> nodes;

namespace stats {

//////////////////////////////////////////////////////////////////////

class stats
{

// attributes
private:

	/// The objet giving informations on the configuration of libdmc
	conf::configuration* conf_;

	/// Start time of generation
	time_t start_time_;

	/// End time of generation
	time_t end_time_;

	/// Count sent states
        tbb::atomic<unsigned long> nb_sent_states_;

	/// Count received states
	tbb::atomic<unsigned long> nb_recv_states_;

	/// The total number of processed states on local node
        tbb::atomic<unsigned long> states_counter_;

	/// The total number of processed successorts on local node
        tbb::atomic<unsigned long> succ_counter_;

	/// Count the number of states sent to a specific node
	std::vector< tbb::atomic<unsigned long> > sent_nodes_;


// methods
public:

	stats(conf::configuration* conf);

	~stats();

	///
	void
	notify_begining_of_time_measurement();

	///
	void
	notify_ending_of_time_measurement();

	///
	unsigned long
	get_time_taken() const;

	///
	void
	generate_stats_file(const nodes& nodes);

	///
	void
	notify_state_sent();

	///
	unsigned long
	get_nb_sent_states() const;

	///
	void
	notify_state_received();

	///
	unsigned long
	get_nb_received_states() const;

	///
	void
	notify_state_processed();

	///
	unsigned long
	get_nb_processed_states() const;

	///
	void
	notify_successor_processed();

	///
	unsigned long
	get_nb_processed_successors() const;

	///
	void
	notify_state_sent_to_node(int node);

	///
	int
	get_nb_states_sent_to_node(int node) const;

private:

	// cannot copy stats
	stats(const stats&);
	stats& operator=(const stats);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
