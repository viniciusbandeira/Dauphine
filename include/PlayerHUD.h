#ifndef INCLUDE_PLAYERHUD_H
#define INCLUDE_PLAYERHUD_H

#include "Sprite.h"
#include "Player.h"
#include "Text.h"

/**
* The HUD entity class.
* Contains all the relevant implementation relative to the HUD.
*/
class PlayerHUD
{

	public:

		/**
		* The constructor.
		* Creates the HUD by setting the position and sprite.
		* @param x_ : position in x axis.
		* @param y_ : position in y axis.
		* @param sprite_ : which sprite to use.
		*/
		PlayerHUD ( Player *const player_ );

		/**
		* The destructor.
		* Exits the current state and destroys all states.
		*/
		virtual ~PlayerHUD ();

		/**
		* Updates the HUD.
		* @see Player::updateInput, Player::updatePosition
		* @param DELTA_TIME : Delta time. Time elapsed between one frame and the other, independent
		* 	of processing speed.
		*/
		void update ();

		/**
		* Renders the HUD.
		* Uses the HUD's sprite render method.
		* @see Sprite::render
		* @param cameraX_ : The x position of the camera.
		* @param cameraY_ : The y position of the camera.
		*/
		void render ();

		void restart_hud_sprites();

		void restart_is_can_render_hud();

		void render_hud_sprites();

		void render_potions_left();

	private:
		enum HUD_Elements : uint8_t
		{
			HEALTH_0 = 0,
			HEALTH_33,
			HEALTH_66,
			HEALTH_100,
			PLAYER_ICON,
			TOTAL_HUD
		};

		void initializeSprites ();

		Sprite *player_hud_sprites [ TOTAL_HUD ]; // Sprites player.
		bool is_can_render_hud [ TOTAL_HUD ]; // Boolean array to verify if the hud can be rendered.
		Player *player; // Instante of a player.
		Text *potions_left; // Reference to the potions left.
};

#endif //INCLUDE_PLAYERHUD_H
