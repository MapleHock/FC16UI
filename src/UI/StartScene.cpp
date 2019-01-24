#include "StartScene.h"
#include "SimpleAudioEngine.h"
#include "../Logic/MainLogic.h"
#include "PlayScene.h"
USING_NS_CC;

namespace UI
{
	
	Scene* StartScene::createScene()
	{
		//auto layer = 
		return StartScene::create();
	}

	bool StartScene::init()
	{
		if (!Scene::init())
		{
			return false;
		}

		auto visibleSize = Director::getInstance()->getVisibleSize();
		Vec2 origin = Director::getInstance()->getVisibleOrigin();

		auto startSprite = Sprite::create("start.jpg");
		startSprite->setScale(0.5);
		startSprite->setPosition(500, 300);
		this->addChild(startSprite, 0);

		//auto closeItem = MenuItemImage::create("closeNormal.png", "closeSelected.png",CC_CALLBACK_1(....))
		auto loadFileItem = MenuItemImage::create("loadFileNormal.jpg", "loadFileSelected.jpg", CC_CALLBACK_1(StartScene::loadFileClickedCallback, this));
		if (loadFileItem == nullptr ||
			loadFileItem->getContentSize().width <= 0 ||
			loadFileItem->getContentSize().height <= 0)
		{
			MainLogic::WriteLog("'CloseNormal.png' and 'CloseSelected.png'");
		}
		else
		{
			float x = origin.x + visibleSize.width - loadFileItem->getContentSize().width / 2;
			float y = origin.y + loadFileItem->getContentSize().height / 2;
			loadFileItem->setPosition(Vec2(x, y));
		}

		auto menu = Menu::create(loadFileItem, nullptr);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);

		return true;
	}

	void StartScene::loadFileClickedCallback(cocos2d::Ref* pSender)
	{
		MainLogic::LoadData();
    MainLogic::StartScene2PlayScene();
	}
}