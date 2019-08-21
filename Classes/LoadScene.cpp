#include "LoadScene.h"
#include "SimpleAudioEngine.h"
#include "MainMenuScene.h"
#include "Config.h"
#include "PhysicsShapeCache.h"

USING_NS_CC;

Scene* LoadScene::createScene()
{
	auto scene = Scene::create();
	auto layer = LoadScene::create();
	layer->init();
	scene->addChild(layer);
	return scene;
}

bool LoadScene::init()
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

void LoadScene::update(float dt)
{
	switch (this->m_loadingStep)
	{
	case 0:
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(Path_PLIST_Preload, Path_PNG_Preload);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(Path_PLIST_PreImage0, Path_PNG_PreImage0);
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile(Path_PLIST_PreImage1, Path_PNG_PreImage1);
		break;
	case 1:
		PhysicsShapeCache::getInstance()->addShapesWithFile("PhysicBodies.plist");
	default:
		Director::getInstance()->replaceScene(MainMenuScene::createScene());
		break;
	}
	this->m_loadingStep++;
}