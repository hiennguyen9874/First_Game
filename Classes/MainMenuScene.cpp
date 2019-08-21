#include "MainMenuScene.h"
#include "LoadGameScene.h"
#include "GameScene.h"
#include "Config.h"
#include "LoadScene.h"
#include "Function.h"
#include<algorithm> 

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
	// 'scene' is an autorelease object
	auto scene = Scene::create();

	// 'layer' is an autorelease object
	auto layer = MainMenuScene::create();

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto backgroundSprite = Sprite::create(Background_Main_Menu);

	backgroundSprite->setPosition(PositionError(0.5, 0.5));
	backgroundSprite->setScale(ScaleError(backgroundSprite));
	this->addChild(backgroundSprite, zBACKGROUND);

	// Logo head shoulders
	auto newspriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName("1.f0023.png");
	auto mySprite = Sprite::createWithSpriteFrame(newspriteFrame);
	
	mySprite->setAnchorPoint(Vec2(0, 1));
	mySprite->setPosition(PositionError(0, 1.f));
	mySprite->setScale(0.25 * ScaleError(mySprite));
	
	
	this->addChild(mySprite, zBACKGROUND);

	// Logo NOU
	auto nouSpriteFram = SpriteFrameCache::getInstance()->getSpriteFrameByName("1.f0024.png");
	auto nouSprite = Sprite::createWithSpriteFrame(nouSpriteFram);
	nouSprite->setAnchorPoint(Vec2(1, 1));
	nouSprite->setPosition(PositionError(1.f, 1.f));
	nouSprite->setScale(0.125 * ScaleError(nouSprite));
	this->addChild(nouSprite, zBACKGROUND);

	// 4 Con gàu
	auto maSprite1 = Sprite::createWithSpriteFrameName("2.f0003.png");
	maSprite1->setPosition(PositionError(1.f / 2, 3.f / 4));
	maSprite1->setScale(0.3 * ScaleError(maSprite1));
	this->addChild(maSprite1, zBACKGROUND);

	auto maSprite2 = Sprite::createWithSpriteFrameName("2.f0004.png");
	maSprite2->setPosition(PositionError(64.f / 87.f, 50.f / 65.f));
	maSprite2->setScale(0.2 * ScaleError(maSprite2));
	this->addChild(maSprite2, zBACKGROUND);

	auto maSprite3 = Sprite::createWithSpriteFrameName("2.f0005.png");
	maSprite3->setPosition(PositionError(78.f / 87, 30.f / 65));
	maSprite3->setScale(0.125 * ScaleError(maSprite3));
	this->addChild(maSprite3, zBACKGROUND);

	auto maSprite4 = Sprite::createWithSpriteFrameName("2.f0006.png");
	maSprite4->setPosition(PositionError(48.f / 87, 12.f / 65));
	maSprite4->setScale(0.125 * ScaleError(maSprite4));
	this->addChild(maSprite4, zBACKGROUND);

	// Logo gameloft
	auto logo = Sprite::createWithSpriteFrameName("2.f0011.png");
	logo->setAnchorPoint(Vec2(0, 0));
	logo->setPosition(PositionError(0, 0));
	logo->setScale(0.05 * ScaleError(logo));
	this->addChild(logo, zBACKGROUND);

	//auto position = Vec2(visibleSize.width * 63 / 87, visibleSize.height * (65 - 43) / 65);
	auto position = PositionError(63.f / 87, 22.f / 65);

	auto btnPlay = MenuItemImage::create();
	btnPlay->setNormalSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("2.f0008.png"));
	btnPlay->setCallback(CC_CALLBACK_1(MainMenuScene::GoToGameScene, this));

	auto menu = Menu::create(btnPlay, nullptr);
	menu->setPosition(position);

	auto sequence = Sequence::create(ScaleTo::create(0.3f, 0.15 * ScaleError(btnPlay->getContentSize())),
		EaseElasticOut::create(ScaleTo::create(0.3f, 0.2 * ScaleError(btnPlay->getContentSize()))),
		DelayTime::create(1.5f), nullptr);

	btnPlay->runAction(RepeatForever::create(sequence));

	this->addChild(menu, zUI);

	return true;
}

void MainMenuScene::GoToLoadGameScene(cocos2d::Ref* sender)
{
	auto scene = LoadGameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.1, scene));
}

void MainMenuScene::GoToGameScene(cocos2d::Ref* sender)
{
	auto scene = GameScene::createScene();
	Director::getInstance()->replaceScene(TransitionFade::create(0.1, scene));
}