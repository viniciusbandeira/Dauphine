#ifndef INCLUDE_BSTATEICEPRISION_H
#define INCLUDE_BSTATEICEPRISION_H

#include "StateBoss.h"

class BStateIcePrision : public StateBoss
{

	public:
		/**
		* The constructor.
		* @param BOSS : Reference to the Boss.
		*/
		BStateIcePrision( Boss *const BOSS );

		/**
		* The destructor.
		*/
		virtual ~BStateIcePrision () {}

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

		void update_player();

		double prision_time = 0.0;

};

#endif // INCLUDE_BSTATEICEPRISION_H
