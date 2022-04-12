// $Author: ahamez $
// $Revision: 1022 $
// $Date: 2008-03-28 22:08:39 +0100 (ven, 28 mar 2008) $

#ifndef DMC_CONFIGURATION_HH_
#define DMC_CONFIGURATION_HH_

#include <vector>
#include <string>
#include <limits>

#include "conf/options.hh"
#include "rg/unicity_table.hh"

#define DMC_MASTER_NODE 0

namespace dmc {  namespace conf {

	/// A host id: < host , port >
	typedef std::pair<std::string,int> host_id;
	/// Host identities table.
	typedef std::vector<host_id*> tab_host_id;

//////////////////////////////////////////////////////////////////////

/// This class has the role to configure the differents settings of the libdmc.

class configuration
{

// attributes
private:

	/// The options from the command line
	conf::options* opt_;

	/// Tells if the generation is distributed or not
	bool distributed_generation_;

	/// Number of nodes
	int nb_nodes_;

	/// Id of the local node
	int node_id_;

	/// Number of threads
	int nb_threads_;

	/// Contains hosts identifications
	tab_host_id tab_host_id_;


public:

    /// table d'unicite
    rg::unicity_table* ut_;



// methods
public:

	configuration(conf::options* opt);
	~configuration();

	/// Returns the number of nodes
	int
	get_nb_nodes()
	{
		return nb_nodes_;
	}

	/// Returns the id of the local node
	int
	get_node_id()
	{
		return node_id_;
	}

	/// Returns the numer of threads on the local node
	int
	get_nb_threads()
	{
		return nb_threads_;
	}

	/// Returns the hostname of a node
	const std::string&
	get_node_hostname(int n)
	{
		return (tab_host_id_[n])->first;
	}

	/// Returns the hostname of the master
	const std::string&
	get_master_hostname()
	{
		return get_node_hostname(DMC_MASTER_NODE);
	}

	/// Tells the caller if the generation is distributed or not
	bool
	generation_is_distributed()
	{
		return distributed_generation_;
	}

	/// Tells the caller if it is the master node
	bool
	local_node_is_master()
	{
		return node_id_ == DMC_MASTER_NODE;
	}

	/// Tells the caller if it is a slave node
	bool
	local_node_is_slave()
	{
		return !local_node_is_master();
	}

	/// Returns the port on which the state_manager service of the node n is listening
	int
	get_sm_server_port(int n)
	{
		return (tab_host_id_[n])->second;
	}

	/// Returns the port on which the local state_manager service is listening
	int
	get_my_sm_server_port()
	{
		return get_sm_server_port(node_id_);
	}

	/// Returns the port on which the firing_manager service of the node n is listening
	int
	get_fm_server_port(int n)
	{
		int max = 0;
		tab_host_id::iterator it;
		for( it = tab_host_id_.begin() ; it != tab_host_id_.end() ; ++it )
		{
			max =  ( (*it)->second > max ? (*it)->second : max );
		}

		return max + n;
	}

	/// Return the port on which the local firing_manger service is listening
	int
	get_my_fm_server_port()
	{
		return get_fm_server_port(node_id_);
	}

	/// Returns the port on which the distributed monitor is listening
	int
	get_monitor_port()
	{
		int min = std::numeric_limits<int>::max();
		tab_host_id::iterator it;
		for( it = tab_host_id_.begin() ; it != tab_host_id_.end() ; ++it )
		{
			min =  ( (*it)->second < min ? (*it)->second : min );
		}

		return min - 1;
	}


private:

	/// Add a < host , port > to tab_host_id_
	void add_node( const std::string & host,
				   int port);

	// cannot copy configuration
	configuration(const configuration&);
	const configuration& operator=(const configuration&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
