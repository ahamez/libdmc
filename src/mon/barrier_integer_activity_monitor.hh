// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_BARRIER_INTEGERACTIVITY_MONITOR_HH_
#define DMC_BARRIER_INTEGERACTIVITY_MONITOR_HH_

#include <pthread.h>

#include "mon/abstract_activity_monitor.hh"
#include "mon/barrier_integer_condition.hh"

namespace dmc { namespace mon {

/////////////////////////////////////////////////////////////////////

/// A class to have barrier semantics on a monitor.
/// Implemented through a barrier_integer_condition.
/// notify_(in)active lets callers through the wait_until_(in)active
/// Example : a thread manager calls t.start, then wait_until_inactive on the monitor of t
/// , then t.cancel()

class barrier_integer_activity_monitor
	:
	public mon::abstract_activity_monitor,
	private mon::barrier_integer_condition
{

// methods
public:

	/// Gives initial activity state.
	barrier_integer_activity_monitor(int nb_thread)
		:
		mon::barrier_integer_condition(nb_thread)
	{}

	void notify_active() override
	{
		add_to_value(1);
	}

	void notify_inactive() override
	{
		add_to_value(-1);
	}

	void set_inactive()
	{
		set_value(0);
	}

	virtual bool is_active()
	{
		return get_value() > 0;
	}

	void wait_until_active ()
	{
		wait_until_gt(0);
	}

	void wait_until_inactive()
	{
		wait_until(0);
	}

	void timed_wait_until_inactive(time_t s,long ns)
	{
		struct timespec t;
		t.tv_sec = s;
		t.tv_nsec = ns;

		while(true)
		{
			wait_until(0);
			nanosleep(&t,nullptr);
			if( ! is_active() )
			{
				break;
			}
		}
	}

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
