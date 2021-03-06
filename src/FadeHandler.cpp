/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @FadeHandler.cpp
 * File responsible for implementing fade type.
 * License: Copyright (C) 2014 Alke Games.
 */

#include <assert.h>
#include "FadeHandler.h"
#include "Logger.h"

#define UNITY_ALPHA 255.0
#define THOUSAND 1000.0
#define ZERO 0

/*
 * The constructor
 * @param sprite_ : representation for animation
 * @see StateEnemy::exit
 */
FadeHandler::FadeHandler(Sprite* const sprite_) :
	should_fade_in(false),
	should_fade_out(false),
	stop_percentage(0.0),
	sprite(sprite_),
	time(0.0),
	current_percentage(0.0),
	rate(0.0)
{
	this -> current_percentage = ( this -> sprite->getAlpha()/UNITY_ALPHA );
}

/*
 * The constructor
 */
FadeHandler::~FadeHandler()
{
	this -> sprite = nullptr;
}

/*
 * Manipulate input characters on screen
 * @param percentage : percentage fade in
 * @param time: time fade in
 */
void FadeHandler::fadeIn( const double percentage_, const double time_)
{
	assert( percentage_ >= ZERO );
	assert( time_ >= ZERO );
	this -> should_fade_in = true;
	this -> should_fade_out = false;
	this -> stop_percentage = percentage_;
	this -> current_percentage = ( this -> sprite->getAlpha()/UNITY_ALPHA);
	this -> rate = ( this -> stop_percentage - this -> current_percentage)/time_;
}

/*
 * Manipulate output characters on screen
 * @param percentage : percentage fade out
 * @param time: time fade out
 */
void FadeHandler::fade_out( const double percentage_, const double time_)
{
	this -> should_fade_out = true;
	this -> should_fade_in = false;
	this -> stop_percentage = percentage_;
	this -> current_percentage = ( this -> sprite->getAlpha()/UNITY_ALPHA);
	this -> rate = ( this -> current_percentage - this -> stop_percentage)/time_;
}

/*
 * Manipulate update characters on screen
 * @param DELTA_TIME : delta time (time elapsed)
 */
void FadeHandler::update( const double DELTA_TIME)
{
	assert ( DELTA_TIME >= ZERO );
	if ( this -> sprite == nullptr )
	{
		Log(WARN) << "No sprite set to Fade Handler.";
		return;
	} else {
		Log ( INFO ) << "\tSprite rendered";
	}

  //Check the rate that should fade at the input
	if ( should_fade_in )
	{
		if ( this -> current_percentage < this -> stop_percentage )
		{
			this -> current_percentage += this -> rate * DELTA_TIME/THOUSAND;
			this -> sprite->setAlpha( UNITY_ALPHA * this -> current_percentage );
		} else {
			should_fade_in = false;
			this -> current_percentage = this -> stop_percentage;
			this -> sprite -> setAlpha( UNITY_ALPHA * this -> current_percentage );
		}
	}
	//check the rate that should fade at the output
	else if ( should_fade_out )
	{
		if ( this -> current_percentage > this -> stop_percentage )
		{
			this -> current_percentage -= this -> rate * DELTA_TIME/THOUSAND;
			this -> sprite -> setAlpha( UNITY_ALPHA * this -> current_percentage );
		} else {
			  should_fade_out = false;
			  this -> current_percentage = this -> stop_percentage;
			  this -> sprite -> setAlpha(UNITY_ALPHA * this -> current_percentage);
		  }
	} else {
		Log ( INFO ) << "\tSprite of fade handler updated";
	}
}

/*
 * Get current percentage
 */
double FadeHandler::getCurrentPercentage()
{
	return this -> current_percentage;
}
