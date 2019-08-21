#include "Bullet.h"

USING_NS_CC;

bool Bullet::init()
{
	if (!Node::init())
		return false;
	this->m_spBullet = Sprite::createWithSpriteFrameName("1.f0025.png");
	this->m_spBullet->setAnchorPoint(Vec2(0.5, 0));
	this->m_spBullet->setVisible(false);
	this->m_spBullet->setTag(BULLET_PLAYER);

	PhysicsShapeCache::getInstance()->setBodyOnSprite("1.f0025", this->m_spBullet);
	this->m_spBullet->getPhysicsBody()->setEnabled(false);
	this->m_spBullet->getPhysicsBody()->setGravityEnable(false);
	this->m_spBullet->getPhysicsBody()->setContactTestBitmask(true);
	this->addChild(this->m_spBullet);
	return true;
}

void Bullet::CreateBullet(Vec2 positionMain, Size visibleS)
{
	this->m_spBullet->setVisible(true);
	this->m_spBullet->setPosition(positionMain.x, positionMain.y + 0.1 * visibleS.height);
	this->m_spBullet->setRotation(0.0f);

	this->m_spBullet->getPhysicsBody()->setEnabled(true);
	this->m_spBullet->getPhysicsBody()->setVelocity(Vec2(0, 0.1 * visibleS.height));
	this->m_spBullet->getPhysicsBody()->setContactTestBitmask(true);
	this->m_spBullet->getPhysicsBody()->setGravityEnable(false);
	this->m_spBullet->getPhysicsBody()->setRotationEnable(false);

	this->m_spBullet->setScale(0.7);
}

void Bullet::CreateBullet(Vec2 positionMain, Size visibleS, int i)
{
	this->m_spBullet->setVisible(true);
	this->m_spBullet->setPosition(positionMain.x, positionMain.y + 0.1 * visibleS.height);
	this->m_spBullet->setRotation(0.0f);

	this->m_spBullet->getPhysicsBody()->setEnabled(true);
	this->m_spBullet->getPhysicsBody()->setVelocity(Vec2(visibleS.width * cos((i * Alpha_Four) * PI / 180), visibleS.height * sin((i * Alpha_Four) * PI / 180)));
	this->m_spBullet->getPhysicsBody()->setContactTestBitmask(true);
	this->m_spBullet->getPhysicsBody()->setGravityEnable(false);

	this->m_spBullet->setRotation(90 - i * Alpha_Four);
	this->m_spBullet->setScale(0.7);
}
