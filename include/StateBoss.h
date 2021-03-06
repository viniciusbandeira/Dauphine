#ifndef INCLUDE_STATEBOSS_H
#define INCLUDE_STATEBOSS_H

#include "Boss.h"

class Boss;

/**
* Parent class for other boss states.
* 
*/
class StateBoss 
{

	public:
		/**
		* The constructor.
		* @param BOSS : Reference to the Boss.
		*/
		StateBoss ( Boss *const BOSS );

		/**
		* The destructor.
		*/
		virtual ~StateBoss ();

		/**
		* Method called on load.
		* Pure virtual function. Called on state enter.
		*/
		virtual void enter () = 0;

		/**
		* Method called on unload.
		* Pure virtual function. Called on state exit.
		*/
		virtual void exit () = 0;

		/**
		* Update method for the state.
		* While the Boss is on the state, this method runs every update.
		*/
		virtual void update ( const double DELTA_TIME ) = 0;

		Boss *boss; /**< Reference to the boss. */

};

#endif // INCLUDE_STATEBOSS_H