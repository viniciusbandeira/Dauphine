
#ifndef INCLUDE_TILEMAP_H
#define INCLUDE_TILEMAP_H

#include "../lib/TmxParser/Tmx.h"
#include "Sprite.h"
#include <vector>
#include "CollisionRect.h"

const int TILE_SIZE = 64;

/**
* Represents the tile distrubution for a level.
* @todo Revise Tile placement implementation.
*/
class TileMap
{

	public:
		/**
		* The constructor.
		* @param mapPath_ : Path to the desired Tiled map.
		*/
		TileMap( const std::string &mapPath_ );

		/**
		* The destructor.
		*/
		~TileMap();

		/**
		* Renders the TileMap.
		* 
		* @param camera_position_x : The x position of the camera.
		* @param camera_position_y : The y position of the camera.
		*/
		void render( const double camera_position_x, const double camera_position_y );

		unsigned int get_map_width();
		unsigned int get_map_height();
		std::vector <CollisionRect> &get_collision_rects();

		double get_initial_x();
		double get_initial_y();

		std::vector<int> &get_enemies_x();
		std::vector<int> &get_enemies_y();
		std::vector<bool> &get_enemies_patrol();		

	private:
		/**
		*
		*/
		void load( const std::string& mapPath_ );

		/**
		*
		*/
		void addTileSet( const std::string& PATH );

		/**
		* Renders a certain layer from the TileMap.
		* 
		* @param camera_position_x : The x position of the camera.
		* @param camera_position_y : The y position of the camera.
		*/
		void renderLayer( const double camera_position_x, const double camera_position_y, const unsigned int layer_ );

		Tmx::Map *map;

		unsigned int layers;
		unsigned int mapWidth;
		unsigned int mapHeight;
		unsigned int initial_x;
		unsigned int initial_y;

		std::vector <std::vector <std::vector <unsigned int>>> tileMatrix; /**< Three-dimensional
			matrix, that contains x = width, y = height, z = layers */

		std::vector <Sprite*> tilesetSprites;

		std::vector <CollisionRect> collisionRects;

		std::vector<int> enemiesX;
		std::vector<int> enemiesY;
		std::vector<bool> enemiesPatrol;

};

#endif // INCLUDE_TILEMAP_H
