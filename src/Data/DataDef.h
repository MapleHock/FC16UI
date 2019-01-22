#pragma once
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include "cocos2d.h"

namespace UI
{
	extern std::string logFileName;
	extern const int MAX_ROUND;
	extern const int PLAYER_NUM;
	
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
			LightInfantry = 1  //�Ჽ��
			, LightArcher = 2   //�ṭ��
			, LightKnight = 3   //�����
			, Mangonel = 4      //Ͷʯ��
			, HeavyInfantry = 5 //�ز���
			, HeavyArcher = 6   //�ع���
			, HeavyKnight = 7   //�����
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

		SoldierMoveType soldierMove;
		UIObject* victim;

		TSoldier();
		~TSoldier();
		virtual void clear();
		virtual void UIUpdate();


	};

	class TTower: public UIObject
	{
	public:
		enum TowerLEVEL 
		{
			LV1 = 0, 
			LV2 = 1,
			LV3 = 2
		};
		int ID;
		TPlayer* owner;
		TowerLEVEL level;
		int blood;
		
		int recruiting;
		TSoldier::SoldierType recruitingType;
		int recruitingRound;

		cocos2d::Vec2 position;

		TTower();
		~TTower();
		virtual void clear();
		virtual void UIUpdate();

		bool upgrade; //
	};

	

	class Command
	{
	public:
		enum CommandType
		{
			Attack       //����(ʿ��id��Ŀ���x��Ŀ���y)
			, Move       //�ƶ���ʿ��id�������ƶ����룩
			, Upgrade    //������ʳ��id��
			, Produce    //������ʳ��id��ʿ�����ͣ�
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
		Road      //��·
		, Forest   //ɭ��
		, River    //����
		, Mountain //ɽ��
		, Dorm     //����
		, Classroom//����
	};
	enum TOccupiedType
	{
		NoneOccupied      //û��ռ��
		, SoldierOccupied  //ʿ��
		, TowerOccupied    //��
		, UnKnownOccupied  //��Ұ֮��
	};
	
	struct TPoint
	{
		
	//	TPosition x;
	//	TPosition y;
		TLandForm land;
		bool occupied;
		bool visible;//�Ƿ�ɼ�
		TOccupiedType occupied_type;//��Ӧ��ռ������
	//	TSoldierID soldier;//��Ӧ��soldier
	//	TTowerID tower;//��Ӧ��tower

		bool player0_visible;
		bool player1_visible;
		bool player2_visible;
		bool player3_visible;
	};

}