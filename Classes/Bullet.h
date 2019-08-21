#pragma once
#include "cocos2d.h"
#include "Config.h"
#include "PhysicsShapeCache.h"

USING_NS_CC;

class Bullet :public Node
{
public:
	virtual bool init();
	CREATE_FUNC(Bullet);

	void CreateBullet(Vec2 positionMain, Size visibleS);
	void CreateBullet(Vec2 positionMain, Size visibleS, int i);
private:
	Sprite* m_spBullet;
};

