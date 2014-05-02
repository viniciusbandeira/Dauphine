#include "LevelOne.h"
#include "Game.h"
#include "LuaScript.h"
#include "Logger.h"

#include "TileMap.h"

#include <fstream>

LevelOne::LevelOne() :
	Level()
{

}

LevelOne::~LevelOne(){

}

void LevelOne::load(){
	Logger::verbose("Loading level 1...");

	// Changing the music.
	Game::instance().getAudioHandler().changeMusic("res/idk.ogg");

	// Getting information from lua script.
	LuaScript luaLevel1("lua/Level1.lua");
	const std::string scriptPlayerSpritePath = luaLevel1.unlua_get<std::string>(
		"level.player.spritePath");
	const std::string scriptBackgroundSpritePath = luaLevel1.unlua_get<std::string>(
		"level.background.spritePath");
	const double scriptX = luaLevel1.unlua_get<double>("level.player.position.x");
	const double scriptY = luaLevel1.unlua_get<double>("level.player.position.y");
	const unsigned int levelW = luaLevel1.unlua_get<int>("level.boundaries.width");
	const unsigned int levelH = luaLevel1.unlua_get<int>("level.boundaries.height");

	// Loading the sprites.
	Sprite* spritePlayer = nullptr;
	spritePlayer = new Sprite(scriptPlayerSpritePath);

	// Loading the player and the camera.
	Player* lPlayer = new Player(scriptX, scriptY, spritePlayer);
	lPlayer->getSprite()->animation->setParameters(0, 0, lPlayer->getWidth(), lPlayer->getHeight(), 11, true);
	Camera* lCamera = new Camera(lPlayer);

	this->width = levelW;
	this->height = levelH;

	// Loading the tile/tilemap.
	std::ifstream mapFile("res/maps/level1.map");

	int numberOfTiles = 0;
	mapFile >> numberOfTiles;
	std::vector<int> tileData;
	for(int i = 0; i < numberOfTiles; i++){
		int tileType = 0;
		mapFile >> tileType;
		tileData.push_back(tileType);
	}

	TileMap* tileMap = new TileMap(tileData);

	addEntity(tileMap);
	lPlayer->setTiles(tileMap->tiles);

	setPlayer(lPlayer);
	setCamera(lCamera);
	Logger::verbose("Done loading level 1...");
}

void LevelOne::unload(){
	Logger::verbose("Unloading level 1...");
	cleanEntities();
}

void LevelOne::update(double dt_){
	// Update all the entities in the list.
	for(auto entity : entities){
        entity->update(dt_);
	}
	this->player->getSprite()->animation->update(this->player->clip, dt_, 5);
	this->camera->update();
}

void LevelOne::render(){
	const int cameraX = this->camera->getClip().x;
	const int cameraY = this->camera->getClip().y;

	// Render all the entities in the list.
	for(auto entity : entities){
        entity->render(cameraX, cameraY);
	}
}

void LevelOne::setPlayer(Player* player_){
	this->player = player_;

	if(this->player != nullptr){
		this->player->setLevelWH(this->width, this->height);
		addEntity(this->player);
	}
	else{
		Logger::warning("Setting a null player!");
	}
	
}

void LevelOne::setCamera(Camera* camera_){
	this->camera = camera_;

	if(this->camera != nullptr){
		if(this->player != nullptr){
			this->camera->setLevelWH(this->width, this->height);
		}
		else{
			Logger::warning("Shouldn't set the camera before the player, in Level!");
		}
	}
	else{
		Logger::warning("Setting a null camera!");
	}

}
