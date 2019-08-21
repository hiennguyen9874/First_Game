#pragma once
#include "cocos2d.h"
#include "Config.h"
#include "ObjectPool.h"
#include "PhysicsShapeCache.h"
#include "Enemy.h"
#include "Bullet.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);

	void update(float dt);

	void CreateBackground();
	void CreatePlane();
	void InitEnemy();
	void InitItem();
	void InitMegaBomb();
	void InitPowerBar();
	void InitBulletPlayer();
	void InitBulletEnemy();

	bool OnContactBegin(cocos2d::PhysicsContact& contact);
	void BulletCollisionsWithEnemy(Sprite* bullet, Sprite* spEnemy);
	void BulletCollisionsWithBallon(Sprite* bullet, Sprite* ballon);
	void PlayerCollisionsWithItem(Sprite* player, Sprite* Item);
	void PlayerCollisionsWithBulletEnemy(Sprite* player, Sprite* bulletEnemy);

	Enemy* FindSpriteBB(Sprite* sprite);	// Return enemy co eneme.m_spriteBalloon == sprite
	Enemy* FindSpriteMain(Sprite* sprite);	// Return enemy co eneme.m_spriteEnemy == sprite
	void CheckBulletPlayerOut();
	void CheckItemOutScreen();
	bool IsResumeGame();

	void DropItem(Vec2 position);			// Run Action drop item
	void Bonus(Vec2 position);				// Run Action bonus score 

	void UpdatePosition(float dt);			// Update vi tri cua may bay
	void ResetEnemies();					// Reset enemy sau 5s
	void SetPowerBar();
	void ShotBulletPlayer();
	void RandomBulletEnemy();
	
	void BeginMegaBombAction();
	void EndMegaBombAction();
	void RemoveAllEnemy();
	void RunActionMegaBomb();

	Enemy* RandEnemy();
private:
	Sprite* m_spritePlayer;
	TYPE_BULLET m_typeBullet;
	Size m_sVisibleSize;

	float m_fTimeBulletPlayer;	// Thoi gian reset bullet player
	float m_fTimeBulletEnemy;	// Thoi gian reset bullet enemy
	float m_fTimeGame;			// Thoi gian choi game. 30s -> 0s
	float m_ftimeResetEnemy;	// Thoi gian de reset toan bo enemy sau 5s
	float m_fTimeResetPowerBar;	// Thoi gian reset power bar
	float m_fTimeReplayScene;

	Label* m_LabelTime;
	Label* m_LabelScore;
	Label* m_LabelScoreEnd;

	float m_iScore;
	float m_iEnemyKilled;

	ObjectPool<Sprite*> m_vectorBulletPlayer;	// Vector of bullet player
	ObjectPool<Sprite*> m_vectorBulletEnemy;	// Vector of bullet enemy
	ObjectPool<Sprite*> m_vectorItem;			// Vector of drop item
	Vector<Enemy*> m_VectorEnemy;				// Vector of Enemy

	Vec2 m_vec2DesPosition;						// Vi tri player can toi

	Sprite* m_spriteMegaBomb;
	Sprite* m_spritePowerBar;					
	Sprite* m_spriteShieldPlayer;
	Sprite* m_spriteEnd;
	
	bool m_bResetPowerBar;	// True -> Yeu cau reset Power bar
	bool m_bPauseGame;		// True neu dang thuc hien Mega Bomb -> Ngung ban.
	bool m_bRemovedAll;		// True neu vua thuc hien Megabomb -> khong thuc hien ResetEnemies().
};

