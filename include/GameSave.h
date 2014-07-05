#ifndef INCLUDE_GAMESAVE_H
#define INCLUDE_GAMESAVE_H

#include <fstream>
#include "Player.h"
#include "Enemy.h"

class GameSave {

	public:

		static GameSave& instance();
		GameSave();

		void createSaveGameFile(int saveSelection_);
		void saveLevel(unsigned int level_, Player* player, std::vector <Enemy*> enemies);
		int getSavedLevel(int continueSelection_);
		void restorePlayerPosition(Player* player);

		int saveSelection;
		unsigned int currentLevel;

		std::ofstream saveFile;
		std::ifstream continueFile;
};

#endif //INCLUDE_GAMESAVE_H