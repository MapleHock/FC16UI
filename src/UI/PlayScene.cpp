#include "PlayScene.h"
#include "../Logic/MainLogic.h"
#include "ui/CocosGUI.h"
//TODO delete iostream
#include <iostream>
USING_NS_CC;


cocos2d::Scene* UI::PlayScene::createScene()
{
	return PlayScene::create();
}

bool UI::PlayScene::init()
{
  if (!Scene::init()) 
  {
    return false;
  }

  auto visibleSize = Director::getInstance()->getVisibleSize();
  Vec2 origin = Director::getInstance()->getVisibleOrigin();

  map_widget = TMXTiledMap::create("test.tmx");
  map_widget->setPosition(0, 0);
  map_widget->setScale(0.3f);
  this->addChild(map_widget, 0);
  
  start_btn = ui::Button::create("s_test.png");
  start_btn->setTitleText("start");
  start_btn->setPosition(Vec2(980, 600));
  start_btn->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
    switch (type) {
    case cocos2d::ui::Widget::TouchEventType::ENDED:
      this->StartClickedCallback();
      break;
    default:
      break;
    }
  });

  this->addChild(start_btn, 1);

  this->schedule(schedule_selector(UI::PlayScene::RefreshMap), 1.0f);
  //CCScheduler::sharedScheduler()->pauseTarget(this);
  return true;
}

void UI::PlayScene::RefreshMap(float dt) 
{
  map_widget->getLayer("background")->removeTileAt(Vec2(1, 1));
  if (start_btn->getTitleText() == "refresh") {
    start_btn->setTitleText("again refresh");
  } else {
    start_btn->setTitleText("refresh");
  }
}

void UI::PlayScene::StartClickedCallback() 
{
  //MainLogic::GameStart();
  start_btn->setTitleText("stop");
}




