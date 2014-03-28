#ifndef INCLUDE_GAME_H
#define INCLUDE_GAME_H

#include "SDLWrapper.h"
#include "Window.h"

class Game {

	public:
		/**
		* The constructor.
		* Creates the window and the renderer.
		* @param lWindow : a created Window.
		*/
		Game(Window *lWindow);

		/**
		* The destructor.
		* Destroys the window and the renderer.
		*/
		~Game();
		
		/**
		* Handles the updates.
		* Updates all game objects. Is part of the main loop.
		* @params lDt : Delta time. Time elapsed between one frame and the other, independent of processing speed.
		* @see runGame()
		*/
		void update(double lDt);
		
		/**
		* The main game loop.
		* Orders the game structure, such as inputs, updates, and rendering.
		*/
		void runGame();

		FPSmanager fpsManager;

		bool isRunning;
		
		double dt;
		
		double x;
		
		double lastTime;
		double now;

	private:
		Window *gameWindow;

};

#endif //INCLUDE_GAME_H
