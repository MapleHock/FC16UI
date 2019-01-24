#include "DataDef.h"
#include "../Logic/MainLogic.h"

namespace UI
{
	int PLAYER_NUM = 4;
	int MAX_ROUND = 300;
	std::string logFileName = "Log/log.txt";
	
	UIObject::UIObject(UIObjectType uiObjectType, bool visiable):uiObjectType(uiObjectType), visiable(visiable){}

	UIObject::~UIObject()
	{
	}
	
	
	TTower::TTower() :UIObject(UIObject ::UIObjectType::TowerType)
	{
	}

	TTower::TTower(int towerID, int owner, int level, int blood, int recruiting, int recruitingRound, std::string recruitingType):UIObject(UIObjectType::TowerType)
	{
		this->ID = towerID;
		if(owner!=-1)
			this->owner = MainLogic::players[owner];
		else
		{
			this->owner = nullptr;
		}
		this->level = TTower::TowerLEVEL(level);
		this->blood = blood;
		this->recruiting = recruiting;
		this->recruitingRound = recruitingRound;
		this->recruitingType = SoldierTypeStr2Enum(recruitingType);

		this->upgrade = false;
		this->produceSoldier = ProduceType{ false, TSoldier::NoneSoldierType };
	}

	TTower::~TTower()
	{
	}

	void TTower::clear()
	{
	}

	void TTower::UIUpdate()
	{
	}

	TSoldier::TSoldier() :UIObject(UIObject::UIObjectType::SoldierType)
	{
	}

	TSoldier::TSoldier(int soldierID, int owner, std::string type, int level, int blood, int x_position, int y_position)
	{
		this->ID = soldierID;
		this->owner = MainLogic::players[owner];
		this->type = SoldierTypeStr2Enum(type);
		this->level = level;
		this->blood = blood;
		this->position = cocos2d::Vec2(x_position, y_position);
		this->soldierMove = SoldierMoveType{ false, UP, 0 };
		this->victim = nullptr;
	}

	TSoldier::~TSoldier()
	{
	}

	void TSoldier::clear()
	{
	}

	void TSoldier::UIUpdate()
	{
	}

  int TSoldier::Info2GID() {
    return 0;
  }

	TPlayer::TPlayer() :UIObject(UIObject::UIObjectType::PlayerType)
	{
	}

	TPlayer::~TPlayer()
	{
	}

	void TPlayer::clear()
	{
	}

	void TPlayer::UIUpdate()
	{
	}


	TSoldier::SoldierType SoldierTypeStr2Enum(std::string str)
	{
		if (str == "LightInfantry")
			return TSoldier::LightInfantry;
		else if (str == "LightArcher")
			return TSoldier::LightArcher;
		else if (str == "LightKnight")
			return TSoldier::LightKnight;
		else if (str == "Mangonel")
			return TSoldier::Mangonel;
		else if (str == "HeavyInfantry")
			return TSoldier::HeavyInfantry;
		else if (str == "HeavyArcher")
			return TSoldier::HeavyArcher;
		else if (str == "HeavyKnight")
			return TSoldier::HeavyKnight;
		else
			return TSoldier::NoneSoldierType;
	}
	MoveDirection moveDirStr2Enum(std::string str)
	{
		if (str == "UP")
			return MoveDirection::UP;
		else if (str == "DOWN")
			return MoveDirection::DOWN;
		else if (str == "LEFT")
			return MoveDirection::LEFT;
		else //if (str == "RIGHT")
			return MoveDirection::RIGHT;
	}
}


