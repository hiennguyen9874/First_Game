#include <string.h>
#include <iomanip>
#include <sstream>
#include <math.h>
#include <climits>
#include <random>
#include "Function.h"
#include "Config.h"

using namespace std;

string strAddNum(string str, int num)
{
	string sub = str.substr(3, 4);
	int num1 = stoi(sub) + num;
	stringstream ss;
	ss << setfill('0') << setw(4) << num1;
	str = "1.f" + ss.str();
	return str;
}

float ScaleError(cocos2d::Sprite* sprite)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return std::max(visibleSize.width / sprite->getContentSize().width, visibleSize.height / sprite->getContentSize().height);
}

float ScaleError(Size sizeContent)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return std::max(visibleSize.width / sizeContent.width, visibleSize.height / sizeContent.height);
}

Vec2 PositionError(float x, float y)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return Vec2((origin.x + (visibleSize.width * x)), (origin.y + (visibleSize.height * y)));
}

float PositionErrorX(float x)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return origin.x + (visibleSize.width * x);
}

float PositionErrorY(float y)
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	return (origin.y + (visibleSize.height * y));
}

// Random integer
int rand_int(int min, int max)
{
	std::random_device rd;  //Will be used to obtain a seed for the random number engine
	std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	std::uniform_int_distribution<> dis(min, max);
	return dis(gen);
}