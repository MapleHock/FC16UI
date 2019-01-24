#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "cocos2d.h"

namespace UI
{
	extern std::string logFileName;
	extern int MAX_ROUND;
	extern int PLAYER_NUM;
	
	class MainLogic;
	class StartScene;
	class PlayScene;
	
	
	enum MoveDirection
	{
		UP = 1
		, DOWN = 2
		, LEFT = 3
		, RIGHT = 4
	};

	MoveDirection moveDirStr2Enum(std::string str);

	class UIObject
	{
	public:
		enum UIObjectType
		{
			NoneObjectType,
			PlayerType,
			TowerType,
			SoldierType
		};
		UIObjectType uiObjectType;
		bool visiable;
		UIObject(UIObjectType uiObjectType = NoneObjectType, bool visiable = true);
		//UIObject() {};
		~UIObject();
		
		virtual void clear() = 0;
		virtual void UIUpdate() = 0;
	};

	class TPlayer;
	class TTower;
	class TSoldier;

	class TPlayer: public UIObject
	{
	public:
		int ID;
		std::vector<TTower*>Halls;
		std::vector<TSoldier*>Soldiers;
		int rank;
		int resource;
		int score;
		int killNum;
		int towerNum;
		int survivalRound;
		int maxPopulation;
		int population;
		int soldierNum;

		TPlayer();
		~TPlayer();
		virtual void clear();
		virtual void UIUpdate();
	};

	class TSoldier : public UIObject
	{
	public:
		enum SoldierType
		{
			LightInfantry = 1  //轻步兵
			, LightArcher = 2   //轻弓兵
			, LightKnight = 3   //轻骑兵
			, Mangonel = 4      //投石机
			, HeavyInfantry = 5 //重步兵
			, HeavyArcher = 6   //重弓兵
			, HeavyKnight = 7   //重骑兵
			, NoneSoldierType
		};
		struct SoldierMoveType
		{
			bool move;
			MoveDirection moveDirection;
			int moveDistance;
		};


		int ID;
		TPlayer* owner;
		int blood;
		SoldierType type;
		cocos2d::Vec2 position;
		int level;

		SoldierMoveType soldierMove;
		UIObject* victim;

		TSoldier();
		TSoldier(int soldierID, int owner, std::string type, int level, int blood, int x_position, int y_position);
		~TSoldier();
		virtual void clear();
		virtual void UIUpdate();

    int Info2GID();

	};

	TSoldier::SoldierType SoldierTypeStr2Enum(std::string str);

	class TTower: public UIObject
	{
	public:
		enum TowerLEVEL 
		{
			LV1 = 0, 
			LV2 = 1,
			LV3 = 2
		};
		struct ProduceType
		{
			bool is_produce;
			TSoldier::SoldierType soldierType;
		};
		ProduceType produceSoldier;
		int ID;
		TPlayer* owner;
		TowerLEVEL level;
		int blood;
		
		int recruiting;
		TSoldier::SoldierType recruitingType;
		int recruitingRound;
		bool upgrade; //
		

		cocos2d::Vec2 position;

		TTower();
		TTower(int towerID, int owner, int level, int blood, int recruiting, int recruitingRound, std::string recruitingType);
		~TTower();
		virtual void clear();
		virtual void UIUpdate();

		
	};

	

	class Command
	{
	public:
		enum CommandType
		{
			Attack       //攻击(士兵id，目标点x，目标点y)
			, Move       //移动（士兵id，方向，移动距离）
			, Upgrade    //升级（食堂id）
			, Produce    //生产（食堂id，士兵类型）
		};
		
		CommandType type;

		
		//Move
		UIObject* moveSoldier;
		MoveDirection moveDirection;
		int moveDistance;

		//Attack
		UIObject* attackObject;
		UIObject* victimObject;

		Command();
		~Command();
	};


	enum TLandForm
	{
		Road      //道路
		, Forest   //森林
		, River    //河流
		, Mountain //山地
		, Dorm     //宿舍
		, Classroom//教室
	};
	enum TOccupiedType
	{
		NoneOccupied      //没被占据
		, SoldierOccupied  //士兵
		, TowerOccupied    //塔
		, UnKnownOccupied  //视野之外
	};
	
	struct TPoint
	{
		
	//	TPosition x;
	//	TPosition y;
		TLandForm land;
		bool occupied;
		bool visible;//是否可见
		TOccupiedType occupied_type;//对应的占据类型
	//	TSoldierID soldier;//对应的soldier
	//	TTowerID tower;//对应的tower

		bool player0_visible;
		bool player1_visible;
		bool player2_visible;
		bool player3_visible;
	};

}