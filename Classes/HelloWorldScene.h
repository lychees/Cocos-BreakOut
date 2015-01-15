#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"

using namespace std;
USING_NS_CC;

struct Brick : public Layer{
	int type; // 暂时没用到 ... 
	float x, y, h, w;	
	CREATE_FUNC(Brick); virtual bool init(); void reset(float, float, float, float, int);
};

struct Bricks : public Layer{
	vector<Brick*> body;
	CREATE_FUNC(Bricks); virtual bool init();
};


struct Ball : public Layer{
	float x, y, r, dx, dy; bool attached; // 是否附着在 Paddle 上
	Sprite* body;
	bool move();
	bool bounce(Brick*);
	void bounce(Bricks*);
	void setReflection(float);
	CREATE_FUNC(Ball); virtual bool init(); void reset(float x, float y, float r);
};

struct Paddle : public Layer{
	int dir; float width, x, dx, ddx;

	float mdx; // 速度上限

	Sprite* body; vector<Ball*> attach;

	void move(); void bounce(Ball*);
	CREATE_FUNC(Paddle); virtual bool init();
};




class Game : public Layer{
public:

	int score; Paddle* pad; Ball* ball; Bricks* bricks;

	void gameLogic(float t);
	void backgroundMusicDieDown();

	void onKeyPressed(EventKeyboard::KeyCode, Event*);
	void onKeyReleased(EventKeyboard::KeyCode, Event*);	
	static Scene* scene(); void menuCloseCallback(Ref* sender);
	CREATE_FUNC(Game); virtual bool init();
};

#endif // __HELLOWORLD_SCENE_H__
