#include "child_destructor.h"

#include <child_process.h>
#include <base/thread.h>


class Child_destructor_thread : Genode::Thread<2*4096>
{
	private:

		ChildProcess *_curr_child;     /* currently destructed child       */
    Genode::Allocator       *_curr_alloc;     /* child object'sallocator          */
    Genode::Lock             _submit_lock;    /* only one submission at a time    */
    Genode::Lock             _activate_lock;  /* submission protocol              */
		bool             _ready;          /* set if submission is completed   */
		int              _watchdog_cnt;   /* watchdog counter in milliseconds */

		/**
		 * Thread entry function
		 */
		void entry() {
			while (true) {

				/* wait for next submission */
				_activate_lock.lock();

				/*
				 * Eventually long-taking operation that involves the
				 * closing of all session of the child. This procedure
				 * may need blocking cancellation to proceed in the
				 * case servers are unresponsive.
				 */
				try {
					destroy(_curr_alloc, _curr_child);
				} catch (Genode::Blocking_canceled) {
					PERR("Suspicious cancellation");
				}

				_ready = true;
			}
		}

	public:

		/*
		 * Watchdog timer granularity in milliseconds. This value defined
		 * after how many milliseconds the watchdog is activated.
		 */
		enum { WATCHDOG_GRANULARITY_MS = 10 };

		/**
		 * Constructor
		 */
		Child_destructor_thread() :
			Thread("child_destructor"),
			_curr_child(0), _curr_alloc(0),
			_activate_lock(Genode::Lock::LOCKED),
			_ready(true)
		{
			start();
		}

		/**
		 * Destruct child, coping with unresponsive servers
		 *
		 * \param alloc       Child object's allocator
		 * \param child       Child to destruct
		 * \param timeout_ms  Maximum destruction time until the destructing
		 *                    thread gets waken up to give up the close call to
		 *                    an unreponsive server.
		 */
		void submit_for_destruction(Genode::Allocator *alloc, ChildProcess *child,
                                Timer::Session *timer, int timeout_ms)
		{
			/* block until destructor thread is ready for new submission */
      Genode::Lock::Guard _lock_guard(_submit_lock);

			/* register submission values */
			_curr_child   = child;
			_curr_alloc   = alloc;
			_ready        = false;
			_watchdog_cnt = 0;

			/* wake up the destruction thread */
			_activate_lock.unlock();

			/*
			 * Now, the destruction thread attempts to close all the
			 * child's sessions. Check '_ready' flag periodically.
			 */
			while (!_ready) {

				/* give the destruction thread some time to proceed */
				timer->msleep(WATCHDOG_GRANULARITY_MS);
				_watchdog_cnt += WATCHDOG_GRANULARITY_MS;

				/* check if we reached the timeout */
				if (_watchdog_cnt > timeout_ms) {

					/*
					 * The destruction seems to got stuck, let's shake it a
					 * bit to proceed and reset the watchdog counter to give
					 * the next blocking operation a chance to execute.
					 */
					cancel_blocking();
					_watchdog_cnt = 0;
				}
			}
		}
};

/**
 * Construct a timer session for the watchdog timer on demand
 */
static Timer::Session *timer_session()
{
	static Timer::Connection timer;
	return &timer;
}


/* construct child-destructor thread early - in case we run out of threads */
static Child_destructor_thread child_destructor;

/**
 * Destruct Launchpad_child, cope with infinitely blocking server->close calls
 *
 * The arguments correspond to the 'Child_destructor_thread::submit_for_destruction'
 * function.
 */
static void destruct_child(Genode::Allocator *alloc, ChildProcess *child,
                            Timer::Session *timer, int timeout)
{
	/* if no timer session was provided by our caller, we have create one */
	if (!timer)
		timer = timer_session();

	child_destructor.submit_for_destruction(alloc, child, timer, timeout);
}
