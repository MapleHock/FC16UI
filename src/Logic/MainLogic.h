#pragma once

#include "cocos2d.h"
#include "../Data/DataDef.h"

namespace UI {
	

	class MainLogic : public cocos2d::Application
	{
		enum GameState
		{
			GAME_NOT_START,
			GAME_RUNNING,
			GAME_PAUSE
		};
	public:
		static GameState MainLogic::gameState;
		static int MainLogic::gameRound;
		static int playerAlive;
		static LONGLONG delayPerRound;	//ms
	public:
		static std::ofstream logFile;
		
		static std::string loadFileName;
		static FILE* loadFilePtr;


		

		static std::vector<UI::TTower*>towers;
		static std::vector<UI::TSoldier*>soldiers;
		static std::vector<UI::TPlayer*>players;
		static std::string mapFile;
	public:
		


		MainLogic();
		virtual ~MainLogic();

		virtual void initGLContextAttrs();


		virtual bool applicationDidFinishLaunching();
		virtual void applicationDidEnterBackground();
		virtual void applicationWillEnterForeground();

		static void GameStart();
		static void GameLoop();
		static void GameOver();
		static void GamePause();
		static void GameResume();
		 
		static std::string getFileDialogName();
		static void LoadData();
		static void LogicUpdate();
		static void UIUpdate(cocos2d::Scene* playscene);
		static void clearData();
		static void WriteLog(std::string message);

    static void StartScene2PlayScene();
	};

}