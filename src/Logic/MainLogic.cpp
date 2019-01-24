#include "MainLogic.h"
#include "../UI/StartScene.h"
#include "../UI/PlayScene.h"
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
	FILE* MainLogic::loadFilePtr = nullptr;
	
	std::vector<UI::TTower*>MainLogic::towers = *(new std::vector<UI::TTower*>());
	std::vector<UI::TSoldier*>MainLogic::soldiers = *(new std::vector<UI::TSoldier*>());
	std::vector<UI::TPlayer*>MainLogic::players = *(new std::vector<UI::TPlayer*>());
	
	std::string MainLogic::mapFile = "gameMap.png"; //TODO delete it ?

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

		/*while (MainLogic::gameRound < MAX_ROUND)
		{
			MainLogic::GameLoop();
		};*/

    MainLogic::GameLoop();
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
			MainLogic::UIUpdate(Director::getInstance()->getRunningScene());
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
		ofn.lpstrFilter = LPCWSTR("All\0*.*\0*.csv\0*.txt\0");
		ofn.nFilterIndex = 1;
		ofn.lpstrFileTitle = NULL;
		ofn.nMaxFileTitle = 0;
		ofn.lpstrInitialDir = NULL;
		ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

		// Display the Open dialog box.
		char filename[260] = "";

		if (GetOpenFileName(&ofn) == TRUE)
		{
			hf = CreateFile(ofn.lpstrFile,
				GENERIC_READ,
				0,
				(LPSECURITY_ATTRIBUTES)NULL,
				OPEN_EXISTING,
				FILE_ATTRIBUTE_NORMAL,
				(HANDLE)NULL);
			bool first = true;
			MainLogic::WriteLog("The szFile");
			
			int j = 0;
			for (int i = 0; i < 260; i++)
			{
				if (szFile[i] != '\0')
					filename[j++] = szFile[i];
				if (j > 4 && std::string(filename + j - 4, 4) == ".txt")
				{
					filename[j] = '\0';
					break;
				}
			}
		}
    
		return std::string(filename);
	}

	void MainLogic::LoadData()
	{
    // TODO fix the dialog open file and fileptr cannot open it
		/*loadFileName = getFileDialogName();
		MainLogic::WriteLog("Loading file from  " + loadFileName);
		if (!loadFileName.size())
		{
			return;
		}*/
		fopen_s(&MainLogic::loadFilePtr, "E:\\programming\\FC16UI\\FC16UI\\Debug.win32\\res\\Enresult.txt", "r");
		if (loadFilePtr == NULL)
		{
			return;
		}
	}

	void MainLogic::LogicUpdate()
	{
		FILE* fp = MainLogic::loadFilePtr;
		int playerID;
		int a, b, c, d, e, f, g, o, p;
		
		
		
		char h[30], m[30];
		if (fscanf_s(fp, "Round %d\n", &MainLogic::gameRound) != 2)
		{
			GameOver();
		}
		fscanf_s(fp, "PlayerAlive: %d\n", &MainLogic::playerAlive);
		
    //TODO no temp?
    if (MainLogic::players.size() == 0) {
      for (int i = 0; i < PLAYER_NUM; i++) {
        UI::TPlayer* temp = new UI::TPlayer();
        MainLogic::players.push_back(temp);
      }
    }

		for (int i = 0; i < PLAYER_NUM; i++)
		{
			fscanf_s(fp, "Player");
			fscanf_s(fp, "%d", &playerID);
			fscanf_s(fp, "Info\n");
			
			fscanf_s(fp, "Rank %d Score %d KillNum %d TowerNum %d SurvivalRound %d SoldierNum %d Resource %d MaxPopulation %d Population %d\n", &a, &b, &c, &d, &e, &f, &g, &o, &p);
			MainLogic::players[playerID]->rank = a;
			MainLogic::players[playerID]->score = b;
			MainLogic::players[playerID]->killNum = c;
			MainLogic::players[playerID]->towerNum = d;
			MainLogic::players[playerID]->survivalRound = e;
			MainLogic::players[playerID]->soldierNum = f;
			MainLogic::players[playerID]->resource = g;
			MainLogic::players[playerID]->maxPopulation = o;
			MainLogic::players[playerID]->population = p;
		}
		
		//Towers
		for (auto item : MainLogic::towers)
			delete item;
MainLogic::towers.clear();
TTower* newTower;

fscanf_s(fp, "TowerInfo\n");
while (fscanf_s(fp, "TowerID %d Owner %d Level %d Blood %d Recruiting %d RecruitingRound %d RecruitingType %s\n",
  &a, &b, &c, &d, &e, &f, h, 20) == 7) {
  newTower = new TTower(a, b, c, d, e, f, std::string(h));
  MainLogic::towers.push_back(newTower);
}
fscanf_s(fp, "SoldierInfo\n");
for (auto item : MainLogic::soldiers)
delete item;
MainLogic::soldiers.clear();
TSoldier* newSoldier = nullptr;
while (fscanf_s(fp, "SoldierID %d Owner %d Type %s Level %d Blood %d X_Position %d Y_Position %d\n", &a, &b, h, 20, &d, &e, &f, &g) == 7) {
  newSoldier = new TSoldier(a, b, h, d, e, f, g);
  MainLogic::soldiers.push_back(newSoldier);
}

fscanf_s(fp, "CommandsInfo\n");

for (int i = 0; i < PLAYER_NUM; i++) {
  if (fscanf_s(fp, "Player%dCommands\n", &playerID) != 1)
    break;
  while (true) {
    if (fscanf_s(fp, "Command Move SoldierID %d Direction %s Distance %d\n", &a, h, 20, &b) == 3) {
      try {
        MainLogic::soldiers[a]->soldierMove.move = true;
        MainLogic::soldiers[a]->soldierMove.moveDirection = moveDirStr2Enum(std::string(h));
        MainLogic::soldiers[a]->soldierMove.moveDistance = b;
      }
      catch (const std::exception&) {
        //Nothing
      }
      continue;
    } else if (fscanf_s(fp, "Command Attack SoldierID %d VicType %s VictimID %d\n", &a, h, 20, &b) == 3) {
      try {
        if (std::string(h) == "Soldier")
          MainLogic::soldiers[a]->victim = MainLogic::soldiers[b];
        else if (std::string(h) == "Tower")
          MainLogic::soldiers[a]->victim = MainLogic::towers[b];
      }
      catch (const std::exception&) {
        //Nothing
      }
      continue;
    } else if (fscanf_s(fp, "Command Upgrade TowerID %d\n", &a) == 1) {
      try {
        MainLogic::towers[a]->upgrade = true;
      }
      catch (const std::exception&) {

      }

      continue;
    } else if (fscanf_s(fp, "Command Produce TowerID %d Type %s\n", &a, h, 20) == 2) {
      try {
        MainLogic::towers[a]->produceSoldier.is_produce = true;
        MainLogic::towers[a]->produceSoldier.soldierType = SoldierTypeStr2Enum(std::string(h));
      }
      catch (const std::exception&) {

      }
    } else {
      break;
    }
  }
}

  }

void MainLogic::UIUpdate(Scene* playscene) 
{
  PlayScene* current_scene = dynamic_cast<PlayScene*>(playscene);
  //TODO add no this scene waring
  if (current_scene == NULL) {
    return;
  }
 // current_scene->RefreshMap();
}


	void MainLogic::clearData()
	{
		loadFileName = "";
		
		
					
	}


	void MainLogic::WriteLog(std::string  message)
	{
		logFile << message << std::endl;
	}

  void MainLogic::StartScene2PlayScene() 
  {
    // change scence
    auto scene = PlayScene::createScene();
    Director::getInstance()->replaceScene(scene);
  }
}