#include "LevelOne.h"
#include "AudioHandler.h"
#include "LuaScript.h"
#include "Logger.h"

#include "Tile.h"

LevelOne::LevelOne() :
	Level()
{

}

LevelOne::~LevelOne(){

}

void LevelOne::update(double dt_){
	// Update all the entities in the list.
	for(Entity* entity : entities){
        entity->update(dt_);
	}

	this->camera->update();
}

void LevelOne::render(){
	if(this->background != nullptr){
		this->background->render(0, 0, &this->camera->getClip());
	}
	else{
		Logger::warning("No background set for this Level!");
	}

	const int cameraX = this->camera->getClip().x;
	const int cameraY = this->camera->getClip().y;

	// Render all the entities in the list.
	for(Entity* entity : entities){
        entity->render(cameraX, cameraY);
	}
}

void LevelOne::load(){
	Logger::verbose("Loading level 1...");

	AudioHandler* audioHandler = AudioHandler::getInstance();
	audioHandler->changeMusic("res/idk.ogg");

	// Getting information from lua script
	LuaScript luaLevel1("lua/Level1.lua");
	const string scriptPlayerSpritePath = luaLevel1.unlua_get<string>(
		"level.player.spritePath");
	const string scriptBackgroundSpritePath = luaLevel1.unlua_get<string>(
		"level.background.spritePath");
	const double scriptX = luaLevel1.unlua_get<double>("level.player.position.x");
	const double scriptY = luaLevel1.unlua_get<double>("level.player.position.y");
	const unsigned int levelW = luaLevel1.unlua_get<int>("level.boundaries.width");
	const unsigned int levelH = luaLevel1.unlua_get<int>("level.boundaries.height");

	// Just an example of Sprite loading, delete this later.
	Sprite* spriteLevelBackground = nullptr;
	spriteLevelBackground = new Sprite(scriptBackgroundSpritePath);

	Sprite* spritePlayer = nullptr;
	spritePlayer = new Sprite(scriptPlayerSpritePath);
	
	Player* lPlayer = new Player(scriptX, scriptY, spritePlayer);
	Camera* lCamera = new Camera(lPlayer);

	this->background = spriteLevelBackground;
	if(this->background == nullptr){
		Logger::warning("Level background is null! No background will be set.");
	}

	this->width = levelW;
	this->height = levelH;

	setPlayer(lPlayer);
	setCamera(lCamera);

	// Tile example (remove).
	Sprite* tileSprite = new Sprite("res/green_tile.png");
	Tile* tile = new Tile(750, 600, tileSprite);
	addEntity(tile);
}

void LevelOne::unload(){
	Logger::verbose("Unloading level 1...");

	if(this->camera != nullptr){
		delete this->camera;
		this->camera = nullptr;
	}
	
	if(this->player != nullptr){
		delete this->player;
		this->player = nullptr;
	}

	if(this->background != nullptr){
		this->background->free();
		delete this->background;
		this->background = nullptr;
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
