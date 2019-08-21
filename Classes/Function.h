#pragma once
#include <string.h>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <climits>
#include <random>
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

int rand_int(int min, int max);
string strAddNum(string str, int num);
float ScaleError(cocos2d::Sprite* sprite);
float ScaleError(Size sizeContent);
Vec2 PositionError(float x, float y);
float PositionErrorX(float x);
float PositionErrorY(float y);
float PositionErrorDeltaY(float y);