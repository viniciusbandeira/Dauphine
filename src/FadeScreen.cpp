/*
 * Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @FadeScreen.cpp
 * Objects in this class are responsible for determining the behavior of the fade on the screen.
 * License: Copyright (C) 2014 Alke Games.
 */

#include "FadeScreen.h"
#include "Game.h"
#include "Logger.h"

FadeScreen::FadeScreen() :

	// Load de image of fade from a file
	fadeImage ( Game::instance (). getResources (). get ( "res/images/black_screen.png" ) ),
	fadeHandler ( new FadeHandler ( this -> fadeImage ) )

{

}

FadeScreen::~FadeScreen ()
{

	if ( this -> fadeHandler != nullptr )
	{

		delete this -> fadeHandler;
		this -> fadeHandler = nullptr;

	} else {
		// Do nothing.
	}
}

/*
Update the position of fade and when show up or not
@param DELTA_TIME : Delta time. Time elapsed between one frame and the other
*/
void FadeScreen::update ( const double DELTA_TIME )
{
	fadeHandler -> update ( DELTA_TIME );
}

// Verify condition to Render the image
void FadeScreen::render ()
{

	if ( this -> fadeImage != nullptr )
	{
		this -> fadeImage -> render ( 0, 0, nullptr, true );
	} else {
		// Do nothing.
	}
}

/* Verify condition to Fade (false or true)
 * @return A bool variable when the currently percentage is reached. */
bool FadeScreen::isFaded ()
{

	bool faded = false;

	const int current_percentage = this -> fadeHandler -> getCurrentPercentage ();

	if ( current_percentage == 1.0 )
	{
		faded = true;
	} else {
		faded = false;
	}

	return faded;
}

/*
Fade out the image on screen according to the percentage and the time
@param percentage_: Percentage of image appearance on screen
@param time_: Time the image will appear on screen
*/
void FadeScreen::fade_out ( const double percentage_, const double time_ )
{
	this -> fadeHandler -> fade_out ( percentage_, time_ );
}

// Fade in the image on screen according to the percentage and the time
void FadeScreen::fadeIn ( const double percentage_, const double time_ )
{
	this -> fadeHandler -> fadeIn ( percentage_, time_ );
}
