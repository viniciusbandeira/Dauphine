#ifndef INCLUDE_BSTATESHIELD_H
#define INCLUDE_BSTATESHIELD_H

#include "StateBoss.h"

class BStateShield : public StateBoss 
{

	public:
		/**
		* The constructor.
		* @param BOSS : Reference to the Boss.
		*/
		BStateShield ( Boss *const BOSS );
		
		/**
		* The destructor.
		*/
		virtual ~BStateShield () {}

		/**
		* @see StateBoss::enter
		*/
		virtual void enter ();

		/**
		* @see StateBoss::exit
		*/
		virtual void exit ();

		/**
		* @see StateBoss::update
		*/
		virtual void update ( const double DELTA_TIME );

};

#endif // INCLUDE_BSTATESHIELD_H
