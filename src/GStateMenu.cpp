/* Dauphine
* Universidade de Brasília - FGA
* Técnicas de Programação, 2/2017
* @GStateMenu.cpp
* The control of state of the menu of the game.
* Class that controll the state of menu of the game.
* License: Copyright (C) 2014 Alke Games.
*/

#include "GStateMenu.h"
#include "LuaScript.h"
#include "Game.h"

#include <string>

/**
* The constructor.
* Initializes the attributes.
* @param shouldIgnore : Analyzes when some action will be ignored or not.
* @param menuImage : Render the images of menu for manage the states.
* @param attractModeBg : Render the image of title of the game in background.
* @param attractMode : Render the image which explains the context of the game of the game.
* @param passedTime : Count the time useds for select some option in menu.
* @param currentSelection : Manages the exhibition for the current option selected in menu.
* @param selectorXPositionLeft : Manages the exhibition for the iten in axis X and left position in menu.
* @param selectorYPositionLeft : Manages the exhibition for the iten in axis Y and left position in menu.
* @param selectorXPositionRight : Manages the exhibition for the iten in axis X and right position in menu.
* @param selectorYPositionRight : Manages the exhibition for the iten in axis Y and right position in menu.
* @param attractHeightSize : Count and update the height size of the attract itens.
* @param attractChangeSpeed : Count and update the speed of the attract itens.
* @param shwingAnimation : Render the animation for change of selector in menu.
* @param shwingIsActivated : Render the animation if the iten is actived.
* @param shwing : Render the logo of the game.
* @param shwingClip : Auxiliar variable used for render the updates.
*/
GStateMenu::GStateMenu() :
	shouldIgnore( false ),
	menuImage( nullptr ),
	menuSelector( nullptr ),
	attractModeBg( nullptr ),
	attractMode( nullptr ),
	passedTime( 0.0 ),
	currentSelection( Selection::NEWGAME ),
	selectorXPositionLeft { 610, 635, 635, 645 },
	selectorYPositionLeft { 560, 625, 690, 755 },
	selectorXPositionRight { 880, 855, 855, 845 },
	selectorYPositionRight { 560, 625, 690, 755 },
	attractHeightSize( 600 ),
	attractChangeSpeed( 1 ),
	attractClip{ 0, 0, 0, this -> attractHeightSize },
	shwingAnimation( nullptr ),
	shwingIsActivated( true ),
	shwing( nullptr ),
	shwingClip { 0,0,0,0 }
{

}

/**
* The destructor.
*/
GStateMenu::~GStateMenu()
{
  // Checking if the object is not null, if not, it is deleted.
	if( this -> shwingAnimation != nullptr )
	{
		delete this -> shwingAnimation;
		this -> shwingAnimation = nullptr;
	}
}

/**
* Load the menu of the game showing the options and playing the music.
*/
void GStateMenu::load()
{
	Log( DEBUG ) << "Loading menu...";

	// Changing the music.
	Game::instance().clearKeyFromInput(GameKeys::ESCAPE);

	Game::instance().getAudioHandler().changeMusic("res/audio/menu.mid");

	// Getting information from lua script.
	LuaScript luaMenu("lua/Menu.lua");
	const std::string pathTitleScreen = luaMenu.unlua_get<std::string>("menu.images.titleScreen");
	const std::string pathCursor = luaMenu.unlua_get<std::string>("menu.images.cursor");

	// Load the configurations used in menu.
    this -> menuImage = Game::instance().getResources().get( pathTitleScreen );
    this -> menuSelector = Game::instance().getResources().get( pathCursor );
    this -> attractModeBg = Game::instance().getResources().get("res/images/title_background.png");
    this -> attractMode = Game::instance().getResources().get("res/images/attract.png");
    this -> attractClip.w = this -> attractMode -> getWidth();
    this -> shwingAnimation = new Animation( 0, 0, 795, 360, 3, false );
    this -> shwing = Game::instance().getResources().get("res/images/shwing_sheet.png");
    this -> shwingAnimation -> ANIMATION_LIMIT = 2;

    Game::instance().getFade().fadeOut( 0, 0.002 );
}

/**
* Exiting of the menu and cleaning the display.
*/
void GStateMenu::unload()
{
	// Exiting of the menu and cleaning its entities.
	Log( DEBUG ) << "\tUnloading menu...";
	this -> attractClip.y = 0;
	cleanEntities();
}

/**
* Changing the state menu of the game.
* @param dt_: Delta time. Time elapsed between one frame and the other, independent
* 	of processing speed.
*/
void GStateMenu::update( const double dt_ )
{
	this -> passedTime += dt_;

	handleSelectorMenu();

	this -> shwingAnimation -> update( this -> shwingClip, dt_ );	

  std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();
  
  // Stopping with the menu update in case of quit of the menu.
	if( keyStates[ GameKeys::ESCAPE ] == true )
	{
		Game::instance().stop();
	}

  // Changing the menu state showing the animation and gettint out for there.
	if( this -> shwingIsActivated )
	{
		// Showing animation for actived state.
		this -> shwingAnimation -> changeAnimation( 0, 0, 12, false, 2 );
		this -> shwingIsActivated = false;
	}

}

/**
* Show the menu of the game with your properties.
*/
void GStateMenu::render()
{

  // Checking if the time rendering the menu is higher than 10 seconds.
	if( this -> passedTime > 10 )
	{
		this -> attractModeBg -> render( 0, 0, nullptr, true );
		this -> attractMode -> render( 0, 0, &this -> attractClip, true );
		shouldIgnore = true;
    
    // Checking if the attractClip in axys y is smaller than height of the attract mode.
    if( this -> attractClip.y < ( int ) this -> attractMode -> getHeight() - this -> attractHeightSize )
		{
			this -> attractClip.y  += this -> attractChangeSpeed;
    }
    
    // Checking if the attractClip in axys y is not smaller than height of the attract mode.    
		else
		{
			//shwing->render(340,50,&this->shwingClip);
    }

    // Checking if the time rendering the menu is higher than 75 seconds.    
		if( this -> passedTime > 75 )
		{
      // Zering the passed time and attract for clip in axis y.
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
		}
  }
  
  // Checking if the time rendering the menu is not higher than 10 seconds.  
	else
	{
    
    // Checking if the image of menu is not null.    
		if( this -> menuImage != nullptr )
		{
      // Setting and rendering the menu image when it is not null.
			this -> menuImage -> render( 0, 0, nullptr, true );

			this -> menuSelector -> setWidth( 50 );

			this -> menuSelector -> render( selectorXPositionLeft[ currentSelection ],
				selectorYPositionLeft[ currentSelection ], nullptr, false, 0.0, nullptr, SDL_FLIP_NONE );

			this -> menuSelector -> render( selectorXPositionRight[ currentSelection ],
				selectorYPositionRight[ currentSelection ], nullptr, false, 0.0, nullptr, SDL_FLIP_HORIZONTAL );

    }
    
    // Checking if the image of menu is null
		else
		{
			Log( WARN ) << "No image set to display on the menu!";
		}
	}

}

/**
* Controlls the user input for enter in menu of the game.
*/
void GStateMenu::handleSelectorMenu()
{
	std::array<bool, GameKeys::MAX> keyStates = Game::instance().getInput();

	// @param selectorDelayTime : Used for manage the delay for change of options in select of the menu.
	const double selectorDelayTime = 0.2;

  // Showing and updating the animation for selector of the menu.
	if( keyStates[ GameKeys::DOWN ] == true || keyStates[ GameKeys::RIGHT ] == true )
	{
    // Configuring when the update is prohibited.
		if( shouldIgnore )
		{
      // Zering the passed time and attract for clip in axis y.      
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
    }
    
    // Setting a delay for big waiting time for selection.
		if( this -> passedTime >= selectorDelayTime )
		{
      // Changing the position of the cursor in the options of menu
			if( currentSelection < ( Selection::TOTAL - 1 ) )
			{
				currentSelection++;
			}
			else
			{
				currentSelection = Selection::NEWGAME;
      }
      
      // Zering the passed time and attract for clip in axis y.			
			this->passedTime = 0.0;
			this->attractClip.y = 0;
		}
  }
  
  // Showing and updating the animation for selector of the menu.
	else if( keyStates[ GameKeys::UP ] == true || keyStates[GameKeys::LEFT] == true )
	{
    // Configurin when the update is prohibited.
		if( shouldIgnore )
		{
      // Zering the passed time and attract for clip in axis y.      
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

    // Setting a delay for big waiting time for selection
		if( this -> passedTime >= selectorDelayTime )
		{
      // Changing the position of the cursor in the options of menu for "new game"      
			if( currentSelection > Selection::NEWGAME )
			{
				currentSelection--;
			}
			else
			{
				currentSelection = ( Selection::TOTAL - 1 );
      }
      // Zering the passed time and attract for clip in axis y.      
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
		}
  }
  
  // Showing and updating the animation for selector of the menu in the "new game" option.
	else if( currentSelection == Selection::NEWGAME && keyStates[ GameKeys::SPACE ] == true )
	{
    // Configurin when the update is prohibited.
		if( shouldIgnore )
		{
      // Zering the passed time and attract for clip in axis y.      
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

    // Zering the passed time and attract for clip in axis y.    
		Game::instance().setState(Game::GStates::NEW_GAME);
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}

  // Showing and updating the animation for selector of the menu in the "continue" option.
	else if( currentSelection == Selection::CONTINUE && keyStates[ GameKeys::SPACE ] == true)
	{
    // Configurin when the update is prohibited.
		if( shouldIgnore )
		{
      // Zering the passed time and attract for clip in axis y.      
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

    // Zering the passed time and attract for clip in axis y.    
		Game::instance().setState( Game::GStates::CONTINUE );
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}

  // Showing and updating the animation for selector of the menu in the "options" option.
	else if( currentSelection == Selection::OPTIONS && keyStates[ GameKeys::SPACE ] == true)
	{
    // Configurin when the update is prohibited.
		if( shouldIgnore )
		{
      // Zering the passed time and attract for clip in axis y.      
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
		}

    // Zering the passed time and attract for clip in axis y.    
		Game::instance().setState( Game::GStates::OPTIONS );
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}

  // Showing and updating the animation for selector of the menu in the "credits" option.
	else if( currentSelection == Selection::CREDITS && keyStates[ GameKeys::SPACE ] == true )
	{
    // Configurin when the update is prohibited.
		if( shouldIgnore )
		{
      // Zering the passed time and attract for clip in axis y.      
			this -> passedTime = 0.0;
			this -> attractClip.y = 0;
			shouldIgnore = false;
			return;
    }
    
    // Zering the passed time and attract for clip in axis y.
		Game::instance().setState( Game::GStates::CREDITS );
		this -> passedTime = 0.0;
		this -> attractClip.y = 0;
	}
}
