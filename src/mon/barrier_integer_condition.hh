// $Author: lbautista $
// $Revision: 1036 $
// $Date: 2008-04-14 17:10:01 +0200 (Mon, 14 Apr 2008) $

#ifndef DMC_BARRIER_INT_CONDITION_HH_
#define DMC_BARRIER_INT_CONDITION_HH_

namespace dmc { namespace mon {

/////////////////////////////////////////////////////////////////////

/// A class to have barrier semantics on a condition.
/// Implements the basic condition interface.
/// incr() increments and notifies callers blocked on the wait_until
/// decr() decrements and notifies
/// int get() allows to poll (non-blocking) the state of the condition
/// wait_until(int) allows non active wait on condition changes

class barrier_integer_condition
{

// attributes
private:

	/// a mutex to protect object accesses
	pthread_mutex_t access_mutex_;
	/// a pthread condition to wait upon
	pthread_cond_t state_;
	/// state of the condition
	int value_;


// methods
public:

	/// constructor : give initial activity state.
	barrier_integer_condition(int value)
		:
		access_mutex_(),
		state_(),
		value_(value)
	{
		pthread_mutex_init(&access_mutex_,nullptr);
		pthread_cond_init(&state_, nullptr);
	}

	/// delete to reclaim mutexes
	virtual
	~barrier_integer_condition()
	{
		pthread_mutex_destroy(&access_mutex_);
		pthread_cond_destroy(&state_);
	};

	/// set the condition state, notifies blocked observers in wait_until
	virtual void
	add_to_value(int value)
	{
		if ( value != 0 )
		{
			// avoid broadcast to notify all threads blocked in wait_until if condition unchanged
			pthread_mutex_lock(&access_mutex_);
			value_ += value ;
			pthread_cond_broadcast(&state_);
			pthread_mutex_unlock(&access_mutex_);
		}
	}

	/// non blocking interrogation of the condition state
	virtual
	int
	get_value()
	{
		pthread_mutex_lock(&access_mutex_);
		int value = value_ ;
		pthread_mutex_unlock(&access_mutex_);
		return value;
	}

	///
	virtual
	void
	set_value(int v)
	{
		pthread_mutex_lock(&access_mutex_);
		value_ = v;
		pthread_cond_broadcast(&state_);
		pthread_mutex_unlock(&access_mutex_);
	}

	/// user level function : sleep until the monitored condition attains desired state
	void
	wait_until_gt(int value)
	{
		pthread_mutex_lock(&access_mutex_);
		while( value_ <= value )
		{
			pthread_cond_wait ( &state_,
								&access_mutex_ );
		}
		pthread_mutex_unlock( &access_mutex_ );
	}

	/// user level function : sleep until the monitored condition attains desired state
	void
	wait_until(int value)
	{
		pthread_mutex_lock(&access_mutex_);
		while ( value_ != value )
		{
			pthread_cond_wait( &state_, &access_mutex_ );
		}
		pthread_mutex_unlock( &access_mutex_ );
	}

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
