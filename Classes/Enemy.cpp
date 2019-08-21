#include "Enemy.h"
#include "Function.h"
#include <string.h>
#include <string>
#include <iomanip>
#include <sstream>

USING_NS_CC;

void setAnimationForSprite(Sprite* sprite, const std::string fileName, const int numFram)
{
	Vector<SpriteFrame*> animFramesEnemy;
	animFramesEnemy.reserve(numFram);
	for (int i = 0; i < numFram; i++)
		animFramesEnemy.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(fileName, i) + ".png"));

	Animation* animation1 = Animation::createWithSpriteFrames(animFramesEnemy, (float)rand_int(10, 13) / 100.0);
	Animate* animate1 = Animate::create(animation1);
	sprite->runAction(RepeatForever::create(animate1));
}

void setPhysicForEnemy(Sprite* sprite, const std::string fileName)
{
	PhysicsShapeCache::getInstance()->setBodyOnSprite(fileName, sprite);
	sprite->getPhysicsBody()->setContactTestBitmask(CollisionEnemy);
	sprite->getPhysicsBody()->setCategoryBitmask(CategoryEnemy);
	sprite->getPhysicsBody()->setCollisionBitmask(CollisionEnemy);
	sprite->getPhysicsBody()->setDynamic(false);
}

void setFrameForEnemy(Sprite* sprite, const std::string fileName)
{
	sprite->setSpriteFrame(fileName + ".png");
}

bool Enemy::init()
{
	if (!Node::init())
		return false;

	// Sprite cho enemy chinh
	this->m_spriteEnemy = Sprite::create();
	//this->m_spriteEnemy->setScale(Scale_Enemy);
	this->m_spriteEnemy->setTag(ENEMY);
	this->addChild(this->m_spriteEnemy, zGAME_BOARD);

	// Sprite cho bong bong
	this->m_spriteBallon = Sprite::createWithSpriteFrameName((string)Path_Ballon_Sprite + ".png");
	this->m_spriteBallon->setScale(Scale_Balloon);
	this->m_spriteBallon->setPosition(0, 5);
	this->m_spriteBallon->setTag(BALLOON);
	this->m_spriteBallon->setVisible(false);

	PhysicsShapeCache::getInstance()->setBodyOnSprite(Path_Ballon_Sprite, this->m_spriteBallon);
	this->m_spriteBallon->getPhysicsBody()->setDynamic(false);
	this->m_spriteBallon->getPhysicsBody()->setContactTestBitmask(CollisionEnemy);
	this->m_spriteBallon->getPhysicsBody()->setCategoryBitmask(CategoryEnemy);
	this->m_spriteBallon->getPhysicsBody()->setCollisionBitmask(CollisionEnemy);
	this->m_spriteBallon->getPhysicsBody()->setEnabled(false);
	this->addChild(this->m_spriteBallon, zGAME_BOARD);

	// Sprite has drop

	this->m_spriteHasDrop = Sprite::createWithSpriteFrameName((string)Path_HAS_DROP_Sprite + ".png");
	this->m_spriteHasDrop->setScale(Scale_HasDrop);
	this->m_spriteHasDrop->setPosition(0, 5);
	this->m_spriteHasDrop->setTag(HAS_DROP);
	this->m_spriteHasDrop->setVisible(false);
	this->addChild(this->m_spriteHasDrop, zBACKGROUND);

	// Sprite No
	this->m_spriteExplosion = Sprite::createWithSpriteFrameName((string)Path_Sprite_Explosion + ".png");
	this->m_spriteExplosion->setPosition(0, 5);
	//this->m_spriteExplosion->setScale(Scale_Enemy);
	this->m_spriteExplosion->setVisible(false);
	this->addChild(this->m_spriteExplosion, zBACKGROUND);

	this->m_bEnableRemovedMegaBomb = true;
	this->m_bEnableBalloon = false;

	return true;
}

void Enemy::SetVisible(Size visible)
{
	this->m_sVisible = visible;
}

void Enemy::SetType(TYPE_ENEMY m_TypeEneyme)
{
	this->m_TypeEneyme = m_TypeEneyme;
	std::string fileName;
	if (this->m_TypeEneyme == Enemy_O)
	{
		fileName = Path_Enemy_O;
	}
	else if (this->m_TypeEneyme == Enemy_U)
	{
		fileName = Path_Enemy_U;
	}
	else if (this->m_TypeEneyme == Enemy_O2)
	{
		fileName = Path_Enemy_O2;
	}
	setFrameForEnemy(this->m_spriteEnemy, fileName);
	setPhysicForEnemy(this->m_spriteEnemy, fileName);
	setAnimationForSprite(this->m_spriteEnemy, fileName, Nume_Frame_Sprite_Player);
}

void Enemy::SetEnableHasDrop(bool enable)
{
	this->m_spriteHasDrop->setVisible(enable);
}

void Enemy::SetEnableBalloon(bool enable)
{
	this->m_spriteBallon->setVisible(enable);
	this->m_spriteBallon->getPhysicsBody()->setEnabled(enable);
}

void Enemy::SetEnablePhysicBodyEnemy(bool enable)
{
	this->m_spriteEnemy->getPhysicsBody()->setEnabled(enable);
}

void Enemy::SetNewPosition(Vec2 position)
{
	this->m_Vec2NewPosition = position;
}

void Enemy::SetBoolBalloon(bool enable)
{
	this->m_bEnableBalloon = enable;
}

bool Enemy::GetEnableHasDrop()
{
	return this->m_spriteHasDrop->isVisible();
}

bool Enemy::GetEnableBalloon()
{
	return this->m_spriteBallon->isVisible();
}

bool Enemy::GetEnableRemovedMegaBomb()
{
	return this->m_bEnableRemovedMegaBomb;
}

Size Enemy::GetContentSize()
{
	return this->m_spriteEnemy->getContentSize();
}

void Enemy::RunHasDropAction()
{
	this->m_spriteHasDrop->setVisible(true);
	auto scaleTo = ScaleTo::create(Scale_Out_Duration, Scale_Out_Factor);
	auto scaleToClone = ScaleTo::create(Scale_In_Duration, Scale_In_Factor);
	auto seq = Sequence::create(scaleTo, scaleToClone, nullptr);
	this->m_spriteHasDrop->runAction(RepeatForever::create(seq));
}

void Enemy::RunExplosionAction()
{
	this->m_spriteExplosion->setVisible(true);
	const int numberSpriteEnemy = Num_Frame_Explosion_Animation;
	Vector<SpriteFrame*> animFramesEnemy;
	animFramesEnemy.reserve(numberSpriteEnemy);
	for (int i = 0; i < numberSpriteEnemy; i++)
		animFramesEnemy.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(Path_Sprite_Explosion, i) + ".png"));

	Animation* animation1 = Animation::createWithSpriteFrames(animFramesEnemy, Delay_Time_Explosion_Action);
	Animate* animate1 = Animate::create(animation1);
	auto seq = Sequence::create(animate1, CallFunc::create(CC_CALLBACK_0(Enemy::DisableSpriteExplosion, this)), nullptr);
	this->m_spriteExplosion->runAction(seq);
}

void Enemy::RunExplosionActionMegaBomb(bool enableExplosion)
{
	this->m_spriteExplosion->setVisible(enableExplosion);

	this->HideEnemy();

	this->m_bEnableRemovedMegaBomb = false;

	this->m_spriteEnemy->setVisible(false);
	this->SetEnablePhysicBodyEnemy(false);

	const int numberSpriteEnemy = Num_Frame_Explosion_Animation + 1;
	Vector<SpriteFrame*> animFramesEnemy;
	animFramesEnemy.reserve(numberSpriteEnemy);
	for (int i = 0; i < numberSpriteEnemy; i++)
		animFramesEnemy.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName(strAddNum(Path_Sprite_Explosion, i) + ".png"));

	Animation* animation1 = Animation::createWithSpriteFrames(animFramesEnemy, Delay_Time_Explosion_Action);
	Animate* animate1 = Animate::create(animation1);
	auto seq = Sequence::create(animate1, CallFunc::create(CC_CALLBACK_0(Enemy::ResetEnemyMegaBomb, this)), nullptr);
	this->m_spriteExplosion->runAction(seq);
}

void Enemy::DisableSpriteExplosion()
{
	this->m_spriteExplosion->setVisible(false);
}

bool Enemy::CheckSpriteBallon(Sprite* sprite)
{
	return this->m_spriteBallon == sprite;
}

bool Enemy::CheckSpriteEnemy(Sprite* sprite)
{
	return this->m_spriteEnemy == sprite;
}

void Enemy::ResetAlive()
{
	this->m_spriteEnemy->setVisible(true);
	this->m_spriteBallon->setVisible(false);
	this->m_spriteBallon->getPhysicsBody()->setEnabled(false);
}

void Enemy::ResetNoAlive()
{
	this->m_spriteEnemy->setVisible(true);
	this->m_spriteBallon->setVisible(false);
	this->m_spriteBallon->getPhysicsBody()->setEnabled(false);
	this->m_spriteHasDrop->setVisible(false);
	this->m_spriteHasDrop->stopAllActions();
}

bool Enemy::GetAlive()
{
	return this->m_spriteEnemy->isVisible();
}

void Enemy::HideEnemy()
{
	this->m_spriteEnemy->setVisible(false);
	this->m_spriteEnemy->getPhysicsBody()->setEnabled(false);
	this->m_spriteBallon->setVisible(false);
	this->m_spriteBallon->getPhysicsBody()->setEnabled(false);
	this->m_spriteHasDrop->setVisible(false);
	this->m_spriteHasDrop->stopAllActions();
}

void Enemy::ResetEnemyMegaBomb()
{
	this->m_spriteExplosion->setVisible(false);

	this->ResetNoAlive();

	this->setPosition(Vec2(this->m_sVisible.width * rand_int(13, 8 * 8 + 13) / 87, this->m_sVisible.height + 2));

	this->stopAllActions();
	auto moveTo = MoveTo::create(DelayTime_Reset_Enemy, this->m_Vec2NewPosition);
	this->runAction(moveTo);

	this->SetEnableBalloon(this->m_bEnableBalloon);

	this->SetEnablePhysicBodyEnemy(true);
	this->m_bEnableRemovedMegaBomb = true;
}
