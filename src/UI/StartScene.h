/*
	When the application is launched, this scene is launched.
	
*/
#pragma once

#include "cocos2d.h"
#include "../Data/DataDef.h"

USING_NS_CC;
namespace UI
{
	class StartScene : public cocos2d::Scene
	{
	public:
		
		static cocos2d::Scene* createScene();
		
		virtual bool init();

		void loadFileClickedCallback(cocos2d::Ref* pSender);

		CREATE_FUNC(StartScene);

 
	};

}