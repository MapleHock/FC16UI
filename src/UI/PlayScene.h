#pragma once
#include "../Data/DataDef.h"
#include "ui/CocosGUI.h"

namespace UI
{
	class PlayScene : public cocos2d::Scene
	{
	public:
		static cocos2d::Scene* createScene();

		virtual bool init();

		CREATE_FUNC(PlayScene);

    void RefreshMap(float dt);

    void StartClickedCallback();


    cocos2d::TMXTiledMap* map_widget;

    cocos2d::ui::Button* start_btn;

    cocos2d::Scheduler* refresh_control;
   
  private:
    //TODO find a way to delete those state bool
    bool is_pause;
	};

}