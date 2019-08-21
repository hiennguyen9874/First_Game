#include "LoadGameScene.h"
#include "SimpleAudioEngine.h"
#include "GameScene.h"

USING_NS_CC;

Scene* LoadGameScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadGameScene::create();
	layer->init();
	scene->addChild(layer);
	return scene;
}

bool LoadGameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->m_loadingStep = 0;
	this->scheduleUpdate();

	return true;
}

void LoadGameScene::update(float dt)
{
	switch (this->m_loadingStep)
	{
	default:
		Director::getInstance()->replaceScene(GameScene::createScene());
		break;
	}
	this->m_loadingStep++;
}