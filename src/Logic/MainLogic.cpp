#include "MainLogic.h"
#include "../UI/StartScene.h"
#include <windows.h>
#include <thread>
#include <chrono>

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);






namespace UI
{

	std::ofstream MainLogic::logFile(logFileName
		, std::ios::out);
	MainLogic::GameState MainLogic::gameState = GameState::GAME_NOT_START;
	int MainLogic::gameRound = 0;
	LONGLONG MainLogic::delayPerRound = 100;
	
	int MainLogic::playerAlive=4;
	

	std::string MainLogic::loadFileName = "../result.txt";
	std::vector<std::stringstream*> MainLogic::loadFileStream = *(new std::vector<std::stringstream*>());
	std::vector<UI::UIObject*>MainLogic::uiObjects = *(new std::vector<UI::UIObject*>());
	std::vector<UI::TTower*>MainLogic::towers = *(new std::vector<UI::TTower*>());
	std::vector<UI::TSoldier*>MainLogic::soldiers = *(new std::vector<UI::TSoldier*>());
	std::vector<UI::TPlayer*>MainLogic::players = *(new std::vector<UI::TPlayer*>());
	
	std::string MainLogic::mapFile = "gameMap.png";

	MainLogic::MainLogic()
	{
		//Start Scene
		
		

	}
	

	MainLogic::~MainLogic()
	{
#if USE_AUDIO_ENGINE
		AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
		SimpleAudioEngine::end();
#endif
		MainLogic::logFile.close();
	}

	void MainLogic::initGLContextAttrs()
	{
	}

	bool MainLogic::applicationDidFinishLaunching()
	{
		auto director = Director::getInstance();
		auto glview = director->getOpenGLView();
		if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
			glview = GLViewImpl::createWithRect("FC16UI1.0", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
			glview = GLViewImpl::create("FC16UI1.0");
#endif
			director->setOpenGLView(glview);
		}

		// turn on display FPS
		director->setDisplayStats(true);

		// set FPS. the default value is 1.0/60 if you don't call this
		director->setAnimationInterval(1.0f / 60);

		// Set the design resolution
		glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
		auto frameSize = glview->getFrameSize();
		// if the frame's height is larger than the height of medium size.
		if (frameSize.height > mediumResolutionSize.height)
		{
			director->setContentScaleFactor(MIN(largeResolutionSize.height / designResolutionSize.height, largeResolutionSize.width / designResolutionSize.width));
		}
		// if the frame's height is larger than the height of small size.
		else if (frameSize.height > smallResolutionSize.height)
		{
			director->setContentScaleFactor(MIN(mediumResolutionSize.height / designResolutionSize.height, mediumResolutionSize.width / designResolutionSize.width));
		}
		// if the frame's height is smaller than the height of medium size.
		else
		{
			director->setContentScaleFactor(MIN(smallResolutionSize.height / designResolutionSize.height, smallResolutionSize.width / designResolutionSize.width));
		}

		auto scene = StartScene::createScene();
		


		director->runWithScene(scene);
		return true;
	}

	void MainLogic::applicationDidEnterBackground()
	{
		Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
		AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
		SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
		SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
	}


	void MainLogic::applicationWillEnterForeground()
	{
		Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
		AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
		SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
	}

	void MainLogic::GameStart()
	{
		if (MainLogic::gameState != GameState::GAME_NOT_START)
			return;

		MainLogic::gameRound = 0;
		MainLogic::gameState = GameState::GAME_RUNNING;
		MainLogic::clearData();
		while (MainLogic::gameRound < MAX_ROUND)
		{
			MainLogic::GameLoop();
		};
		MainLogic::GameOver();
	}

	void MainLogic::GameLoop()
	{
		if (MainLogic::gameState != GameState::GAME_RUNNING)
		{
			return;
		}
		else
		{
			MainLogic::LogicUpdate();
			MainLogic::UIUpdate();
			std::this_thread::sleep_for(std::chrono::milliseconds(delayPerRound));
			
		}
	}

	void MainLogic::GameOver()
	{
		if (MainLogic::gameState == GameState::GAME_NOT_START)
		{
			return;
		}

		else
		{
			MainLogic::gameRound = 0;
			MainLogic::gameState = GameState::GAME_NOT_START;
			MainLogic::clearData();
		}

	}

	void MainLogic::GamePause()
	{
		if (MainLogic::gameState != GameState::GAME_RUNNING)
			return;
		else
		{
			MainLogic::gameState = GameState::GAME_PAUSE;
		}
	}

	void MainLogic::GameResume()
	{
		if (MainLogic::gameState != GameState::GAME_PAUSE)
			return;
		else
		{
			MainLogic::gameState = GameState::GAME_RUNNING;
		}
	}

	std::string MainLogic::getFileDialogName()
	{
		OPENFILENAME ofn;       // common dialog box structure
		char szFile[260];       // buffer for file name
		HWND hwnd = GetActiveWindow();              // owner window
		HANDLE hf;              // file handle

								// Initialize OPENFILENAME
		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = hwnd;
		ofn.lpstrFile = LPWSTR(szFile);
		// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
		// use the contents of szFile to initialize itself.
		ofn.lpstrFile[0] = '\0';
		ofn.nMaxFile = sizeof(szFile);
		ofn.lpstrFilter = LPCWSTR("All\0*.*\0Text\0*.TXT\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box. 

		if (GetOpenFileName(&ofn) == TRUE)
		{
			hf = CreateFile(ofn.lpstrFile,
				GENERIC_READ,
				0,
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				(HANDLE)NULL);
			//MainLogic::WriteLog("Successfully opened the file");
			//MainLogic::WriteLog("the lpstrFileTitle is ");
			//MainLogic::WriteLog((char*)ofn.lpstrFileTitle);
			//MainLogic::WriteLog("the initial dir is ");
			//MainLogic::WriteLog((char*)ofn.lpstrInitialDir);

		}
		else
			MainLogic::WriteLog("Fail to open the file");
		return std::string((char*)ofn.lpstrFile);
	}

	void MainLogic::LoadData()
	{
		loadFileName = getFileDialogName();
		MainLogic::WriteLog("Loading file from  " + loadFileName);
	/*	if (!loadFileName.size())
		{
			return;
		}
		std::ifstream loadFile(loadFileName, std::ios::in);
		std::string temp;
		std::stringstream* tempStream = new std::stringstream();
		
		loadFile >> temp;
		for(int i = 1;i<=MAX_ROUND;i++)
		{
			
			tempStream->clear();
			loadFile >> temp;
			(*tempStream) << " Round " << temp;
			while (loadFile >> temp)
			{
				if (temp == "Round")
					break;
				(*tempStream) <<""<< temp<<" ";

			};
			loadFileStream.push_back(tempStream);
		}
		loadFile.close();*/
	}

	void MainLogic::LogicUpdate()
	{/*
		std::stringstream tempStream = this->loadFileStream[MainLogic::gameRound];
		tempStream >> "Round" >> MainLogic::gameRound;
		tempStream >> "PlayerAlive:" >> playerAlive;
		int playerID;
		for (int i = 0; i < PLAYER_NUM; i++)
		{
			tempStream >> "Player" >> playerID >> "Info";
			tempStream >> "Rank" >> players[playerID]->rank;
			tempStream >> "Score" >> players[playerID]->score;
			tempStream >> "KillNum" >> players[playerID]->killNum;
			tempStream >> "TowerNum" >> players[playerID]->towerNum;
			tempStream >> "SurvivalRound" >> players[playerID]->survivalRound;
			tempStream >> "SoldierNum" >> players[playerID]->soldierNum;
			tempStream >> "Resources" >> players[playerID]->resource;
			tempStream >> "MaxPopulation" >> players[playerID]->maxPopulation;
			tempStream >> "Population" >> players[playerID]->population;
		}
		tempStream >> "TowerInfo";
		int towerID;
		int ownerID;
		int towerLevel;
		while (tempStream >> "TowerID")
		{
			tempStream >> towerID;
			tempStream >> "Owner" >> ownerID;
			if (ownerID == -1)
				towers[towerID]->owner = nullptr;
			else
				towers[towerID]->owner = players[ownerID];
			tempStream >> "Level" >> towerLevel;
			towers[towerID]->level = TTower::TowerLEVEL(towerLevel);
			tempStream >> "Blood" >> towers[towerID]->blood;
			tempStream >> "Recruiting" >> towers[towerID]->recruiting;
			tempStream >> "Recruiting" >> towers[towerID]->recruitingRound;
			tempStream >>"RecruitingType">>towers[towerID]->

		}
		*/
	}

	void MainLogic::UIUpdate()
	{
	}

	void MainLogic::clearData()
	{
		loadFileName = "";
		loadFileStream.clear();
		loadFileStream.resize(MAX_ROUND);
					
	}


	void MainLogic::WriteLog(std::string  message)
	{
		logFile << message << std::endl;
	}
}