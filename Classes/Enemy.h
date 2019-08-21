#pragma once
#include "cocos2d.h"
#include "Config.h"
#include "PhysicsShapeCache.h"

USING_NS_CC;

class Enemy :public Node
{
public:
	virtual bool init();
	CREATE_FUNC(Enemy);

	//TypeEnemy getType() { return this->m_eTypeEnyme; }
	void SetVisible(Size visible);
	void SetType(TYPE_ENEMY m_TypeEneyme);
	void SetEnableHasDrop(bool enable);
	void SetEnableBalloon(bool enable);
	void SetEnablePhysicBodyEnemy(bool enable);
	void SetNewPosition(Vec2 position);
	void SetBoolBalloon(bool enable);

	bool GetEnableHasDrop();
	bool GetEnableBalloon();
	bool GetEnableRemovedMegaBomb();
	Size GetContentSize();

	void RunHasDropAction();

	void RunExplosionAction();
	void RunExplosionActionMegaBomb(bool enableExplosion);

	void DisableSpriteExplosion();

	bool CheckSpriteBallon(Sprite* sprite);
	bool CheckSpriteEnemy(Sprite* sprite);

	void ResetAlive();

	void ResetNoAlive();				

	bool GetAlive();					// Kiem tra trang thai Visible cua Enemy

	void HideEnemy();					// An Enemy khi bi ban boi bullet
	
	void ResetEnemyMegaBomb();			// Reset Enemy sau khi thuc hien mega bomb
protected:
	Sprite* m_spriteEnemy;				// Sprite cho enemy
	Sprite* m_spriteBallon;				// Sprite cho Balloon
	Sprite* m_spriteHasDrop;			// Sprite Drop Item
	Sprite* m_spriteExplosion;			// Sprite Explosion
	TYPE_ENEMY m_TypeEneyme;
	
	Size m_sVisible;					
	Vec2 m_Vec2NewPosition;				// Vi tri cua Enemy sau khi reset
	
	bool m_bEnableBalloon;				// True neu Enemy hien tai co Balloon
	bool m_bEnableRemovedMegaBomb;		// True neu action Explosion chay xong,
};