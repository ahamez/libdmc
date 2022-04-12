// $Author: ahamez $
// $Revision: 1019 $
// $Date: 2008-03-27 10:54:49 +0100 (jeu, 27 mar 2008) $

#ifndef DMC_NEW_STATES_SET_HH_
#define DMC_NEW_STATES_SET_HH_

#include <deque>
#include <pthread.h>
#include <list>

#include "rg/dmc_state.hh"

namespace dmc { namespace rg {

//////////////////////////////////////////////////////////////////////

/// A class implementing producer/consumer semantics with blocking empty
/// pop and blocking full push.

class new_states_set
{

// attributes
private:

	///
	bool empty_;

	///
	mutable pthread_mutex_t mut;

	///
	mutable pthread_cond_t not_empty;

	///
	mutable bool signal_empty_;

	///
	typedef std::deque<rg::dmc_state*> stack_t;
	stack_t stack;


// methods
public:

	//////////////////////////////////////////

	new_states_set()
		:
		empty_(true),
		mut(),
		not_empty(),
		signal_empty_(false),
		stack()
	{
		pthread_mutex_init( &mut, nullptr);
		pthread_cond_init( &not_empty, nullptr);
	}

	//////////////////////////////////////////

	~new_states_set()
	{
		pthread_mutex_destroy(&mut);
		pthread_cond_destroy(&not_empty);
	}

	//////////////////////////////////////////

	inline
	void
	lock() const
	{
		pthread_mutex_lock(&mut);
	}

	//////////////////////////////////////////

	inline
	bool
	is_empty() const
	{
		return stack.empty();
	}

	//////////////////////////////////////////

	inline
	void
	unlock() const
	{
		pthread_mutex_unlock(&mut);
	}

	//////////////////////////////////////////

	inline
	void
	signal_empty() const
	{
		pthread_cond_broadcast(&not_empty);
	}

	inline
	void
	wait_non_empty() const
	{
		if( empty_ )
		{
			pthread_cond_wait( &not_empty,
							   &mut);
		}
	}

	inline
	rg::dmc_state*
	pop_state()
	{
		rg::dmc_state* s = stack.front();
		stack.pop_front();
		return s;
	}

	inline
	void
	push_state(rg::dmc_state* as)
	{
		pthread_mutex_lock(&mut);

 		stack.push_back(as);

		pthread_cond_broadcast(&not_empty);
		pthread_mutex_unlock(&mut);
	}

	inline
	unsigned int
	get_size() const
	{
		return stack.size();
	}

	//////////////////////////////////////////

private:

	// cannot copy states_set
	new_states_set(const new_states_set&);
	new_states_set& operator=(const new_states_set&);

};


//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
