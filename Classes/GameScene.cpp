#include "GameScene.h"
#include "Function.h"
#include "GameOverScene.h"
#include "MainMenuScene.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
	auto scene = Scene::createWithPhysics();
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	Vect gravity(0.0f, 10.0f);
	scene->getPhysicsWorld()->setGravity(gravity);

	// 'layer' is an autorelease object
	auto layer = GameScene::create();
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	this->m_iScore = 0;
	this->m_iEnemyKilled = 0;

	this->m_bPauseGame = false;
	this->m_bRemovedAll = false;
	this->m_bResetPowerBar = false;

	this->m_fTimeGame = StartTime;
	this->m_ftimeResetEnemy = ResetEnemyTime;
	this->m_fTimeResetPowerBar = 1.0f / Full_Enemy_Killed;
	this->m_fTimeReplayScene = TimeReplayScene;

	this->m_sVisibleSize = visibleSize;
	this->m_typeBullet = tBullet_One;
	this->m_fTimeBulletPlayer = DeltaTimeBulletPlayer;
	this->m_fTimeBulletEnemy = DeltaTimeBulletEnemy;

	this->CreateBackground();
	this->CreatePlane();
	this->InitBulletPlayer();
	this->InitEnemy();
	this->InitItem();
	this->InitMegaBomb();
	this->InitPowerBar();
	this->InitBulletEnemy();

	this->m_spriteEnd = Sprite::createWithSpriteFrameName("1.f0133.png");
	this->m_spriteEnd->setPosition(visibleSize / 2);
	this->m_spriteEnd->setVisible(false);
	this->addChild(this->m_spriteEnd, zGAME_BOARD + 2);

	this->m_LabelScoreEnd = Label::createWithTTF(std::to_string((int)this->m_iScore), Path_Font_Label_Timer, Size_Label_Timer);
	this->m_LabelScoreEnd->setPosition(visibleSize / 2);
	this->m_LabelScoreEnd->setVisible(false);
	this->addChild(m_LabelScoreEnd, zGAME_BOARD + 3);

	//add contact event listener
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::OnContactBegin, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	this->scheduleUpdate();
	return true;
}

void GameScene::update(float dt)
{
	if (this->m_fTimeGame <= 0)
	{
		// Het thoi gian
		//Director::getInstance()->pause();
		Director::getInstance()->getRunningScene()->getPhysicsWorld()->setSpeed(0);
		this->m_spriteEnd->setVisible(true);
		this->m_LabelScoreEnd->setVisible(true);
		this->m_LabelScoreEnd->setString(std::to_string((int)this->m_iScore));
		if (this->m_fTimeReplayScene <= 0)
		{
			Director::getInstance()->replaceScene(MainMenuScene::createScene());
			for (int i = 0; i < this->m_vectorItem.size(); i++)
				this->m_vectorItem.at(i)->stopAllActions();
		}
		else
			this->m_fTimeReplayScene -= dt;
	}
	else
	{
		if (this->m_bRemovedAll)
		{
			// Kiem tra xem co dang thuc hien megabomb khong
			if (this->IsResumeGame())
			{
				// Neu megabomb da thuc hien xong
				this->m_bPauseGame = false;
				this->m_bResetPowerBar = false;
				this->m_iEnemyKilled = 0;
			}
			else
				this->m_bPauseGame = true;
		}

		// Bullet_5 neu score > 30,000
		if (this->m_iScore >= 30000)
			this->m_typeBullet = tBullet_Five;

		// Kiem tra xem co can Reset Power bar khong
		if (this->m_bResetPowerBar)
		{
			if (this->m_fTimeResetPowerBar <= 0)
			{
				this->m_iEnemyKilled -= 1;
				if (this->m_iEnemyKilled <= 0)
				{
					this->m_iEnemyKilled = 0;
				}
				this->m_fTimeResetPowerBar = 1.0f / Full_Enemy_Killed;
			}
			else
				this->m_fTimeResetPowerBar -= dt;
		}

		this->CheckBulletPlayerOut();	// Kiem tra cac vien dan bay ra ngoai thi an di
		this->UpdatePosition(dt);		// Update vi tri cua player

		// Shot Bullet for Enemy
		if (this->m_fTimeBulletEnemy <= 0)
		{
			this->RandomBulletEnemy();
			this->m_fTimeBulletEnemy = DeltaTimeBulletEnemy;
		}
		else
			this->m_fTimeBulletEnemy -= dt;

		// Shot Bullet for Player
		if (this->m_fTimeBulletPlayer <= 0)
		{
			if (!this->m_bPauseGame)
				this->ShotBulletPlayer();
			this->m_fTimeBulletPlayer = DeltaTimeBulletPlayer;
		}
		else
			this->m_fTimeBulletPlayer -= dt;

		// Update score
		if (this->m_iScore <= 0)
			this->m_iScore = 0;
		this->m_LabelTime->setString(std::to_string((int)this->m_fTimeGame) + "\"");
		this->m_LabelScore->setString(std::to_string((int)this->m_iScore));

		this->CheckItemOutScreen();			// An item di ra ngoai man hinh

		if (this->m_ftimeResetEnemy <= 0.0)
		{
			if (!this->m_bPauseGame)
			{
				if (!this->m_bRemovedAll)
					this->ResetEnemies();
				this->m_ftimeResetEnemy = ResetEnemyTime;
			}
			this->m_bPauseGame = false;
			this->m_bRemovedAll = false;
		}
		else
			this->m_ftimeResetEnemy -= dt;

		this->SetPowerBar();

		// Update time game
		this->m_fTimeGame -= dt;
	}
}

#pragma region Init
void GameScene::CreateBackground()
{
	auto backgroundSprite = Sprite::create(Background_Game_Scene);
	backgroundSprite->setPosition(PositionError(0.5, 0.5));
	backgroundSprite->setScale(ScaleError(backgroundSprite));
	this->addChild(backgroundSprite, zBACKGROUND);

	// Logo Heads shoulders
	auto mySprite = Sprite::createWithSpriteFrameName(Path_Sprite_Logo_Heads_Shoulders);
	mySprite->setAnchorPoint(Vec2(0, 1));
	mySprite->setPosition(PositionError(0, 1.f));
	mySprite->setScale(Scale_Logo_Heads_Shoulders * ScaleError(mySprite));
	this->addChild(mySprite, zBACKGROUND);

	// Thoi gian con lai
	auto timeSprite = Sprite::createWithSpriteFrameName(Path_Sprite_Timer);
	timeSprite->setScale(Scale_Icon_Hub * ScaleError(timeSprite));
	timeSprite->setPosition(PositionError(Position_Sprite_Timer_W, Position_Sprite_Timer_H));
	this->addChild(timeSprite, zBACKGROUND);

	this->m_LabelTime = Label::createWithTTF(std::to_string(this->m_fTimeGame) + "\"", Path_Font_Label_Timer, Size_Label_Timer);
	this->m_LabelTime->setPosition(this->m_sVisibleSize.width * Position_Label_Timer_W, this->m_sVisibleSize.height * Position_Label_Timer_H);
	this->addChild(m_LabelTime, zBACKGROUND);

	// Score
	auto starSprite = Sprite::createWithSpriteFrameName(Path_Score_Sprite);
	starSprite->setPosition(PositionError(Position_Sprite_Score_W, Position_Sprite_Score_H));
	starSprite->setScale(Scale_Icon_Hub * ScaleError(starSprite));
	this->addChild(starSprite, zBACKGROUND);

	this->m_LabelScore = Label::createWithTTF(std::to_string(this->m_iScore), Path_Font_Label_Score, Size_Label_Score);
	this->m_LabelScore->setPosition(PositionError(Position_Label_Score_W, Position_Label_Score_H));
	this->addChild(m_LabelScore, zBACKGROUND);

	// Thanh nang luong
	auto loadSprite = Sprite::createWithSpriteFrameName(Path_Sprite_Power);
	loadSprite->setPosition(PositionError(Position_Sprite_Power_W, Position_Sprite_Power_H));
	loadSprite->setScale(Scale_Icon_Hub * ScaleError(loadSprite));
	this->addChild(loadSprite, zGAME_BOARD);

	auto loadSprite1 = Sprite::createWithSpriteFrameName(Path_Sprite_PowerBar);
	loadSprite1->setPosition(PositionError(Position_Label_PowerBar_W, Position_Label_PowerBar_H));
	loadSprite1->setScale(Scale_PowerBar * ScaleError(loadSprite1));
	this->addChild(loadSprite1, zBACKGROUND);
}

void GameScene::CreatePlane()
{
	this->m_spritePlayer = Sprite::createWithSpriteFrameName((string)Path_Sprite_Player + ".png");
	this->m_spritePlayer->setPosition(PositionError(Position_Sprite_Player_W, Position_Sprite_Player_H));
	this->m_spritePlayer->setTag(PLAYER);
	this->m_spritePlayer->setScale(Scale_Enemy * ScaleError(this->m_spritePlayer));

	PhysicsShapeCache::getInstance()->setBodyOnSprite(Path_Sprite_Player, this->m_spritePlayer);
	this->m_spritePlayer->getPhysicsBody()->setDynamic(false);
	this->m_spritePlayer->getPhysicsBody()->setGravityEnable(false);
	this->m_spritePlayer->getPhysicsBody()->setCategoryBitmask(CategoryPlayer);
	this->m_spritePlayer->getPhysicsBody()->setCollisionBitmask(CollisionPlayer);
	this->m_spritePlayer->getPhysicsBody()->setContactTestBitmask(CollisionPlayer);

	this->addChild(this->m_spritePlayer, zGAME_BOARD);
	this->m_vec2DesPosition = this->m_spritePlayer->getPosition();

	// Thiet lap animation cho may bay
	const int numberSprite = Num_Frame_Player_Animation;
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(numberSprite);
	for (int i = 0; i < numberSprite; i++)
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(Path_Sprite_Player, i) + ".png"));

	Animation* animation = Animation::createWithSpriteFrames(animFrames, Delay_Time_Animation_Player);
	Animate* animate = Animate::create(animation);
	this->m_spritePlayer->runAction(RepeatForever::create(animate));

	// Su kien di chuyen cho may bay
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = [&](Touch* touch, Event* event) {
		this->m_vec2DesPosition = touch->getLocation();
		return true;
	};
	listener->onTouchMoved = [&](Touch* touch, Event* event) {
		this->m_vec2DesPosition = touch->getLocation();
		return true;
	};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GameScene::InitEnemy()
{
	int bb = 2;
	int a = rand_int(0, MaxEnemyX * MaxEnemyY - 1), b = rand_int(0, MaxEnemyX * MaxEnemyY - 1);
	for (int i = 0; i < MaxEnemyX; i++)
	{
		for (int j = 0; j < MaxEnemyY; j++)
		{
			Enemy* enemy = Enemy::create();
			enemy->SetVisible(this->m_sVisibleSize);

			int randd = rand_int(Enemy_O, Enemy_O2);
			enemy->SetType((TYPE_ENEMY)randd);

			if (i * MaxEnemyY + j == a || i * MaxEnemyY + j == b)
			{
				enemy->SetEnableBalloon(true);
			}
			enemy->setScale(Scale_Enemy_ALl * ScaleError(enemy->GetContentSize()));
			enemy->setPosition(PositionError(Position_Player_X(i), Position_Player_Y(j)));
			this->addChild(enemy, zGAME_BOARD);
			this->m_VectorEnemy.pushBack(enemy);
		}
	}
}

void GameScene::InitItem()
{
	for (int i = 0; i < MaxDropItem; i++)
	{
		Sprite* spriteDrop = Sprite::createWithSpriteFrameName((string)Path_Sprite_Item_Drop + ".png");

		PhysicsShapeCache::getInstance()->setBodyOnSprite(Path_Sprite_Item_Drop, spriteDrop);
		spriteDrop->getPhysicsBody()->setContactTestBitmask(CollisionItem);
		spriteDrop->getPhysicsBody()->setCategoryBitmask(CategoryItem);
		spriteDrop->getPhysicsBody()->setCollisionBitmask(CollisionItem);
		spriteDrop->getPhysicsBody()->setEnabled(false);

		spriteDrop->setScale(Scale_Drop_Item *ScaleError(spriteDrop));
		spriteDrop->setTag(ITEM_DROP);
		spriteDrop->setVisible(false);

		spriteDrop->getPhysicsBody()->setDynamic(false);

		this->addChild(spriteDrop, zBACKGROUND);
		this->m_vectorItem.addObject(spriteDrop);
	}
}

void GameScene::InitMegaBomb()
{
	this->m_spriteMegaBomb = Sprite::createWithSpriteFrameName(Path_Animation_Megabomb);
	this->m_spriteMegaBomb->setVisible(false);
	this->addChild(this->m_spriteMegaBomb, zGAME_BOARD);

	this->m_spriteShieldPlayer = Sprite::createWithSpriteFrameName(Path_Sprite_ShieldPlayer);
	this->m_spriteShieldPlayer->setPosition(this->m_spritePlayer->getPosition().x, this->m_spritePlayer->getPosition().y + 0.05 * this->m_sVisibleSize.height);
	this->m_spriteShieldPlayer->setVisible(false);
	this->m_spriteShieldPlayer->setScale(Scale_Enemy * ScaleError(this->m_spriteMegaBomb));
	this->addChild(this->m_spriteShieldPlayer, zGAME_BOARD);
}

void GameScene::InitPowerBar()
{
	this->m_spritePowerBar = Sprite::createWithSpriteFrameName(Path_Sprite_PowerBarFull);
	this->m_spritePowerBar->setPosition(PositionError(Position_Label_Run_PowerBar_W, Position_Label_Run_PowerBar_H));
	this->m_spritePowerBar->setScale(Scale_Icon_Hub * ScaleError(this->m_spritePowerBar));
	this->m_spritePowerBar->setAnchorPoint(Vec2(0, 0.5));
	this->addChild(this->m_spritePowerBar, zBACKGROUND);
}

void GameScene::InitBulletPlayer()
{
	for (int i = 0; i < MaxBullet; i++)
	{
		Sprite* bullet = Sprite::createWithSpriteFrameName((string)Path_Sprite_BulletPlayer + ".png");
		bullet->setAnchorPoint(Vec2(0.5, 0));
		bullet->setScale(Scale_Bullet_Player * ScaleError(bullet));
		bullet->setVisible(false);
		bullet->setTag(BULLET_PLAYER);

		PhysicsShapeCache::getInstance()->setBodyOnSprite(Path_Sprite_BulletPlayer, bullet);
		bullet->getPhysicsBody()->setGravityEnable(false);
		bullet->getPhysicsBody()->setCategoryBitmask(CategoryBulletPlayer);
		bullet->getPhysicsBody()->setCollisionBitmask(CollisionBulletPlayer);
		bullet->getPhysicsBody()->setContactTestBitmask(CollisionBulletPlayer);
		bullet->getPhysicsBody()->setEnabled(false);

		this->addChild(bullet, zGAME_BOARD);
		this->m_vectorBulletPlayer.addObject(bullet);
	}
}

void GameScene::InitBulletEnemy()
{
	for (int i = 0; i < Num_Bullet_Enemy; i++)
	{
		Sprite* bulletEnemy = Sprite::createWithSpriteFrameName((string)Path_Sprite_Bullet_Enemy + ".png");
		bulletEnemy->setVisible(false);
		bulletEnemy->setTag(BULLET_ENEMY);
		bulletEnemy->setScale(Scale_Bullet_Enemy * ScaleError(bulletEnemy));

		PhysicsShapeCache::getInstance()->setBodyOnSprite(Path_Sprite_Bullet_Enemy, bulletEnemy);
		bulletEnemy->getPhysicsBody()->setGravityEnable(false);
		bulletEnemy->getPhysicsBody()->setCategoryBitmask(CategoryBulletEnemy);
		bulletEnemy->getPhysicsBody()->setCollisionBitmask(CollisionBulletEnemy);
		bulletEnemy->getPhysicsBody()->setContactTestBitmask(CollisionBulletEnemy);
		bulletEnemy->getPhysicsBody()->setEnabled(false);

		this->addChild(bulletEnemy, zGAME_BOARD);
		this->m_vectorBulletEnemy.addObject(bulletEnemy);
	}
}

#pragma endregion

#pragma region Collision

bool GameScene::OnContactBegin(cocos2d::PhysicsContact& contact)
{
	Sprite* spriteA = (Sprite*)contact.getShapeA()->getBody()->getNode();
	Sprite* spriteB = (Sprite*)contact.getShapeB()->getBody()->getNode();

	if (spriteA != nullptr && spriteB != nullptr)
	{
		int tagA = spriteA->getTag();
		int tagB = spriteB->getTag();

		if ((tagA == BULLET_PLAYER && tagB == ENEMY))
			BulletCollisionsWithEnemy(spriteA, spriteB);
		else if (tagA == ENEMY && tagB == BULLET_PLAYER)
			BulletCollisionsWithEnemy(spriteB, spriteA);
		else if (tagA == BULLET_PLAYER && tagB == BALLOON)
			BulletCollisionsWithBallon(spriteA, spriteB);
		else if (tagA == BALLOON && tagB == BULLET_PLAYER)
			BulletCollisionsWithBallon(spriteB, spriteA);
		else if (tagA == PLAYER && tagB == ITEM_DROP)
		{
			// CCLOG("Player collision with item drop");
			PlayerCollisionsWithItem(spriteA, spriteB);
		}
		else if (tagA == ITEM_DROP && tagB == PLAYER)
		{
			// CCLOG("Item drop collision with item player");
			PlayerCollisionsWithItem(spriteB, spriteA);
		}
		else if (tagA == BULLET_PLAYER && tagB == BULLET_ENEMY || tagA == BULLET_ENEMY && tagB == BULLET_PLAYER)
		{
			spriteA->setVisible(false);
			spriteA->getPhysicsBody()->setEnabled(false);

			spriteB->setVisible(false);
			spriteB->getPhysicsBody()->setEnabled(false);
		}
		else if (tagA == PLAYER && tagB == BULLET_ENEMY)
		{
			// CCLOG("Player collision with BULLET_ENEMY");
			PlayerCollisionsWithBulletEnemy(spriteA, spriteB);
		}
		else if (tagA == BULLET_ENEMY && tagB == PLAYER)
		{
			// CCLOG("BULLET_ENEMY collision with PLAYER");
			PlayerCollisionsWithBulletEnemy(spriteB, spriteA);
		}
	}
	return true;
}

void GameScene::BulletCollisionsWithEnemy(Sprite* bullet, Sprite* spEnemy)
{
	if (bullet->isVisible())
	{
		// int iter = this->FindContact(bullet, spEnemy);

		Enemy* enemy = this->FindSpriteMain(spEnemy);
		enemy->RunExplosionAction();
		if (enemy->GetEnableHasDrop())
		{
			this->DropItem(enemy->getPosition()); // Drop Item
		}

		this->Bonus(enemy->getPosition());

		bullet->setVisible(false);
		bullet->getPhysicsBody()->setEnabled(false);

		enemy->HideEnemy();
	}
}

void GameScene::BulletCollisionsWithBallon(Sprite* bullet, Sprite* ballon)
{
	if (bullet->isVisible() && ballon->isVisible())
	{
		Enemy* enemyBB = this->FindSpriteBB(ballon);

		if (enemyBB->GetAlive())
			enemyBB->RunHasDropAction();

		ballon->setVisible(false);
		ballon->getPhysicsBody()->setEnabled(false);

		bullet->setVisible(false);
		bullet->getPhysicsBody()->setEnabled(false);
	}
}

void GameScene::PlayerCollisionsWithItem(Sprite* player, Sprite* Item)
{
	if (Item->isVisible())
	{
		if (this->m_typeBullet == tBullet_One)
		{
			this->m_typeBullet = tBullet_Four;
			// CCLOG("Set m_typeBullet to Four");
		}
		Item->setVisible(false);
		Item->getPhysicsBody()->setEnabled(false);
		// CCLOG("%d", this->m_typeBullet);
	}
}

void GameScene::PlayerCollisionsWithBulletEnemy(Sprite* player, Sprite* bulletEnemy)
{
	if (bulletEnemy->isVisible())
	{
		// CCLOG("Set m_typeBullet to One");
		bulletEnemy->setVisible(false);
		bulletEnemy->getPhysicsBody()->setEnabled(false);
		// CCLOG("%d", this->m_typeBullet);

		if (!this->m_bPauseGame)
		{
			this->m_typeBullet = (this->m_typeBullet == tBullet_Five) ? tBullet_Four : tBullet_One;

			// Tru 300 diem
			Sprite* sprite = Sprite::createWithSpriteFrameName(Path_Sprite_Bonus_Three);

			sprite->setPosition(this->m_spritePlayer->getPosition().x, this->m_spritePlayer->getPosition().y + 0.1 * this->m_sVisibleSize.height);
			sprite->setScale(Scale_Enemy * ScaleError(sprite));
			this->addChild(sprite, zUI);

			auto moveBy = MoveBy::create(1, Vec2(0, 10));
			auto fadeo = FadeOut::create(1);
			auto spa = Spawn::createWithTwoActions(moveBy, fadeo);

			auto seq1 = Sequence::create(spa, RemoveSelf::create(), nullptr);
			sprite->runAction(seq1);
			this->m_iScore -= Num_Bonus_Three;

			//  Animation when bullet enemy collision with player
			const int numberSprite = Num_Frame_Animation_BulletEnemy_Collision_With_Player;
			Vector<SpriteFrame*> animFrames;
			animFrames.reserve(numberSprite);
			for (int i = 0; i < numberSprite; i++)
				animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(Path_Animation_BulletEnemy_Collision_With_Player, i) + ".png"));

			Animation* animation = Animation::createWithSpriteFrames(animFrames, Delay_Time_Animation_BulletEnemy_Collision_With_Player);
			Animate* animate = Animate::create(animation);
			this->m_spritePlayer->runAction(animate);
		}
	}
}

#pragma endregion

#pragma region Check

Enemy* GameScene::FindSpriteBB(Sprite* sprite)
{
	for (int i = 0; i < this->m_VectorEnemy.size(); i++)
	{
		Enemy* enemy = this->m_VectorEnemy.at(i);
		if (enemy->CheckSpriteBallon(sprite) == true)
		{
			return enemy;
		}
	}
	return nullptr;
}

Enemy* GameScene::FindSpriteMain(Sprite* sprite)
{
	for (int i = 0; i < this->m_VectorEnemy.size(); i++)
	{
		Enemy* enemy = this->m_VectorEnemy.at(i);
		if (enemy->CheckSpriteEnemy(sprite) == true)
		{
			return enemy;
		}
	}
	return nullptr;
}

bool checkOut(Sprite* sprite, Size visibleS)
{
	auto vt = sprite->getPosition();
	if (vt.x <= 0 || vt.x >= visibleS.width)
		return true;
	if (vt.y <= 0 || vt.y >= visibleS.height)
		return true;
	return false;
}

void GameScene::CheckBulletPlayerOut()
{
	for (int i = this->m_vectorBulletPlayer.size() - 1; i >= 0; i--)
	{
		Sprite* sprite = this->m_vectorBulletPlayer.at(i);
		if (checkOut(sprite, this->m_sVisibleSize))
		{
			sprite->setVisible(false);
			if (sprite->getPhysicsBody() != nullptr)
				sprite->getPhysicsBody()->setEnabled(false);
		}
	}
}

void GameScene::CheckItemOutScreen()
{
	for (int i = 0; i < this->m_vectorItem.size(); i++)
	{
		auto sprite = this->m_vectorItem.at(i);
		if (checkOut(sprite, this->m_sVisibleSize))
		{
			sprite->setPosition(-1, -1);
			sprite->setVisible(false);
			sprite->getPhysicsBody()->setEnabled(false);
			sprite->getPhysicsBody()->setDynamic(false);
		}
	}
}

bool GameScene::IsResumeGame()
{
	for (int i = 0; i < this->m_VectorEnemy.size(); i++)
	{
		if (!this->m_VectorEnemy.at(i)->GetEnableRemovedMegaBomb())
			return false;
	}
	return true;
}

#pragma endregion

#pragma region RunAction

void GameScene::DropItem(Vec2 position)
{
	Sprite* spriteDrop = this->m_vectorItem.getObject();
	spriteDrop->setPosition(position);
	spriteDrop->setVisible(true);
	spriteDrop->getPhysicsBody()->setEnabled(true);
	spriteDrop->getPhysicsBody()->setDynamic(false);

	auto moveBy = MoveBy::create(5, Vec2(0, -this->m_sVisibleSize.height));
	moveBy->setTag(Tag_MoveTo_DropItem_Action);
	spriteDrop->runAction(moveBy);
}

void GameScene::Bonus(Vec2 position)
{
	if (this->m_iEnemyKilled < Full_Enemy_Killed && !this->m_bPauseGame)
		this->m_iEnemyKilled += 1;
	else if (this->m_iEnemyKilled >= Full_Enemy_Killed)
		this->RunActionMegaBomb();
	//CCLOG("%d", (int)this->m_iEnemyKilled);

	Sprite* sprite1;
	if (this->m_typeBullet == tBullet_One)
	{
		this->m_iScore += Num_Bonus_One;
		sprite1 = Sprite::createWithSpriteFrameName(Path_Sprite_Bonus_One);
	}
	else
	{
		this->m_iScore += Num_Bonus_Two;
		sprite1 = Sprite::createWithSpriteFrameName(Path_Sprite_Bonus_Two);
	}
	sprite1->setPosition(position);
	sprite1->setScale(Scale_Enemy * ScaleError(sprite1));
	this->addChild(sprite1, zUI);

	auto moveBy = MoveBy::create(1, Vec2(0, 5));
	auto fadeo = FadeOut::create(1);
	auto spa = Spawn::createWithTwoActions(moveBy, fadeo);

	auto seq1 = Sequence::create(spa, RemoveSelf::create(), nullptr);
	sprite1->runAction(seq1);
}

#pragma endregion

#pragma region Update

void GameScene::UpdatePosition(float dt)
{
	auto vitri = this->m_spritePlayer->getPosition();

	vitri.x += (this->m_vec2DesPosition.x - vitri.x) * 0.25;
	vitri.y += (this->m_vec2DesPosition.y - vitri.y) * 0.25;

	if (vitri.x < 0)
		vitri.x = 0;
	if (vitri.x > this->getBoundingBox().getMaxX())
		vitri.x = this->getBoundingBox().getMaxX();
	if (vitri.y < 0)
		vitri.y = 0;
	if (vitri.y > PositionErrorY(MaxYPlayer))
		vitri.y = PositionErrorY(MaxYPlayer);
	this->m_spritePlayer->setPosition(vitri);
	//
	this->m_spriteShieldPlayer->setPosition(this->m_spritePlayer->getPosition().x, this->m_spritePlayer->getPosition().y + PositionErrorY(0.05) * this->m_sVisibleSize.height);
}

void GameScene::ResetEnemies()
{
	std::random_shuffle(this->m_VectorEnemy.begin(), this->m_VectorEnemy.end());

	int a = rand_int(0, 31), b = rand_int(0, 31);
	for (int i = 0; i < MaxEnemyX; i++)
	{
		for (int j = 0; j < MaxEnemyY; j++)
		{
			Enemy* enemy = this->m_VectorEnemy.at(i * MaxEnemyY + j);
			Vec2 newPosition = Vec2(this->m_sVisibleSize.width * (13 + i * 8) / 87, this->m_sVisibleSize.height * (65 - 18 - j * 6) / 65);

			if (enemy->GetAlive() == false)
			{
				enemy->setPosition(Vec2(this->m_sVisibleSize.width * rand_int(13, 8 * 8 + 13) / 87, this->m_sVisibleSize.height + 2));
				enemy->ResetNoAlive();
			}
			else
				if (enemy->GetEnableHasDrop())
					enemy->ResetAlive();
				else
					enemy->ResetNoAlive();

			auto moveTo = MoveTo::create(DelayTime_Reset_Enemy, newPosition);
			enemy->runAction(moveTo);

			if (i * MaxEnemyY + j == a || i * MaxEnemyY + j == b)
			{
				enemy->SetEnableBalloon(true);
			}
			enemy->SetEnablePhysicBodyEnemy(true);
		}
	}
}

void GameScene::SetPowerBar()
{
	this->m_spritePowerBar->setScaleY(Scale_PowerBar_RunX * ScaleError(this->m_spritePowerBar));
	this->m_spritePowerBar->setScaleX(Scale_PowerBar_RunY * ScaleError(this->m_spritePowerBar) * this->m_iEnemyKilled / Full_Enemy_Killed);
}

void GameScene::ShotBulletPlayer()
{
	if (this->m_typeBullet == tBullet_One)
	{
		Sprite* bullet = this->m_vectorBulletPlayer.getObject();
		bullet->setVisible(true);
		bullet->setPosition(this->m_spritePlayer->getPosition().x, this->m_spritePlayer->getPosition().y + PositionErrorY(0.1));
		bullet->setRotation(0.0f);

		bullet->getPhysicsBody()->setEnabled(true);
		bullet->getPhysicsBody()->setRotationEnable(false);
		bullet->getPhysicsBody()->setVelocity(Vec2(0, 1 * this->m_sVisibleSize.height));
	}
	else if (this->m_typeBullet == tBullet_Four)
	{
		for (int i = 1; i <= 4; i++)
		{
			Sprite* bullet = this->m_vectorBulletPlayer.getObject();
			bullet->setVisible(true);
			bullet->setPosition(this->m_spritePlayer->getPosition().x, this->m_spritePlayer->getPosition().y + PositionErrorY(0.1));
			bullet->setRotation(0.0f);

			bullet->getPhysicsBody()->setEnabled(true);
			bullet->getPhysicsBody()->setRotationEnable(false);
			bullet->getPhysicsBody()->setVelocity(Vec2(this->m_sVisibleSize.width * cos((i * Alpha_Four) * PI / 180), this->m_sVisibleSize.height * sin((i * Alpha_Four) * PI / 180)));

			bullet->setRotation(90 - i * Alpha_Four);
		}
	}
	else
	{
		for (int i = 1; i <= 5; i++)
		{
			Sprite* bullet = this->m_vectorBulletPlayer.getObject();
			bullet->setVisible(true);
			bullet->setPosition(this->m_spritePlayer->getPosition().x, this->m_spritePlayer->getPosition().y + PositionErrorY(0.1));
			bullet->setRotation(0.0f);

			bullet->getPhysicsBody()->setEnabled(true);
			bullet->getPhysicsBody()->setRotationEnable(false);
			bullet->getPhysicsBody()->setVelocity(Vec2(this->m_sVisibleSize.width * cos((i * Alpha_Five) * PI / 180), this->m_sVisibleSize.height * sin((i * Alpha_Five) * PI / 180)));

			bullet->setRotation(90 - i * Alpha_Five);
		}
	}
}

void GameScene::RandomBulletEnemy()
{
	Enemy* enemy = this->m_VectorEnemy.at(rand_int(0, this->m_VectorEnemy.size() - 1));

	Vec2 positionMoveBy = Vec2(this->m_spritePlayer->getPosition().x - enemy->getPosition().x,
		this->m_spritePlayer->getPosition().y - enemy->getPosition().y);

	Sprite* spriteBulletEnemy = this->m_vectorBulletEnemy.getObject();

	spriteBulletEnemy->setVisible(true);
	spriteBulletEnemy->getPhysicsBody()->setEnabled(false);
	spriteBulletEnemy->setPosition(enemy->getPosition());

	spriteBulletEnemy->getPhysicsBody()->setEnabled(true);
	spriteBulletEnemy->getPhysicsBody()->setDynamic(true);
	spriteBulletEnemy->getPhysicsBody()->setRotationEnable(false);
	spriteBulletEnemy->getPhysicsBody()->setVelocity(positionMoveBy / 3);

	const int numberSprite = Num_Frame_BulletEnemy_Animation;
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(numberSprite);
	for (int i = 0; i < numberSprite; i++)
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(Path_Sprite_Bullet_Enemy, i) + ".png"));

	Animation* animation = Animation::createWithSpriteFrames(animFrames, Delay_Time_Animation_BulletEnemy);
	Animate* animate = Animate::create(animation);

	spriteBulletEnemy->runAction(RepeatForever::create(animate));
}

#pragma endregion

#pragma region MegaBomb

void GameScene::BeginMegaBombAction()
{
	this->m_bPauseGame = true;
	this->m_spriteMegaBomb->setVisible(true);
	this->m_spriteShieldPlayer->setVisible(true);
}

void GameScene::EndMegaBombAction()
{
	this->m_spriteMegaBomb->setVisible(false);
	this->RemoveAllEnemy();
	this->m_spriteShieldPlayer->setVisible(false);
	m_spriteMegaBomb->setSpriteFrame((string)Path_Animation_Megabomb);
}

void GameScene::RemoveAllEnemy()
{
	int a = rand_int(0, 31), b = rand_int(0, 31);
	this->m_bRemovedAll = true;
	for (int i = 0; i < MaxEnemyX; i++)
	{
		for (int j = 0; j < MaxEnemyY; j++)
		{
			Enemy* enemy = this->m_VectorEnemy.at(i * MaxEnemyY + j);

			Vec2 newPosition = Vec2(this->m_sVisibleSize.width * (13 + i * 8) / 87, this->m_sVisibleSize.height * (65 - 18 - j * 6) / 65);

			enemy->SetNewPosition(newPosition);

			if (i * MaxEnemyY + j == a || i * MaxEnemyY + j == b)
				enemy->SetBoolBalloon(true);
			else
				enemy->SetBoolBalloon(false);

			if (enemy->GetAlive())
			{
				if (enemy->GetEnableHasDrop() || enemy->GetEnableBalloon())
					this->DropItem(enemy->getPosition()); // Drop Item
				this->Bonus(enemy->getPosition());
				enemy->RunExplosionActionMegaBomb(true);
			}
			else
				enemy->RunExplosionActionMegaBomb(false);
		}
	}
}

void GameScene::RunActionMegaBomb()
{
	this->m_spriteMegaBomb->setVisible(true);
	this->m_spriteMegaBomb->setPosition(this->m_spritePlayer->getPosition().x, this->m_spritePlayer->getPosition().y + PositionErrorY(0.1));

	//auto moveTo = MoveTo::create(DelayTimeActionMegabomb_Up, Vec2(this->m_sVisibleSize.width / 2, this->m_sVisibleSize.height * Position_Player_Y(1.5)));
	auto moveTo = MoveTo::create(DelayTimeActionMegabomb_Up, PositionError(1.f / 2, 6.f/10));

	const int numberSprite1 = Number_Frame_MegaBomb1;
	Vector<SpriteFrame*> animFrames1;
	animFrames1.reserve(numberSprite1);
	for (int i = 0; i < numberSprite1; i++)
		animFrames1.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(Path_Animation_Megabomb, i) + ".png"));

	Animation* animation1 = Animation::createWithSpriteFrames(animFrames1, Delay_Time_MegaBomb_Action);
	Animate* animate1 = Animate::create(animation1);

	const int numberSprite = Number_Frame_MegaBomb0;
	Vector<SpriteFrame*> animFrames;
	animFrames.reserve(numberSprite);
	for (int i = 0; i < numberSprite; i++)
		animFrames.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(Path_Sprite_MegaBomb, i) + ".png"));

	Animation* animation = Animation::createWithSpriteFrames(animFrames, Delay_Time_MegaBomb_Action);
	Animate* animate = Animate::create(animation);

	auto seq = Sequence::create(CallFunc::create(CC_CALLBACK_0(GameScene::BeginMegaBombAction, this)),
		moveTo, animate1, animate,
		CallFunc::create(CC_CALLBACK_0(GameScene::EndMegaBombAction, this)), nullptr);

	this->m_spriteMegaBomb->runAction(seq);

	// Anh sang phia sau icon Power
	Sprite* spriteL = Sprite::createWithSpriteFrameName(Path_Sprite_PowerBar2);
	spriteL->setPosition(PositionError(Position_Sprite_Power_W, Position_Sprite_Power_H));
	this->addChild(spriteL, zBACKGROUND);

	auto rotate1 = RotateTo::create(Delta_Time_PowerBar2, 36 * PI);
	auto seq1 = Sequence::create(rotate1, RemoveSelf::create(), nullptr);
	spriteL->runAction(seq1);

	this->m_bResetPowerBar = true;
}

#pragma endregion

Enemy* GameScene::RandEnemy()
{
	for (int i = 0; i < MaxEnemyX * MaxEnemyY - 1; i++)
	{
		Enemy* enemy = this->m_VectorEnemy.at(rand_int(0, this->m_VectorEnemy.size() - 1));
		if (enemy->GetAlive())
			return enemy;
	}
	return nullptr;
}
