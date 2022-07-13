/// \file 
/// \brief Definitions related to the Timer class

#ifndef TIMER_H
#define TIMER_H

#include <ctime>
#include <cassert>

class Timer               /// This class allows representing timers \ingroup information
{
	protected:
		clock_t begin;				///< the clock at the beggining of the timer
		double duration;			///< the cumulative duration of the the timer in seconds
		bool is_activated;		///< true if the timer has been started, false otherwise
		clock_t update_limit;	///< the clock value from which the update must be performed
	
	public:
		// constructor
		Timer ();						///< constructs a timer
		
		// functions in order to handle timer
		void Reset ();          ///< resets the timer
		void Start ();   			  ///< starts the timer
		void Stop ();   				///< stop the timer
		double Get_Duration (); ///< returns the duration related to the timer in milliseconds
		void Update ();				  ///< updates the cumulative duration (useful and required if the timer is expected to exceed one hour)
};


//-----------------------------
// inline function definitions
//-----------------------------


inline void Timer::Reset ()
// resets the timer
{
	duration = 0;
	is_activated = false;
}


inline void Timer::Start ()
// starts the timer
{
	assert (! is_activated);
	is_activated = true;
	begin = clock();
	update_limit = begin + 1800 * CLOCKS_PER_SEC;
}


inline void Timer::Stop ()
// stop the timer
{
	assert (is_activated);
	duration += ((double) ((unsigned long) (clock() - begin))) / CLOCKS_PER_SEC;
	is_activated = false;
}


inline double Timer::Get_Duration ()
// returns the duration related to the timer in seconds
{
	if (is_activated)  return duration + ((double) (clock() - begin)) / CLOCKS_PER_SEC;
	else return duration;
}


inline void Timer::Update ()
// updates the cumulative duration (useful and required if the timer is expected to exceed one hour)
{
	clock_t c;
	
	c = clock();
	if (c >= update_limit)
	{
		duration  += ((double) (c - begin)) / CLOCKS_PER_SEC;
		
		begin = clock();
		update_limit = begin + 1800 * CLOCKS_PER_SEC;
	}
}
#endif
