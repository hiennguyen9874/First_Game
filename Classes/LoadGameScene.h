#pragma once
#include "cocos2d.h"

class LoadGameScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(LoadGameScene);

	void update(float dt);
private:
	int m_loadingStep;
};