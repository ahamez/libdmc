// $Author: ahamez $
// $Revision: 687 $
// $Date: 2006-08-24 22:10:10 +0200 (Thu, 24 Aug 2006) $
#ifndef DMC_BARRIER_CONDITION_HH_
#define DMC_BARRIER_CONDITION_HH_

#include <pthread.h>

namespace dmc { namespace mon {

/////////////////////////////////////////////////////////////////////

/// A class to have barrier semantics on a condition.
/// Implements the basic condition interface. 
/// set() notifies callers blocked on the wait_until
/// get() allows to poll (non-blocking) the state of the condition
/// wait_until() allows non active wait on condition changes

class barrier_condition 
{

// attributes
private :

	/// a mutex to protect object accesses
	pthread_mutex_t mut_;
	/// a pthread condition to wait upon
	pthread_cond_t state_;
	/// state of the condition
	bool is_state_;
	
	
// methods
public:

	/// constructor : give initial activity state.
	barrier_condition(bool cond_state) 
		:
		mut_(),
		state_(),
		is_state_(cond_state) 
	{
		pthread_mutex_init(&mut_, nullptr);
		pthread_cond_init(&state_, nullptr);
	}

	/// delete to reclaim mutexes
	virtual ~barrier_condition()
	{
		pthread_mutex_destroy(&mut_);
		pthread_cond_destroy(&state_);
	};
  
	/// set the condition state, notifies blocked observers in wait_until
	virtual void 
	set_state(bool cond_state) 
	{
		pthread_mutex_lock(&mut_);
		if ( is_state_ != cond_state ) 
		{
			// avoid broadcast to notify all threads blocked in wait_until if condition unchanged
			is_state_ = cond_state ;
			pthread_cond_broadcast(&state_);
		}
		pthread_mutex_unlock(&mut_);
	}

	/// non blocking interrogation of the condition state
	virtual bool 
	get_state () 
	{
		pthread_mutex_lock(&mut_);
		bool cond_state = is_state_ ;
		pthread_mutex_unlock(&mut_);
		return cond_state;
	}
 
	/// user level function : sleep until the monitored condition attains desired state
	void  
	wait_until(bool cond_state)
	{
		pthread_mutex_lock(&mut_);
		while ( is_state_ != cond_state )
		{
			pthread_cond_wait(&state_, &mut_);
		}  
		pthread_mutex_unlock(&mut_);
	}
  
};
	
//////////////////////////////////////////////////////////////////////
	
}} // namespaces

#endif
