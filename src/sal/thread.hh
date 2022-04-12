// $Author: lbautista $
// $Revision: 1030 $
// $Date: 2008-04-03 18:06:33 +0200 (jeu, 03 avr 2008) $

#ifndef DMC_THREAD_H_
#define DMC_THREAD_H_

#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <signal.h>

namespace dmc { namespace sal {

//////////////////////////////////////////////////////////////////////

/// A wraper around pthreads to have objects with taste of thread

class thread
{

// attributes
protected:

	pthread_t tid_;

private:

	pthread_attr_t thread_type_;


// methods
public:

	thread()
		:
		tid_(),
		thread_type_()
	{
		pthread_attr_init(&thread_type_);
		pthread_attr_setschedpolicy( &thread_type_,
									 SCHED_OTHER);
		pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS,nullptr);
	};

	virtual ~thread(){};

	void start_thread()
	{
		if( pthread_create( &tid_,
							&thread_type_,
							entry_point,
							this ) )
		{
			perror("[pthread_create]");
			exit(1);
		}

    struct sigaction SIG_action;

    SIG_action.sa_handler = &SIG_handler;

    sigaction(SIGQUIT, &SIG_action, nullptr);
    sigaction(SIGKILL, &SIG_action, nullptr);
  }

        void kill_thread()
        {
            pthread_kill(tid_,SIGQUIT);
        }

	void join_thread()
	{
		pthread_join(tid_,nullptr);
	}

	pthread_t get_tid() const
	{
		return tid_;
	}

protected:

        static void SIG_handler(int signal) {
            switch (signal) {
                case (SIGQUIT): {
                                    pthread_exit(nullptr);
                                    break;
                                }
                case (SIGKILL): {
                                    pthread_exit(nullptr);
                                    break;
                                }
            }
        };

	static void* entry_point(void* pthis)
	{
		static_cast<thread*>(pthis)->run();
		// just to skip warning...
		return nullptr;
	};

	void stop_thread()
	{
		if( tid_ == pthread_self() )
		{
			pthread_exit(nullptr);
		}
	}

	bool operator==(const thread& t)
	{
		return t.tid_ == tid_;
	}

	/// to be redefined in derived classes
	virtual void run() = 0;

private:

	// cannot copy a thread
	thread(const thread&);
	thread& operator=(const thread&);

};

//////////////////////////////////////////////////////////////////////

}} // namespaces

#endif
