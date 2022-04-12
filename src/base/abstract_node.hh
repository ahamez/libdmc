// $Author: lbautista $
// $Revision: 1029 $
// $Date: 2008-04-03 16:15:41 +0200 (Thu, 03 Apr 2008) $

#ifndef DMC_ABSTRACT_NODE_HH_
#define DMC_ABSTRACT_NODE_HH_

#include "fm/abstract_firing_manager.hh"

namespace dmc { namespace base {

        typedef unsigned long* counter_set;

	typedef struct
	{
		bool empty_set;
		unsigned long nb_sent;
		unsigned long nb_recv;
	} status_t;

	typedef struct
	{
		unsigned int set_size;
		unsigned long nb_states;
		unsigned long nb_succ;
		counter_set sent_nodes;
	} stats_t;

//////////////////////////////////////////////////////////////////////

class abstract_node
	:
	public fm::abstract_firing_manager
{

// methods
public:

	virtual
	~abstract_node()
	{
	};

	///
	virtual
	status_t
	get_status() = 0;

	///
	virtual
	stats_t*
	get_stats() = 0;

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
