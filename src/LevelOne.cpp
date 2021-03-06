/* Dauphine
 * Universidade de Brasília - FGA
 * Técnicas de Programação, 2/2017
 * @LevelOne.cpp
 * The first level of the game.
 * Derived from Level class
 * License: Copyright (C) 2014 Alke Games.
 */

#include <assert.h>
#include "LevelOne.h"
#include "Game.h"
#include "GameSave.h"
#include "LuaScript.h"
#include "Logger.h"
#include "Enemy.h"
#include "Crosshair.h"
#include "TileMap.h"
#include "Collision.h"
#include "Crosshair.h"

double ok = 0; // iterator that determines when to change the state to game over

// Quantity of items that can be caught in phase 1.
LevelOne::LevelOne ():
	Level(),
	items {{ 59 * 64, 114 * 64, 0, 0 },{ 1750, 1750, 0, 0 }},
	caught_items { false, false, true, true }
{
	this -> changeCheckpoints ( 2, { 4000, 7500 }, { 1600, 1600 });
}

LevelOne::~LevelOne ()
{

}

/**
* Loads the level.
* From the Level1.lua script, loads all the necessary objects.
*/

void LevelOne::loadTilemap(){
	// Loading the tile/tilemap.
	this -> tile_map = new TileMap ( "res/maps/level1.tmx" );
	assert( this-> tile_map != nullptr);
}
void LevelOne::loadWidthHeight(){
	// Setting the level width/height.
	this -> width = this -> tile_map -> getMapWidth ();

	this -> height = this -> tile_map -> getMapHeight ();

	SDL_Rect bounds = { 0, 0, ( int )this -> width, ( int )this -> height };

	this -> quadTree = new QuadTree ( 0, bounds );
	assert( this -> quadTree != nullptr );

	this -> background = Game::instance (). getResources (). get (
	 "res/images/lv1_background_parallax.png" );

	this -> backgroud_top = Game::instance (). getResources (). get (
	 "res/images/lv1_parallax_top.png" );
}

void LevelOne::loadCheckpoints(){
	// Load the number of checkpoints achieved
 for ( int i = 0; i < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++i )
 {

	 this -> checkpoints.push_back ( Game::instance (). getResources (). get (
		"res/images/checkpoint.png" ));

 }
}

void LevelOne::loadPlayer(){
	// Getting information from lua script.
	LuaScript luaLevel1 ( "lua/Level1.lua" );

	const std::string PATH_PLAYER_SPRITE_SHEET = luaLevel1.unlua_get <std::string> (
		"level.player.spriteSheet" );

		// Loading the player and the camera.

		Player *level_player = nullptr;

		if ( Game::instance (). get_saves (). is_saved ( Game::instance (). current_slot )
		 && Game::instance (). get_saves (). get_saved_level ( Game::instance (). current_slot ) == 1 )
		{

			double saved_x_position = 0.0;
			double saved_y_position = 0.0;

			Game::instance (). get_saves (). get_player_position ( saved_x_position, saved_y_position,
			Game::instance ().current_slot );

			level_player = new Player ( saved_x_position, saved_y_position, PATH_PLAYER_SPRITE_SHEET );
		}else
		{
			level_player = new Player ( this -> tile_map -> get_initial_x (), this -> tile_map ->
				get_initial_y (), PATH_PLAYER_SPRITE_SHEET );
		}
		Camera *level_camera = new Camera ( level_player );
		// Loading the refill of potion.
		this -> image = Game::instance (). getResources (). get( "res/images/potion.png" );
		assert(this -> image != nullptr);

		this -> player_Hud = new PlayerHUD( level_player );
		assert(this -> player_Hud != nullptr);

		// Finally, setting the player and the camera.
		set_player ( level_player );

		Enemy::points_life = this -> player -> life;

		set_camera ( level_camera );

		Game::instance (). get_fade (). fade_out ( 0, 0.002 );
}

void LevelOne::loadEnemies()
{
	// Getting information from lua script.
	LuaScript luaLevel1 ( "lua/Level1.lua" );
	const std::string PATH_ENEMY = luaLevel1.unlua_get <std::string> ( "level.enemy" );
	// Load all the enemies from the tile_map.
	for ( unsigned int i = 0; i < this -> tile_map -> get_enemies_x (). size (); i++ )
	{

		Enemy *enemy = new Enemy ( this -> tile_map -> get_enemies_x (). at( i ),
			this -> tile_map -> get_enemies_y (). at( i ), PATH_ENEMY,
			this -> tile_map -> get_enemies_patrol (). at( i ), 0.0 );

		if ( Game::instance (). get_saves (). is_saved ( Game::instance (). current_slot ) )
		{

			if ( Game::instance (). get_saves (). is_enemy_dead ( i, Game::instance (). current_slot )
				&& Game::instance (). get_saves (). get_saved_level ( Game::instance (). current_slot )
				== 1 )
			{

				enemy -> set_dead ( true );

			} else {
				enemy -> set_dead ( false );
			}
		}

		enemy -> setLevelWH ( this -> width, this -> height );
		this -> enemies. push_back ( enemy );

	}
}
void LevelOne::load ()
{
	Log ( DEBUG ) << "Loading level 1...";

	loadTilemap ();
	loadWidthHeight ();
	loadCheckpoints ();


	// Getting information from lua script.
	LuaScript luaLevel1 ( "lua/Level1.lua" );

	const std::string PATH_BACKGROUND_AUDIO = luaLevel1.unlua_get <std::string> (
		"level.audio.background" );

	// Changing the music.
	Game::instance (). get_audio_handler (). change_music ( PATH_BACKGROUND_AUDIO );

	loadPlayer();

	loadEnemies();
}

// Unloads everything that was loaded.

void LevelOne::unload ()
{
	Log ( DEBUG ) << "\tUnloading level 1...";

	clean_entities ();
	clear_enemies ();
	clear_documents ();

	for ( int i = 0; i < 4; ++i )
	{

		caught_items [ i ] = false;
	}
}

// Update the entities from Level 1.
void LevelOne::updateEntities (const double DELTA_TIME )
{

	// Populating the QuadTree.
	this -> quadTree -> setObjects ( this -> tile_map -> getCollisionRects () );

	// Updating the entities, using the QuadTree.
	std::vector < CollisionRect > return_objects;

	for ( auto entity : this -> entities )
	{

		return_objects. clear ();

		this -> quadTree -> retrieve ( return_objects, entity -> get_bounding_box () );

		entity -> setCollisionRects ( return_objects );

		entity -> update( DELTA_TIME );

	}
}

// Update enemies from Level 1.
void LevelOne::updateEnemies ( const double DELTA_TIME )
{
	// Populating the QuadTree.
	this -> quadTree -> setObjects ( this -> tile_map -> getCollisionRects () );

	// Updating the entities, using the QuadTree.
	std::vector < CollisionRect > return_objects;

	// Updating the enemies.
	for ( auto enemy : this -> enemies )
	{

		return_objects. clear ();

		this -> quadTree -> retrieve ( return_objects, enemy -> get_bounding_box () );

		enemy -> setCollisionRects ( return_objects );

		enemy -> update( DELTA_TIME );

	}
}

// Update the potions from Level 1.
void LevelOne::updatePotions ( )
{
	// Populating the QuadTree.
	this -> quadTree -> setObjects ( this -> tile_map -> getCollisionRects () );

	// Updating the entities, using the QuadTree.
	std::vector < CollisionRect > return_objects;

	// Updating the potions.
	for( auto potion : this -> player -> potions )
	{

		return_objects. clear ();

		this -> quadTree -> retrieve ( return_objects, potion -> get_bounding_box () );

		potion -> setCollisionRects ( return_objects );

	}
}

// Update the position and state from player.
void LevelOne::updatePlayer ( )
{
	/// @todo Maybe refactor this static Enemy::px, Enemy::py.
	// Updating player info for the enemies.
	Enemy::px = this -> player -> x;
	Enemy::py = this -> player -> y;
	Enemy::position_vulnerable = this -> player -> is_vulnerable;

	for ( int i = 0; i < 4; ++i )
	{

		if ( Collision::rects_collided ( this -> player -> get_bounding_box (),
			{ items [ 0 ] [ i ], items [ 1 ] [ i ], 192, 192 } ) && caught_items [ i ] == false )
		{

			this -> player -> addPotions ( 3 );

			caught_items [ i ] = true;

		}
	}

 	if ( this -> player -> life != Enemy::points_life )
 	{

		if ( this -> player -> is_vulnerable )
		{

			this -> player -> life--;
			Enemy::points_life = this -> player -> life;
			this -> player -> changeState ( Player::player_states::HITED );
			this -> player -> is_vulnerable = false;
		}else
		{
			//Do nothing.
		}
 	}

	// Updating the HUD.
	this -> player_Hud -> update ();

	// Updating the camera.
	this -> camera -> update ();

	// Set next level if end is reached.
	if ( this -> player -> reached_level_end )
	{

		Game::instance (). transitionTo = Game::GStates::LEVEL_TWO;
		Game::instance (). setState ( Game::GStates::TRANSITION );
		return;

	} else {
		// Do nothing.
	}
}

// Update the potion collision on enemies.
void LevelOne::updatePotionEnemyCollision ( )
{
	// Updating the potion/enemy collision.
	for ( auto potion : this -> player -> potions )
	{
		for ( auto enemy : this -> enemies )
		{

			if ( Collision::rects_collided ( potion -> get_bounding_box (),
				enemy -> get_bounding_box () ) )
			{

				if ( potion -> activated )
				{

					if ( enemy -> life > 0 && this -> player -> can_attack )
					{

						enemy -> life -= 100;
						potion -> activated = false;

					}
					// Log(DEBUG) << "Enemy Life = " << enemy->life;

					if ( enemy -> life <= 0 )
					{

						enemy -> changeState ( Enemy::EStates::DEAD );

					}
				}
			}
		}
	}

}

// Update the attack collision on enemies.
void LevelOne::updateAttackEnemiesCollision ( )
{
	// Updating the player attack/enemy collision.
	for ( auto enemy : this -> enemies )
	{

		if ( Collision::rects_collided ( this -> player -> get_bounding_box (),
			enemy -> get_bounding_box () ) )
		{

			if ( this -> player -> is_right != enemy -> is_right )
			{

				if ( this -> player -> is_current_state ( Player::player_states::ATTACK ) ||
					this -> player -> is_current_state ( Player::player_states::ATTACKMOVING ) )
				{

					if ( enemy -> life > 0 && this -> player -> can_attack )
					{

						enemy -> life -= this -> player -> attack_strength;
						this -> player -> can_attack = false;

					}

					// Log(DEBUG) << "Enemy Life = " << enemy->life;

					if ( enemy -> life <= 0 )
					{

						enemy -> changeState ( Enemy::EStates::DEAD );

					}
				}
			}
		}
	}

}

// Update the checkpoints from Level 1.
void LevelOne::updateCheckpoints()
{
	double player_x = this -> player -> get_bounding_box (). x;
	double player_y = this -> player -> get_bounding_box (). y;

	// Updating the checkpoints.
	for ( int j = 0; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
	{

		if ( !this -> checkpoints_visited [ j ] && player_x >= checkpoints_X [ j ] && player_x <= checkpoints_X[j]
		    + 100 && player_y >= checkpoints_Y [ j ] && player_y <= checkpoints_Y [ j ] + 200 )
		{

			this -> checkpoints [ j ] = Game::instance (). getResources (). get (
			"res/images/checkpoint_visited.png" );

			Game::instance (). get_saves ().saveLevel ( 1, this -> player, this -> enemies,
			Game::instance (). current_slot );

			this -> checkpoints_visited [ j ] = true;

		}
	}
}

// Update Documents from Level 1.
void LevelOne::updateDocuments( )
{
	// Checks the condition of the document to render
	for ( auto document : this -> documents )
	{

		if ( Collision::rects_collided ( this -> player -> get_bounding_box (),
			document -> get_bounding_box () ) )
		{
			document -> should_render = true;
		}else
		{
			document -> should_render = false;
		}
	}
}

/**
* Updates the objects within the Level.
* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other.
*/
void LevelOne::update ( const double DELTA_TIME )
{

	updateEntities( DELTA_TIME );

	updateEnemies( DELTA_TIME );

	// Set to GameOver if the player is dead.
	if ( this -> player -> isDead () )
	{

		this -> player -> changeState ( Player::player_states::DEAD );

		ok = ok + DELTA_TIME;

		if( ok > 3 )
		{

			Game::instance (). setState ( Game::GStates::GAMEOVER );

		}

		return;

	}

	updatePotions();

	updatePlayer();

	updatePotionEnemyCollision();

	updateAttackEnemiesCollision();

	updateCheckpoints();

	updateDocuments();
}

void LevelOne::renderTileMap ( )
{
	const int CAMERA_X = this -> camera -> getClip (). x;
	const int CAMERA_Y = this -> camera -> getClip (). y;

	this -> background -> render ( 0, 480 - CAMERA_Y );
	this -> backgroud_top -> render( -CAMERA_X / 10, 1165 - CAMERA_Y );
	assert( this -> background != nullptr );
	assert( this -> backgroud_top != nullptr );

	// Render the tiles in the TileMap.
	this -> tile_map -> render ( CAMERA_X, CAMERA_Y );

	for ( int j = 0; j < this -> TOTAL_NUMBER_OF_CHECKPOINTS; ++j )
	{
		this -> checkpoints [ j ] -> render ( this -> checkpoints_X [ j ] - CAMERA_X,
		this -> checkpoints_Y [ j ] - CAMERA_Y );
	}

	this -> player_Hud -> render ();

	for ( auto enemy : this -> enemies )
	{
		enemy -> render ( CAMERA_X, CAMERA_Y );
	}
}

void LevelOne::renderEntities ( )
{
	const int CAMERA_X = this -> camera -> getClip (). x;
	const int CAMERA_Y = this -> camera -> getClip (). y;

	// Render all the entities in the list.
	for ( auto entity : this -> entities )
	{
        entity -> render ( CAMERA_X, CAMERA_Y );
	}

	for ( unsigned int i = 0; i < 4; i++ )
	{

		if ( this -> image != nullptr && caught_items [ i ] == false )
		{

			this -> image -> Sprite::render ( ( items [ 0 ] [ i ] + 60 ) - CAMERA_X,
				( ( items [ 1 ] [ i ] ) - CAMERA_Y ) );

		} else {
			// Do nohing;
		}
	}

}

void LevelOne::renderDocuments( )
{
	const int CAMERA_X = this -> camera -> getClip (). x;
	const int CAMERA_Y = this -> camera -> getClip (). y;

	// Render the document text
	for ( auto document : this -> documents )
	{

		document -> render ( CAMERA_X, CAMERA_Y );

		if ( document -> should_render )
		{
			document -> renderDocumentText ();
		} else {
			// Do nothing .
		}
	}
}

/*
Renders the level.
Always renders on 0,0 position.
*/
void LevelOne::render ()
{

	renderTileMap();

	renderEntities();

	renderDocuments();

}
