#pragma once

#pragma region Enum

enum
{
	zBACKGROUND,
	zGAME_BOARD,
	zUI,
};

enum TYPE_BULLET
{
	tBullet_One = 0,
	tBullet_Four,
	tBullet_Five
};

enum TypeObject
{
	PLAYER,
	BULLET_PLAYER,
	BULLET_ENEMY,
	ENEMY,
	BALLOON,
	HAS_DROP,
	ITEM_DROP,
};

enum TYPE_ENEMY
{
	Enemy_O,
	Enemy_U,
	Enemy_O2,
};
#pragma endregion

#pragma region Math

#define PI 3.14159265359

#pragma endregion

#pragma region Path_Sprite

#define Path_Enemy_O "1.f0034"
#define Path_Enemy_U "1.f0046"
#define Path_Enemy_O2 "1.f0058"
#define Path_Sprite_BulletPlayer "1.f0025"
#define Path_Ballon_Sprite "1.f0106"
#define Path_HAS_DROP_Sprite "1.f0139"
#define Path_Sprite_Explosion "1.f0067"
#define Path_Sprite_Player "1.f0012"
#define Path_Sprite_Logo_Heads_Shoulders "1.f0118.png"
#define Path_Sprite_Timer "1.f0074.png"
#define Path_Sprite_Item_Drop "1.f0104"
#define Path_Score_Sprite "1.f0075.png"
#define Path_Sprite_Power "1.f0062.png"
#define Path_Sprite_PowerBar "1.f0064.png"
#define Path_Sprite_PowerBarFull "1.f0065.png"
#define Path_Sprite_Bonus_One "1.f0125.png"
#define Path_Sprite_Bonus_Three "1.f0130.png"
#define Path_Sprite_Bonus_Two "1.f0126.png"
#define Path_Sprite_MegaBomb "1.f0107.png"
#define Path_Sprite_ShieldPlayer "1.f0101.png"
#define Path_Sprite_Bullet_Enemy "1.f0004"
#define Path_Sprite_PowerBar2 "1.f0063.png"

#pragma endregion

#pragma region Preload
#define Path_PLIST_Preload "data/preload.plist"
#define Path_PNG_Preload "data/preload.png"
#define Path_PLIST_PreImage0 "data/preimage--0.plist"
#define Path_PNG_PreImage0 "data/preimage--0.png"
#define Path_PLIST_PreImage1 "data/preimage--1.plist"
#define Path_PNG_PreImage1 "data/preimage--1.png"
#pragma endregion

#pragma region Background
#define Background_Main_Menu "background/background-4.jpg"
#define Background_Game_Scene "background/background-2.jpg"
#pragma endregion

#pragma region PathAnimation

#define Path_Animation_Megabomb "1.f0119.png"
#define Path_Animation_BulletEnemy_Collision_With_Player "1.f0016.png"

#pragma endregion

#pragma region Fonts

#define Path_Font_Label_Timer "fonts/arial.ttf"
#define Path_Font_Label_Score "fonts/arial.ttf"

#pragma endregion

#pragma region Position

#define Position_Sprite_Player_W 44.0f / 87.0f
#define Position_Sprite_Player_H 15.0f / 65.0f
#define Position_Sprite_Timer_W 22.0f / 87.0f
#define Position_Sprite_Timer_H 62.0f / 66.0f
#define Position_Label_Timer_W 28.0f / 87.0f
#define Position_Label_Timer_H 63.0f / 66.0f
#define Position_Sprite_Score_W 37.0f / 87.0f
#define Position_Sprite_Score_H 62.0f / 66.0f
#define Position_Label_Score_W 46.5f / 87.0f
#define Position_Label_Score_H 63.0f / 66.0f
#define Position_Sprite_Power_W 54.0f / 87.0f
#define Position_Sprite_Power_H 62.0f / 66.0f
#define Position_Label_PowerBar_W 62.5f / 86.5f
#define Position_Label_PowerBar_H 62.0f / 66.0f
#define Position_Label_Run_PowerBar_W 55.0f / 87.0f
#define Position_Label_Run_PowerBar_H 63.0f / 66.0f
#define Position_Player_X(i) (13 + i * 8) / 87.0f
#define Position_Player_Y(i) (65 - 18 - i * 6) / 65.0f

#pragma endregion

#pragma region DeltaTime

#define StartTime 30
#define ResetEnemyTime 5.0f
#define TimeReplayScene 5.0f
#define DeltaTimeBulletPlayer 0.3
#define DeltaTimeBulletEnemy 0.65

#pragma endregion

#pragma region Scale

#define Scale_Enemy 0.1f
#define Scale_Enemy_ALl 0.05f
#define Scale_Background 0.7
#define Scale_Logo_Heads_Shoulders 0.2
#define Scale_PowerBar 0.07
#define Scale_Icon_Hub 0.07
#define Scale_Bullet_Player 0.015
#define Scale_Bullet_Enemy 0.04
#define Scale_Drop_Item 0.06
#define Scale_PowerBar_RunX 0.045
#define Scale_PowerBar_RunY 0.045
#define Scale_Balloon 1.05
#define Scale_HasDrop 1.2

#pragma endregion

#pragma region FontsSize

#define Size_Label_Timer 17
#define Size_Label_Score 17

#pragma endregion

#pragma region NumFrameAnimation

#define Num_Frame_Player_Animation 4
#define Num_Frame_Animation_BulletEnemy_Collision_With_Player 6
#define Num_Frame_Explosion_Animation 7
#define Nume_Frame_Sprite_Player 4
#define Number_Frame_MegaBomb0 8
#define Number_Frame_MegaBomb1 4
#define Num_Frame_BulletEnemy_Animation 8

#pragma endregion

#pragma region DelayTimeAction

#define Delay_Time_Animation_Player 0.08f
#define Delay_Time_Animation_BulletEnemy_Collision_With_Player 0.15f
#define Delay_Time_Explosion_Action 0.04f
#define Delay_Time_MegaBomb_Action 0.125
#define Delta_Time_Action_Scale_Button 2.0f
#define Delay_Time_Animation_BulletEnemy 0.01f
#define DelayTime_Reset_Enemy 1.5
#define DelayTimeActionMegabomb_Up 1
#define Delta_Time_PowerBar2 1.0f
#define Scale_Out_Factor 1.f
#define Scale_Out_Duration 0.5f
#define Scale_In_Factor 1.25f
#define Scale_In_Duration 0.5f

#pragma endregion

#pragma region MaxNumber

#define MaxEnemyX 8
#define MaxEnemyY 4
#define MaxBullet 20
#define MaxYPlayer 0.28f
#define Full_Enemy_Killed 32.0f
#define MaxDropItem 10
#define Num_Bullet_Enemy 20
#define Num_Bonus_One 100
#define Num_Bonus_Two 200
#define Num_Bonus_Three 300

#pragma endregion

#pragma region SetCollision

#define CategoryItem 0x00000001			// 0001
#define CategoryPlayer 0x00000002		// 0010
#define CategoryEnemy 0x00000004		// 0011
#define CategoryBulletPlayer 0x00000008	// 0100
#define CategoryBulletEnemy 0x00000010	// 0101

#define CollisionItem CategoryPlayer	// 2 = 0010
#define CollisionPlayer (CategoryItem + CategoryEnemy + CategoryBulletEnemy) // 21 = ‭0001 0101‬
#define CollisionEnemy (CategoryPlayer + CategoryBulletPlayer) // 10 = 0000 1010
#define CollisionBulletPlayer (CategoryEnemy + CategoryBulletEnemy) // 20 = ‭0001 0100‬
#define CollisionBulletEnemy (CategoryPlayer + CategoryBulletPlayer) // 10 = 0000 1010

#pragma endregion

#pragma region Angle

#define RadiusBullet 0.8
#define Alpha_Four 36
#define Alpha_Five 30

#pragma endregion

#pragma region Tag

#define Tag_MoveTo_DropItem_Action 1

#pragma endregion

#pragma region ResolutionSize

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

#pragma endregion
