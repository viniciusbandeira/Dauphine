#include "EStateAtack.h"
#include "Logger.h"

double atackTime;

void EStateAtack::enter(){
	Log(DEBUG) << "ENTER ATACK";
	this->enemy->getAnimation()->changeAnimation(2, 1, 6, false, 0.6);
	atackTime = 0;
}

void EStateAtack::exit(){

}

void EStateAtack::update(const double dt_){
	atackTime += dt_;
	const double atackTotalTime = 0.6;

	this->enemy->vx = 0;
	if(atackTime > atackTotalTime){
		Enemy::pLife--;
		this->enemy->changeState(Enemy::EStates::ALERT);
	}
}

EStateAtack::EStateAtack(Enemy* const enemy_) :
	StateEnemy(enemy_)
{

}